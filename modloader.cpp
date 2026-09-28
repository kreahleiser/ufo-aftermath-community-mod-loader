#define WIN32_LEAN_AND_MEAN
#include "modloader.h"
#include "vfs.h"
#include "fontpad.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "alpine_embed.h"

static int g_baked;
static int g_baking;
static char g_gameOverride[MAX_PATH];

extern "C" void ModLog(const char *fmt, ...);

extern "C" void ModLoaderSetGameDir(const char *p)
{
    strncpy(g_gameOverride, p, MAX_PATH - 1);
    g_gameOverride[MAX_PATH - 1] = 0;
}

static void gamedir(char *out, int n)
{
    if (g_gameOverride[0]) {
        strncpy(out, g_gameOverride, n - 1);
        out[n - 1] = 0;
        size_t L = strlen(out);
        if (L && out[L - 1] != '\\') {
            if (L + 1 < (size_t)n) { out[L] = '\\'; out[L + 1] = 0; }
        }
        return;
    }
    GetModuleFileNameA(GetModuleHandleA(NULL), out, n);
    char *slash = strrchr(out, '\\');
    if (slash) slash[1] = 0;
    else out[0] = 0;
}

static void wipe_dir(const char *path)
{
    char pat[MAX_PATH];
    _snprintf(pat, MAX_PATH, "%s\\*", path);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pat, &fd);
    if (h == INVALID_HANDLE_VALUE) {
        RemoveDirectoryA(path);
        return;
    }
    do {
        if (fd.cFileName[0] == '.' && (fd.cFileName[1] == 0 || (fd.cFileName[1] == '.' && fd.cFileName[2] == 0)))
            continue;
        char child[MAX_PATH];
        _snprintf(child, MAX_PATH, "%s\\%s", path, fd.cFileName);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) wipe_dir(child);
        else DeleteFileA(child);
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    RemoveDirectoryA(path);
}

static unsigned int hash_blob(unsigned int h, const char *n, DWORD size, DWORD time)
{
    while (*n) { h ^= (unsigned char)*n++; h *= 16777619u; }
    h ^= size; h *= 16777619u;
    h ^= time; h *= 16777619u;
    return h;
}

static unsigned int hash_mods(const char *mods)
{
    unsigned int h = 2166136261u;
    char pat[MAX_PATH];
    WIN32_FIND_DATAA fd;
    _snprintf(pat, MAX_PATH, "%s\\*.lua", mods);
    HANDLE f = FindFirstFileA(pat, &fd);
    if (f != INVALID_HANDLE_VALUE) {
        do {
            h = hash_blob(h, fd.cFileName, fd.nFileSizeLow, fd.ftLastWriteTime.dwLowDateTime);
        } while (FindNextFileA(f, &fd));
        FindClose(f);
    }
    _snprintf(pat, MAX_PATH, "%s\\*.param", mods);
    f = FindFirstFileA(pat, &fd);
    if (f != INVALID_HANDLE_VALUE) {
        do {
            h = hash_blob(h, fd.cFileName, fd.nFileSizeLow, fd.ftLastWriteTime.dwLowDateTime);
        } while (FindNextFileA(f, &fd));
        FindClose(f);
    }
    return h;
}

static int read_lua_param(const char *lua_path)
{
    char p[MAX_PATH];
    strncpy(p, lua_path, MAX_PATH - 1);
    p[MAX_PATH - 1] = 0;
    char *dot = strrchr(p, '.');
    if (dot) strcpy(dot, ".param");
    else strcat(p, ".param");
    FILE *f = fopen(p, "r");
    if (!f) return 0;
    int v = 0;
    if (fscanf(f, "%d", &v) != 1) v = 0;
    fclose(f);
    return v;
}

static int list_lua(const char *mods, std::vector<std::string> *out)
{
    char pat[MAX_PATH];
    _snprintf(pat, MAX_PATH, "%s\\*.lua", mods);
    WIN32_FIND_DATAA fd;
    HANDLE f = FindFirstFileA(pat, &fd);
    if (f == INVALID_HANDLE_VALUE) return 0;
    do {
        out->push_back(fd.cFileName);
    } while (FindNextFileA(f, &fd));
    FindClose(f);
    for (size_t a = 0; a < out->size(); a++)
        for (size_t b = a + 1; b < out->size(); b++)
            if (_stricmp((*out)[a].c_str(), (*out)[b].c_str()) > 0) {
                std::string t = (*out)[a]; (*out)[a] = (*out)[b]; (*out)[b] = t;
            }
    return (int)out->size();
}

