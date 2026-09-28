#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gamepath.h"

static void trim_slash(char *s)
{
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\\' || s[n - 1] == '/'))
        s[--n] = 0;
}

static void add_unique(char list[][MAX_PATH], int *count, int cap, const char *path)
{
    char p[MAX_PATH];
    strncpy(p, path ? path : "", MAX_PATH - 1);
    p[MAX_PATH - 1] = 0;
    trim_slash(p);
    if (!p[0]) return;
    for (int i = 0; i < *count; i++)
        if (_stricmp(list[i], p) == 0) return;
    if (*count >= cap) return;
    strncpy(list[*count], p, MAX_PATH - 1);
    list[*count][MAX_PATH - 1] = 0;
    (*count)++;
}

int has_ufo_exe(const char *dir)
{
    if (!dir || !dir[0]) return 0;
    char p[MAX_PATH];
    _snprintf(p, MAX_PATH, "%s\\UFO.exe", dir);
    return GetFileAttributesA(p) != INVALID_FILE_ATTRIBUTES;
}

int is_ufo_exe_filename(const char *path)
{
    if (!path || !path[0]) return 0;
    const char *slash = strrchr(path, '\\');
    const char *name = slash ? slash + 1 : path;
    return _stricmp(name, "UFO.exe") == 0;
}

int game_folder_from_ufo_exe(const char *exePath, char *out, int n)
{
    if (!is_ufo_exe_filename(exePath)) return 0;
    char dir[MAX_PATH];
    strncpy(dir, exePath, MAX_PATH - 1);
    dir[MAX_PATH - 1] = 0;
    char *slash = strrchr(dir, '\\');
    if (!slash) return 0;
    *slash = 0;
    if (!has_ufo_exe(dir)) return 0;
    strncpy(out, dir, n - 1);
    out[n - 1] = 0;
    return 1;
}

static int parse_quoted(const char *s, char *out, int n)
{
    while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') s++;
    if (*s != '"') return 0;
    s++;
    int i = 0;
    while (*s && *s != '"' && i < n - 1) {
        if (*s == '\\' && s[1]) {
            s++;
            out[i++] = *s++;
        } else {
            out[i++] = *s++;
        }
    }
    out[i] = 0;
    return out[0] != 0;
}

static void parse_libraryfolders_vdf(const char *vdfPath, char libs[][MAX_PATH], int *n, int cap)
{
    HANDLE h = CreateFileA(vdfPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (h == INVALID_HANDLE_VALUE) return;
    DWORD sz = GetFileSize(h, 0);
    if (sz == INVALID_FILE_SIZE || sz < 8 || sz > 4 * 1024 * 1024) {
        CloseHandle(h);
        return;
    }
    char *buf = (char *)malloc(sz + 1);
    if (!buf) {
        CloseHandle(h);
        return;
    }
    DWORD got = 0;
    if (!ReadFile(h, buf, sz, &got, 0)) got = 0;
    CloseHandle(h);
    buf[got] = 0;
    char *p = buf;
    while ((p = strstr(p, "\"path\"")) != 0) {
        p += 6;
        char lib[MAX_PATH];
        if (parse_quoted(p, lib, MAX_PATH))
            add_unique(libs, n, cap, lib);
    }
    free(buf);
}

static void add_root_if_steam(char roots[][MAX_PATH], int *n, int cap, const char *root)
{
    char vdf[MAX_PATH];
    _snprintf(vdf, MAX_PATH, "%s\\steamapps\\libraryfolders.vdf", root);
    if (GetFileAttributesA(vdf) != INVALID_FILE_ATTRIBUTES)
        add_unique(roots, n, cap, root);
    _snprintf(vdf, MAX_PATH, "%s\\config\\libraryfolders.vdf", root);
    if (GetFileAttributesA(vdf) != INVALID_FILE_ATTRIBUTES)
        add_unique(roots, n, cap, root);
}

static void collect_steam_roots(char roots[][MAX_PATH], int *n, int cap)
{
    char pf[MAX_PATH];
    pf[0] = 0;
    GetEnvironmentVariableA("ProgramFiles(x86)", pf, MAX_PATH);
    if (pf[0]) {
        char r[MAX_PATH];
        _snprintf(r, MAX_PATH, "%s\\Steam", pf);
        add_root_if_steam(roots, n, cap, r);
    }
    pf[0] = 0;
    GetEnvironmentVariableA("ProgramFiles", pf, MAX_PATH);
    if (pf[0]) {
        char r[MAX_PATH];
        _snprintf(r, MAX_PATH, "%s\\Steam", pf);
        add_root_if_steam(roots, n, cap, r);
    }
    add_root_if_steam(roots, n, cap, "C:\\Program Files (x86)\\Steam");
    add_root_if_steam(roots, n, cap, "C:\\Program Files\\Steam");
    add_root_if_steam(roots, n, cap, "C:\\Steam");

    char drives[256];
    DWORD dlen = GetLogicalDriveStringsA(sizeof(drives), drives);
    if (!dlen || dlen >= sizeof(drives)) return;
    for (char *d = drives; *d; d += strlen(d) + 1) {
        if (GetDriveTypeA(d) != DRIVE_FIXED) continue;
        char root[MAX_PATH];
        _snprintf(root, MAX_PATH, "%sSteam", d);
        add_root_if_steam(roots, n, cap, root);
        _snprintf(root, MAX_PATH, "%sSteamLibrary", d);
        add_root_if_steam(roots, n, cap, root);
        _snprintf(root, MAX_PATH, "%sProgram Files (x86)\\Steam", d);
        add_root_if_steam(roots, n, cap, root);
        _snprintf(root, MAX_PATH, "%sProgram Files\\Steam", d);
        add_root_if_steam(roots, n, cap, root);
    }
}

int find_steam_ufo(char *out, int n)
{
    char roots[16][MAX_PATH];
    int nroots = 0;
    collect_steam_roots(roots, &nroots, 16);

    char libs[24][MAX_PATH];
    int nlibs = 0;
    for (int i = 0; i < nroots; i++) {
        add_unique(libs, &nlibs, 24, roots[i]);
        char vdf[MAX_PATH];
        _snprintf(vdf, MAX_PATH, "%s\\steamapps\\libraryfolders.vdf", roots[i]);
        parse_libraryfolders_vdf(vdf, libs, &nlibs, 24);
        _snprintf(vdf, MAX_PATH, "%s\\config\\libraryfolders.vdf", roots[i]);
        parse_libraryfolders_vdf(vdf, libs, &nlibs, 24);
    }

    char found[8][MAX_PATH];
    int nf = 0;
    char preferred[MAX_PATH];
    preferred[0] = 0;
    for (int i = 0; i < nlibs; i++) {
        char game[MAX_PATH];
        _snprintf(game, MAX_PATH, "%s\\steamapps\\common\\UFO Aftermath", libs[i]);
        if (!has_ufo_exe(game)) continue;
        add_unique(found, &nf, 8, game);
        char mgr[MAX_PATH];
        _snprintf(mgr, MAX_PATH, "%s\\ufo_mod_manager.exe", game);
        if (GetFileAttributesA(mgr) != INVALID_FILE_ATTRIBUTES && !preferred[0])
            strncpy(preferred, game, MAX_PATH - 1);
    }
    if (preferred[0]) {
        strncpy(out, preferred, n - 1);
        out[n - 1] = 0;
        return 1;
    }
    if (nf > 0) {
        strncpy(out, found[0], n - 1);
        out[n - 1] = 0;
        return 1;
    }
    return 0;
}

int find_game(char *out, int n)
{
    char exe[MAX_PATH];
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    char *slash = strrchr(exe, '\\');
    if (slash) *slash = 0;
    if (has_ufo_exe(exe)) {
        strncpy(out, exe, n - 1);
        out[n - 1] = 0;
        return 1;
    }
    return find_steam_ufo(out, n);
}