static int l_print(lua_State *L)
{
    int n = lua_gettop(L);
    for (int i = 1; i <= n; i++) {
        const char *s = lua_tostring(L, i);
        if (!s) s = lua_typename(L, lua_type(L, i));
        ModLog("%s%s", s, i < n ? "\t" : "\n");
    }
    if (n == 0) ModLog("\n");
    return 0;
}

static int parse_quoted(const char *s, char out[][MAX_PATH], int maxn)
{
    int n = 0;
    while (*s && n < maxn) {
        while (*s && *s != '"') s++;
        if (*s != '"') break;
        s++;
        const char *e = strchr(s, '"');
        if (!e) break;
        int len = (int)(e - s);
        if (len >= MAX_PATH) len = MAX_PATH - 1;
        memcpy(out[n], s, len);
        out[n][len] = 0;
        n++;
        s = e + 1;
    }
    return n;
}

static void trim_slash(char *s)
{
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\\' || s[n - 1] == '/')) s[--n] = 0;
}

static int l_mkdir(lua_State *L)
{
    char buf[MAX_PATH];
    strncpy(buf, luaL_checkstring(L, 1), MAX_PATH - 1);
    buf[MAX_PATH - 1] = 0;
    trim_slash(buf);
    for (char *p = buf; *p; p++) {
        if (*p == '/' ) *p = '\\';
    }
    char *p = buf;
    if (p[0] && p[1] == ':') p += 2;
    if (*p == '\\') p++;
    for (; *p; p++) {
        if (*p == '\\') {
            *p = 0;
            CreateDirectoryA(buf, 0);
            *p = '\\';
        }
    }
    CreateDirectoryA(buf, 0);
    lua_pushinteger(L, 0);
    return 1;
}

static int copy_one(const char *src, const char *dst)
{
    char dest[MAX_PATH];
    DWORD attr = GetFileAttributesA(dst);
    if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY)) {
        const char *base = strrchr(src, '\\');
        if (!base) base = src;
        else base++;
        _snprintf(dest, MAX_PATH, "%s\\%s", dst, base);
    } else {
        strncpy(dest, dst, MAX_PATH - 1);
        dest[MAX_PATH - 1] = 0;
    }
    return CopyFileA(src, dest, FALSE) ? 0 : 1;
}

static int l_copy(lua_State *L)
{
    char src[MAX_PATH], dst[MAX_PATH];
    strncpy(src, luaL_checkstring(L, 1), MAX_PATH - 1);
    strncpy(dst, luaL_checkstring(L, 2), MAX_PATH - 1);
    src[MAX_PATH - 1] = 0;
    dst[MAX_PATH - 1] = 0;
    trim_slash(src);
    trim_slash(dst);
    if (strchr(src, '*') || strchr(src, '?')) {
        char dir[MAX_PATH];
        strncpy(dir, src, MAX_PATH - 1);
        dir[MAX_PATH - 1] = 0;
        char *slash = strrchr(dir, '\\');
        if (slash) *slash = 0;
        WIN32_FIND_DATAA fd;
        HANDLE h = FindFirstFileA(src, &fd);
        if (h == INVALID_HANDLE_VALUE) {
            lua_pushinteger(L, 1);
            return 1;
        }
        int rc = 0;
        do {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
            char from[MAX_PATH];
            _snprintf(from, MAX_PATH, "%s\\%s", dir, fd.cFileName);
            if (copy_one(from, dst) != 0) rc = 1;
        } while (FindNextFileA(h, &fd));
        FindClose(h);
        lua_pushinteger(L, rc);
        return 1;
    }
    lua_pushinteger(L, copy_one(src, dst));
    return 1;
}

static int l_os_execute(lua_State *L)
{
    const char *cmd = luaL_optstring(L, 1, NULL);
    if (!cmd) {
        lua_pushinteger(L, 0);
        return 1;
    }
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(si));
    memset(&pi, 0, sizeof(pi));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    char buf[4096];
    _snprintf(buf, sizeof(buf), "cmd.exe /c %s", cmd);
    if (!CreateProcessA(NULL, buf, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        lua_pushinteger(L, 1);
        return 1;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    lua_pushinteger(L, (int)code);
    return 1;
}

static int l_pad_font(lua_State *L)
{
    const char *tga = luaL_checkstring(L, 1);
    const char *cfg = luaL_checkstring(L, 2);
    char err[256];
    err[0] = 0;
    int ok = pad_font_atlas(tga, cfg, err, 256);
    if (!ok) ModLog("pad_font failed: %s (%s)\n", err, tga);
    lua_pushboolean(L, ok ? 1 : 0);
    return 1;
}

static int l_vfs(lua_State *L)
{
    const char *params = luaL_checkstring(L, 1);
    ModLog("vfs %s\n", params);
    char args[4][MAX_PATH];
    int n = parse_quoted(params, args, 4);
    if (n < 3) {
        lua_pushboolean(L, 0);
        return 1;
    }
    char err[256];
    err[0] = 0;
    int ok = vfs_extract_file(args[0], args[1], args[2], err, 256);
    if (!ok) ModLog("extract failed: %s\n", err);
    lua_pushboolean(L, ok ? 1 : 0);
    return 1;
}

static int run_buf(lua_State *L, const char *buf, const char *name)
{
    if (luaL_loadbuffer(L, buf, strlen(buf), name) || lua_pcall(L, 0, 0, 0)) {
        ModLog("lua error in %s: %s\n", name, lua_tostring(L, -1));
        lua_pop(L, 1);
        return 0;
    }
    return 1;
}

static int bake(const char *game, const char *mods, const std::vector<std::string> &luas)
{
    char tdt[MAX_PATH];
    _snprintf(tdt, MAX_PATH, "%sufo_tdt", game);
    wipe_dir(tdt);
    CreateDirectoryA(tdt, 0);

    lua_State *L = luaL_newstate();
    if (!L) { ModLog("luaL_newstate failed\n"); return 0; }
    luaL_openlibs(L);
    lua_register(L, "print", l_print);
    lua_register(L, "vfs", l_vfs);
    lua_register(L, "pad_font", l_pad_font);
    lua_register(L, "mkdir", l_mkdir);
    lua_register(L, "copy", l_copy);
    lua_getglobal(L, "os");
    lua_pushcfunction(L, l_os_execute);
    lua_setfield(L, -2, "execute");
    lua_pop(L, 1);
    {
        char gp[MAX_PATH];
        strncpy(gp, game, MAX_PATH - 1);
        gp[MAX_PATH - 1] = 0;
        size_t n = strlen(gp);
        if (n && (gp[n - 1] == '\\' || gp[n - 1] == '/')) gp[n - 1] = 0;
        lua_pushstring(L, gp);
        lua_setglobal(L, "GAMEPATH");
        lua_pushstring(L, tdt);
        lua_setglobal(L, "PATH");
        char md[MAX_PATH];
        _snprintf(md, MAX_PATH, "%s\\mods", gp);
        lua_pushstring(L, md);
        lua_setglobal(L, "MODDIR");
        lua_pushnumber(L, 0);
        lua_setglobal(L, "PARAM"); /* overwritten per plugin below */
        lua_pushboolean(L, 0);
        lua_setglobal(L, "GDFileAdded");
        lua_pushboolean(L, 0);
        lua_setglobal(L, "LocFileAdded");
    }

    static const char kShim[] =
        "KEY=1\nVALUE=2\n"
        "if not string.gfind then string.gfind = string.gmatch end\n"
        "do local ol, oo = io.lines, io.open\n"
        "io.lines = function(fn)\n"
        "  if type(fn)=='string' then fn = string.gsub(fn, '^plugins\\\\', MODDIR..'\\\\') end\n"
        "  return ol(fn)\n"
        "end\n"
        "io.open = function(fn, mode)\n"
        "  if type(fn)=='string' then fn = string.gsub(fn, '^plugins\\\\', MODDIR..'\\\\') end\n"
        "  return oo(fn, mode)\n"
        "end\n"
        "end\n";
    if (!run_buf(L, kShim, "shim")) { lua_close(L); return 0; }
    if (!run_buf(L, kCoreLua, "core.lua")) { lua_close(L); return 0; }
    if (!run_buf(L, kParseLua, "parse.lua")) { lua_close(L); return 0; }
    if (!run_buf(L, kFilesLua, "files.lua")) { lua_close(L); return 0; }
    if (!run_buf(L, kItemsLua, "items.lua")) { lua_close(L); return 0; }
    if (!run_buf(L, kUtilityLua, "utility.lua")) { lua_close(L); return 0; }
    if (!run_buf(L, kXtentLua, "xtent.lua")) { lua_close(L); return 0; }
    /* copy()/mkdir() exist now; point ALPine's plugins\ paths at mods\ */
    static const char kCopyShim[] =
        "do local oc = copy\n"
        "copy = function(source, dest)\n"
        "  if type(source)=='string' then source = string.gsub(source, '^plugins\\\\', MODDIR..'\\\\') end\n"
        "  return oc(source, dest)\n"
        "end end\n"
        "function getlanguage()\n"
        "  return 'skip-loc'\n"
        "end\n";
    if (!run_buf(L, kCopyShim, "copy_shim")) { lua_close(L); return 0; }

    int ok = 1;
    for (size_t i = 0; i < luas.size(); i++) {
        char p[MAX_PATH];
        char gp[MAX_PATH];
        strncpy(gp, game, MAX_PATH - 1);
        gp[MAX_PATH - 1] = 0;
        size_t n = strlen(gp);
        if (n && gp[n - 1] == '\\') gp[n - 1] = 0;
        _snprintf(p, MAX_PATH, "%s\\mods\\%s", gp, luas[i].c_str());
        int param = read_lua_param(p);
        lua_pushnumber(L, param);
        lua_setglobal(L, "PARAM");
        ModLog("running plugin %s PARAM=%d\n", p, param);
        if (luaL_loadfile(L, p) || lua_pcall(L, 0, 0, 0)) {
            ModLog("plugin failed: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
            ok = 0;
            break;
        }
    }
    lua_getglobal(L, "GDFileAdded");
    int added = lua_toboolean(L, -1);
    lua_pop(L, 1);
    lua_close(L);
    if (!ok) { wipe_dir(tdt); return 0; }
    if (!added) {
        ModLog("plugins ran but added no gamedata files\n");
        wipe_dir(tdt);
        return 1;
    }

    char overlay[MAX_PATH];
    char gp[MAX_PATH];
    strncpy(gp, game, MAX_PATH - 1);
    gp[MAX_PATH - 1] = 0;
    size_t n = strlen(gp);
    if (n && gp[n - 1] == '\\') gp[n - 1] = 0;
    _snprintf(overlay, MAX_PATH, "%s\\modalpine.vfs", gp);
    DeleteFileA(overlay);
    char err[256];
    err[0] = 0;
    if (!vfs_pack_dir(tdt, overlay, err, 256)) {
        ModLog("pack failed: %s\n", err);
        wipe_dir(tdt);
        return 0;
    }
    ModLog("wrote %s\n", overlay);
    wipe_dir(tdt);
    return 1;
}

void EnsureModsApplied(void)
{
    if (g_baked || g_baking) return;
    g_baking = 1;

    char game[MAX_PATH];
    gamedir(game, MAX_PATH);
    char gp[MAX_PATH];
    strncpy(gp, game, MAX_PATH - 1);
    gp[MAX_PATH - 1] = 0;
    size_t n = strlen(gp);
    if (n && gp[n - 1] == '\\') gp[n - 1] = 0;

    char mods[MAX_PATH];
    _snprintf(mods, MAX_PATH, "%s\\mods", gp);
    CreateDirectoryA(mods, 0);

    char readme[MAX_PATH];
    _snprintf(readme, MAX_PATH, "%s\\readme.txt", mods);
    if (GetFileAttributesA(readme) == INVALID_FILE_ATTRIBUTES) {
        const char *txt =
            "Drop ALPine .lua plugins in this folder.\r\n"
            "They are applied on the next game launch into modalpine.vfs.\r\n"
            "gamedata.vfs is never modified.\r\n"
            "Use UFO Aftermath Community Mod Loader to tick plugins on and off.\r\n";
        HANDLE h = CreateFileA(readme, GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
        if (h != INVALID_HANDLE_VALUE) {
            DWORD w;
            WriteFile(h, txt, (DWORD)strlen(txt), &w, 0);
            CloseHandle(h);
        }
    }

    std::vector<std::string> luas;
    list_lua(mods, &luas);
    char overlay[MAX_PATH];
    _snprintf(overlay, MAX_PATH, "%s\\modalpine.vfs", gp);
    if (luas.empty()) {
        DeleteFileA(overlay);
        g_baked = 1;
        g_baking = 0;
        return;
    }

    unsigned int hsh = hash_mods(mods);
    char hashp[MAX_PATH];
    _snprintf(hashp, MAX_PATH, "%s\\ufo_mods.hash", gp);
    unsigned int old = 0;
    FILE *hf = fopen(hashp, "rb");
    if (hf) { fread(&old, 4, 1, hf); fclose(hf); }
    if (old == hsh && GetFileAttributesA(overlay) != INVALID_FILE_ATTRIBUTES) {
        ModLog("mods unchanged, keeping overlay\n");
        g_baked = 1;
        g_baking = 0;
        return;
    }

    ModLog("baking %d plugin(s)\n", (int)luas.size());
    if (bake(game, mods, luas)) {
        hf = fopen(hashp, "wb");
        if (hf) { fwrite(&hsh, 4, 1, hf); fclose(hf); }
    }

    g_baked = 1;
    g_baking = 0;
}
