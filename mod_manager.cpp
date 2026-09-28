#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0501
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <dwmapi.h>
#include <uxtheme.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <vector>
#include "resource.h"
#include "gamepath.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

static HWND g_list;
static HWND g_status;
static HWND g_hint;
static HWND g_darkChk;
static HWND g_main;
static HWND g_modeCombo;
static HWND g_fpsCombo;
static HWND g_chkBorder;
static HWND g_chkCrisp;
static HWND g_chkLog;
static HWND g_modeHint;
static HWND g_paramLabel;
static HWND g_paramEdit;
static HWND g_paramHint;
static HWND g_chkIntro;
static HWND g_chkDev;
static HWND g_regBtn;
static char g_game[MAX_PATH];
static int g_refreshing;
static int g_loadingDisplay;
static int g_paramLoading;
static int g_dark;
static HBRUSH g_brDark;
static HBRUSH g_brList;

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif
#ifndef KEY_WOW64_64KEY
#define KEY_WOW64_64KEY 0x0100
#endif
#ifndef KEY_WOW64_32KEY
#define KEY_WOW64_32KEY 0x0200
#endif

#define UFO_REG_SUBKEY "SOFTWARE\\ALTAR\\UFOAftermath"

static void set_status(const char *s);

static void ini_path(char *out, int n)
{
    GetModuleFileNameA(NULL, out, n);
    char *slash = strrchr(out, '\\');
    if (slash) slash[1] = 0;
    else out[0] = 0;
    strncat(out, "ufo_mod_manager.ini", n - (int)strlen(out) - 1);
}

static void load_dark()
{
    char p[MAX_PATH];
    ini_path(p, MAX_PATH);
    g_dark = GetPrivateProfileIntA("Manager", "DarkMode", 0, p) ? 1 : 0;
}

static void save_dark()
{
    char p[MAX_PATH];
    ini_path(p, MAX_PATH);
    WritePrivateProfileStringA("Manager", "DarkMode", g_dark ? "1" : "0", p);
}

static void load_launch_flags()
{
    char p[MAX_PATH];
    ini_path(p, MAX_PATH);
    int intro = GetPrivateProfileIntA("Launch", "SkipIntro", 0, p);
    int dev = GetPrivateProfileIntA("Launch", "DevConsole", 0, p);
    if (g_chkIntro) SendMessage(g_chkIntro, BM_SETCHECK, intro ? BST_CHECKED : BST_UNCHECKED, 0);
    if (g_chkDev) SendMessage(g_chkDev, BM_SETCHECK, dev ? BST_CHECKED : BST_UNCHECKED, 0);
}

static void save_launch_flags()
{
    char p[MAX_PATH];
    ini_path(p, MAX_PATH);
    int intro = g_chkIntro && SendMessage(g_chkIntro, BM_GETCHECK, 0, 0) == BST_CHECKED;
    int dev = g_chkDev && SendMessage(g_chkDev, BM_GETCHECK, 0, 0) == BST_CHECKED;
    WritePrivateProfileStringA("Launch", "SkipIntro", intro ? "1" : "0", p);
    WritePrivateProfileStringA("Launch", "DevConsole", dev ? "1" : "0", p);
}

static void enable_cheats_cfg(int on)
{
    if (!g_game[0] || !on) return;
    char cfg[MAX_PATH];
    _snprintf(cfg, MAX_PATH, "%s\\config.cfg", g_game);
    FILE *f = fopen(cfg, "rb");
    if (!f) return;
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    std::string s;
    s.resize(n > 0 ? (size_t)n : 0);
    if (n > 0) fread(&s[0], 1, n, f);
    fclose(f);
    if (s.find("KEY \"cheats\"") != std::string::npos) {
        size_t p = s.find("KEY \"cheats\"");
        size_t t = s.find("TRUE", p);
        size_t fpos = s.find("FALSE", p);
        if (t != std::string::npos && (fpos == std::string::npos || t < fpos))
            return;
        if (fpos != std::string::npos) {
            s.replace(fpos, 5, "TRUE");
        }
    } else {
        size_t end = s.find("END_OF_STRUCT");
        if (end == std::string::npos) return;
        s.insert(end, "    KEY \"cheats\" BOOL TRUE\r\n");
    }
    f = fopen(cfg, "wb");
    if (!f) return;
    fwrite(s.data(), 1, s.size(), f);
    fclose(f);
}

static void launch_game()
{
    if (!g_game[0]) {
        set_status("Could not find UFO.exe.");
        return;
    }
    char exe[MAX_PATH];
    _snprintf(exe, MAX_PATH, "%s\\UFO.exe", g_game);
    if (GetFileAttributesA(exe) == INVALID_FILE_ATTRIBUTES) {
        set_status("UFO.exe is missing from the game folder.");
        return;
    }
    int intro = g_chkIntro && SendMessage(g_chkIntro, BM_GETCHECK, 0, 0) == BST_CHECKED;
    int dev = g_chkDev && SendMessage(g_chkDev, BM_GETCHECK, 0, 0) == BST_CHECKED;
    save_launch_flags();
    if (dev) enable_cheats_cfg(1);

    char cmd[1024];
    _snprintf(cmd, 1024, "\"%s\"", exe);
    if (intro || dev) {
        strcat(cmd, " --options");
        if (intro) strcat(cmd, " nointro=true");
        if (dev) strcat(cmd, " enable_system_console=true");
    }

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(si));
    memset(&pi, 0, sizeof(pi));
    si.cb = sizeof(si);
    if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, g_game, &si, &pi)) {
        set_status("Could not start UFO.exe.");
        return;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    set_status("Launching UFO: Aftermath...");
}

static void display_ini_path(char *out, int n)
{
    if (g_game[0])
        _snprintf(out, n, "%s\\ufo_display.ini", g_game);
    else
        _snprintf(out, n, "ufo_display.ini");
}

static const int kFps[] = { 30, 60, 90, 120, 144, 180, 240 };
static const int kFpsN = 7;

static void update_mode_hint()
{
    if (!g_modeCombo || !g_modeHint) return;
    int sel = (int)SendMessage(g_modeCombo, CB_GETCURSEL, 0, 0);
    int mode = (int)SendMessage(g_modeCombo, CB_GETITEMDATA, sel, 0);
    if (mode == 43)
        SetWindowTextA(g_modeHint, "Fills the screen height with 4:3. HUD stays the correct shape. Side bars on widescreen.");
    else
        SetWindowTextA(g_modeHint, "16:9 picture with a stretched HUD. Black bars on the sides of ultrawide monitors.");
}

static void load_display_fields()
{
    if (!g_modeCombo) return;
    g_loadingDisplay = 1;
    char p[MAX_PATH];
    display_ini_path(p, MAX_PATH);
    int mode = GetPrivateProfileIntA("Display", "Mode", 169, p);
    int border = GetPrivateProfileIntA("Display", "Borderless", 1, p);
    int cap = GetPrivateProfileIntA("Display", "FrameLimit", 60, p);
    int crisp = GetPrivateProfileIntA("Display", "CrispUi", 1, p);
    int log = GetPrivateProfileIntA("Display", "Logging", 0, p);

    SendMessage(g_modeCombo, CB_SETCURSEL, (mode == 43) ? 1 : 0, 0);
    update_mode_hint();

    int fpsIdx = 1; /* 60 */
    for (int i = 0; i < kFpsN; i++)
        if (kFps[i] == cap) fpsIdx = i;
    SendMessage(g_fpsCombo, CB_SETCURSEL, fpsIdx, 0);

    SendMessage(g_chkBorder, BM_SETCHECK, border ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(g_chkCrisp, BM_SETCHECK, crisp ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(g_chkLog, BM_SETCHECK, log ? BST_CHECKED : BST_UNCHECKED, 0);
    g_loadingDisplay = 0;
}

static void save_display_fields()
{
    if (g_loadingDisplay || !g_game[0]) return;
    int modeSel = (int)SendMessage(g_modeCombo, CB_GETCURSEL, 0, 0);
    int mode = (int)SendMessage(g_modeCombo, CB_GETITEMDATA, modeSel, 0);
    if (mode != 43 && mode != 169) mode = 169;
    int fpsSel = (int)SendMessage(g_fpsCombo, CB_GETCURSEL, 0, 0);
    int cap = (fpsSel >= 0 && fpsSel < kFpsN) ? kFps[fpsSel] : 60;
    int border = (SendMessage(g_chkBorder, BM_GETCHECK, 0, 0) == BST_CHECKED);
    int crisp = (SendMessage(g_chkCrisp, BM_GETCHECK, 0, 0) == BST_CHECKED);
    int log = (SendMessage(g_chkLog, BM_GETCHECK, 0, 0) == BST_CHECKED);

    char p[MAX_PATH];
    display_ini_path(p, MAX_PATH);
    char body[2048];
    _snprintf(body, sizeof(body),
        "; UFO Aftermath Community Mod Loader\r\n"
        "; Created or updated by the Mod Loader. Delete this file to restore defaults.\r\n"
        "; ALPine plugins: put .lua files in the mods folder next to UFO.exe.\r\n"
        "\r\n"
        "[Display]\r\n"
        "; 169 = 16:9 3D + HUD stretched to 16:9, pillarboxed on ultrawide (default)\r\n"
        "; 43  = largest 4:3 that fits (HUD unstretched, still much bigger than 1024x768)\r\n"
        "Mode=%d\r\n"
        "\r\n"
        "; 1 = borderless fullscreen on the current monitor\r\n"
        "; 0 = do not resize the window\r\n"
        "Borderless=%d\r\n"
        "\r\n"
        "; Cap FPS. Aftermath UI/geoscape break if the GPU runs uncapped (common on AMD).\r\n"
        "; 60 is what the engine was built for.\r\n"
        "FrameLimit=%d\r\n"
        "\r\n"
        "; 1 = nearest-neighbour UI textures (kills bitmap-font cell borders when scaled)\r\n"
        "; 0 = leave the game's linear filtering\r\n"
        "CrispUi=%d\r\n"
        "\r\n"
        "; 1 = write ufo_display.log next to the DLL (for bug reports)\r\n"
        "Logging=%d\r\n",
        mode, border ? 1 : 0, cap, crisp ? 1 : 0, log ? 1 : 0);
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (h == INVALID_HANDLE_VALUE) {
        set_status("Could not write ufo_display.ini.");
        return;
    }
    DWORD w;
    WriteFile(h, body, (DWORD)strlen(body), &w, 0);
    CloseHandle(h);
    set_status("Display settings saved. Restart the game to apply.");
}

static void apply_theme(HWND hwnd)
{
    BOOL use = g_dark ? TRUE : FALSE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &use, sizeof(use));

    const wchar_t *theme = g_dark ? L"DarkMode_Explorer" : L"Explorer";
    if (g_list) {
        SetWindowTheme(g_list, theme, NULL);
        COLORREF bg = g_dark ? RGB(32, 32, 32) : GetSysColor(COLOR_WINDOW);
        COLORREF fg = g_dark ? RGB(240, 240, 240) : GetSysColor(COLOR_WINDOWTEXT);
        ListView_SetBkColor(g_list, bg);
        ListView_SetTextBkColor(g_list, bg);
        ListView_SetTextColor(g_list, fg);
        HWND header = ListView_GetHeader(g_list);
        if (header) SetWindowTheme(header, theme, NULL);
        InvalidateRect(g_list, 0, TRUE);
    }

    HWND child = GetWindow(hwnd, GW_CHILD);
    while (child) {
        char cls[64];
        GetClassNameA(child, cls, 64);
        if (_stricmp(cls, "Button") == 0 || _stricmp(cls, "ComboBox") == 0 || _stricmp(cls, "Edit") == 0)
            SetWindowTheme(child, theme, NULL);
        InvalidateRect(child, 0, TRUE);
        child = GetWindow(child, GW_HWNDNEXT);
    }

    SetClassLongA(hwnd, -10 /* GCL_HBRUSH */, (LONG)(g_dark ? g_brDark : GetSysColorBrush(COLOR_WINDOW)));
    InvalidateRect(hwnd, 0, TRUE);
    RedrawWindow(hwnd, 0, 0, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

struct Plugin {
    std::string file;   /* AHE.lua */
    std::string title;
    int enabled;
    int has_param;
    int param;          /* 0 = unset / plugin default */
    int param_default;  /* 0 = unknown */
};

static void set_status(const char *s)
{
    SetWindowTextA(g_status, s);
}

static void trim_slash(char *s)
{
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\\' || s[n - 1] == '/'))
        s[--n] = 0;
}

static int path_same(const char *a, const char *b)
{
    char aa[MAX_PATH], bb[MAX_PATH];
    strncpy(aa, a ? a : "", MAX_PATH - 1); aa[MAX_PATH - 1] = 0;
    strncpy(bb, b ? b : "", MAX_PATH - 1); bb[MAX_PATH - 1] = 0;
    trim_slash(aa);
    trim_slash(bb);
    return _stricmp(aa, bb) == 0;
}

static int read_game_path_view(REGSAM wow, char *out, DWORD n)
{
    HKEY k;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, UFO_REG_SUBKEY, 0, KEY_READ | wow, &k) != ERROR_SUCCESS)
        return 0;
    DWORD type = 0, sz = n;
    LONG e = RegQueryValueExA(k, "Path", 0, &type, (LPBYTE)out, &sz);
    RegCloseKey(k);
    if (e != ERROR_SUCCESS || (type != REG_SZ && type != REG_EXPAND_SZ) || !out[0])
        return 0;
    return 1;
}

static int registry_fix_active(void)
{
    if (!g_game[0]) return 0;
    char p[MAX_PATH];
    p[0] = 0;
    if (read_game_path_view(KEY_WOW64_32KEY, p, MAX_PATH) && path_same(p, g_game))
        return 1;
    p[0] = 0;
    if (read_game_path_view(KEY_WOW64_64KEY, p, MAX_PATH) && path_same(p, g_game))
        return 1;
    return 0;
}

static LONG write_game_path_view(REGSAM wow, const char *path)
{
    HKEY k;
    DWORD disp = 0;
    LONG e = RegCreateKeyExA(HKEY_LOCAL_MACHINE, UFO_REG_SUBKEY, 0, 0, 0,
        KEY_WRITE | wow, 0, &k, &disp);
    if (e != ERROR_SUCCESS) return e;
    e = RegSetValueExA(k, "Path", 0, REG_SZ, (const BYTE *)path, (DWORD)strlen(path) + 1);
    RegCloseKey(k);
    return e;
}

static LONG apply_registry_fix(const char *path)
{
    char full[MAX_PATH];
    strncpy(full, path, MAX_PATH - 1);
    full[MAX_PATH - 1] = 0;
    trim_slash(full);
    LONG e32 = write_game_path_view(KEY_WOW64_32KEY, full);
    LONG e64 = write_game_path_view(KEY_WOW64_64KEY, full);
    if (e32 == ERROR_ACCESS_DENIED || e64 == ERROR_ACCESS_DENIED)
        return ERROR_ACCESS_DENIED;
    if (e32 != ERROR_SUCCESS) return e32;
    return ERROR_SUCCESS;
}

static LONG delete_game_path_view(REGSAM wow, const char *game)
{
    HKEY k;
    LONG e = RegOpenKeyExA(HKEY_LOCAL_MACHINE, UFO_REG_SUBKEY, 0,
        KEY_READ | KEY_WRITE | wow, &k);
    if (e == ERROR_FILE_NOT_FOUND || e == ERROR_PATH_NOT_FOUND)
        return ERROR_SUCCESS;
    if (e != ERROR_SUCCESS) return e;
    char p[MAX_PATH];
    p[0] = 0;
    DWORD type = 0, sz = MAX_PATH;
    LONG q = RegQueryValueExA(k, "Path", 0, &type, (LPBYTE)p, &sz);
    int ours = (q != ERROR_SUCCESS) || !p[0] || path_same(p, game);
    LONG del = ERROR_SUCCESS;
    if (ours) {
        del = RegDeleteValueA(k, "Path");
        if (del == ERROR_FILE_NOT_FOUND) del = ERROR_SUCCESS;
    }
    DWORD nkeys = 1, nvals = 1;
    RegQueryInfoKeyA(k, 0, 0, 0, &nkeys, 0, 0, &nvals, 0, 0, 0, 0);
    RegCloseKey(k);
    if (ours && nkeys == 0 && nvals == 0)
        RegDeleteKeyExA(HKEY_LOCAL_MACHINE, UFO_REG_SUBKEY, wow, 0);
    return del;
}

static LONG remove_registry_fix(const char *path)
{
    LONG e32 = delete_game_path_view(KEY_WOW64_32KEY, path);
    LONG e64 = delete_game_path_view(KEY_WOW64_64KEY, path);
    if (e32 == ERROR_ACCESS_DENIED || e64 == ERROR_ACCESS_DENIED)
        return ERROR_ACCESS_DENIED;
    if (e32 != ERROR_SUCCESS) return e32;
    return ERROR_SUCCESS;
}

static int process_is_elevated(void)
{
    HANDLE tok = NULL;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &tok))
        return 0;
    TOKEN_ELEVATION el;
    DWORD sz = 0;
    BOOL ok = GetTokenInformation(tok, TokenElevation, &el, sizeof(el), &sz);
    CloseHandle(tok);
    return (ok && el.TokenIsElevated) ? 1 : 0;
}

static int elevate_registry(const char *flag)
{
    char exe[MAX_PATH];
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    char params[MAX_PATH + 40];
    _snprintf(params, sizeof(params), "%s \"%s\"", flag, g_game);
    SHELLEXECUTEINFOA sei;
    memset(&sei, 0, sizeof(sei));
    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = g_main ? g_main : GetDesktopWindow();
    sei.lpVerb = "runas";
    sei.lpFile = exe;
    sei.lpParameters = params;
    sei.nShow = SW_HIDE;
    if (!ShellExecuteExA(&sei)) {
        DWORD err = GetLastError();
        if (err == ERROR_CANCELLED || err == 1223)
            return -1;
        return 0;
    }
    if (!sei.hProcess) return 0;
    WaitForSingleObject(sei.hProcess, INFINITE);
    DWORD code = 1;
    GetExitCodeProcess(sei.hProcess, &code);
    CloseHandle(sei.hProcess);
    return (code == 0) ? 1 : 0;
}

static void refresh_reg_button(void)
{
    if (!g_regBtn) return;
    int have = g_game[0] && has_ufo_exe(g_game);
    EnableWindow(g_regBtn, have ? TRUE : FALSE);
    if (!have) {
        SetWindowTextA(g_regBtn, "Apply Registry Fix");
        return;
    }
    if (registry_fix_active())
        SetWindowTextA(g_regBtn, "Remove Registry Fix");
    else
        SetWindowTextA(g_regBtn, "Apply Registry Fix");
}

static void toggle_registry_fix(void)
{
    if (!g_game[0]) {
        set_status("Could not find UFO.exe next to the mod loader.");
        return;
    }
    int active = registry_fix_active();
    int want_apply = !active;

    if (!process_is_elevated()) {
        int r = elevate_registry(want_apply ? "--registry-apply" : "--registry-remove");
        if (r < 0) {
            set_status("Registry change cancelled.");
            return;
        }
        if (!r) {
            MessageBoxA(g_main,
                "Windows needs administrator permission to write HKLM\\SOFTWARE\\ALTAR\\UFOAftermath\\Path.",
                "UFO Aftermath Community Mod Loader", MB_OK | MB_ICONWARNING);
            set_status("Could not update the registry. Administrator permission is required.");
            return;
        }
    } else {
        LONG e = want_apply ? apply_registry_fix(g_game) : remove_registry_fix(g_game);
        if (e != ERROR_SUCCESS) {
            set_status("Could not update the registry.");
            return;
        }
    }

    refresh_reg_button();
    int now = registry_fix_active();
    if (want_apply && now)
        set_status("Registry Path written for ALPine and other tools. Restart those tools if they are open.");
    else if (!want_apply && !now)
        set_status("Removed the UFO Aftermath Path registry entry.");
    else
        set_status("Could not update the registry. Administrator permission is required.");
}

static int handle_registry_cmdline(const char *cmd)
{
    if (!cmd) return 0;
    while (*cmd == ' ') cmd++;
    int apply = 0, remove = 0;
    if (_strnicmp(cmd, "--registry-apply", 16) == 0) {
        apply = 1;
        cmd += 16;
    } else if (_strnicmp(cmd, "--registry-remove", 17) == 0) {
        remove = 1;
        cmd += 17;
    } else {
        return 0;
    }
    while (*cmd == ' ') cmd++;
    char path[MAX_PATH];
    path[0] = 0;
    if (*cmd == '"') {
        cmd++;
        const char *end = strchr(cmd, '"');
        size_t n = end ? (size_t)(end - cmd) : strlen(cmd);
        if (n >= MAX_PATH) n = MAX_PATH - 1;
        memcpy(path, cmd, n);
        path[n] = 0;
    } else if (*cmd) {
        strncpy(path, cmd, MAX_PATH - 1);
        path[MAX_PATH - 1] = 0;
    }
    if (!path[0] || !has_ufo_exe(path)) {
        if (!find_game(path, MAX_PATH))
            return 2;
    }
    strncpy(g_game, path, MAX_PATH - 1);
    g_game[MAX_PATH - 1] = 0;
    LONG e = apply ? apply_registry_fix(g_game) : remove_registry_fix(g_game);
    return (e == ERROR_SUCCESS) ? 1 : 2;
}

static int lua_param_default(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    char line[1024];
    int def = 0;
    while (fgets(line, sizeof(line), f)) {
        char *s = strstr(line, "PARAM_DEFAULT");
        if (!s) continue;
        s += 13;
        while (*s == ' ' || *s == '\t' || *s == '=' || *s == ':') s++;
        if (isdigit((unsigned char)*s) || *s == '-')
            def = atoi(s);
        if (def) break;
    }
    fclose(f);
    return def;
}

static int lua_uses_param(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    char line[1024];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char *s = line;
        while (*s == ' ' || *s == '\t') s++;
        if (s[0] == '-' && s[1] == '-') continue;
        for (char *p = s; *p; p++) {
            if ((p == s || (!isalnum((unsigned char)p[-1]) && p[-1] != '_'))
                && p[0] == 'P' && p[1] == 'A' && p[2] == 'R' && p[3] == 'A' && p[4] == 'M'
                && !isalnum((unsigned char)p[5]) && p[5] != '_') {
                found = 1;
                break;
            }
        }
        if (found) break;
    }
    fclose(f);
    return found;
}

static void param_path(char *out, int n, const char *dir, const std::string &file)
{
    std::string stem = file;
    size_t d = stem.rfind('.');
    if (d != std::string::npos) stem = stem.substr(0, d);
    _snprintf(out, n, "%s\\%s.param", dir, stem.c_str());
}

static int read_param(const char *dir, const std::string &file)
{
    char p[MAX_PATH];
    param_path(p, MAX_PATH, dir, file);
    FILE *f = fopen(p, "r");
    if (!f) return 0;
    int v = 0;
    if (fscanf(f, "%d", &v) != 1) v = 0;
    fclose(f);
    return v;
}

static void write_param(const char *dir, const std::string &file, int v)
{
    char p[MAX_PATH];
    param_path(p, MAX_PATH, dir, file);
    if (v == 0) {
        DeleteFileA(p);
        return;
    }
    FILE *f = fopen(p, "w");
    if (!f) return;
    fprintf(f, "%d\n", v);
    fclose(f);
}

static std::string stem_of(const std::string &file)
{
    size_t d = file.rfind('.');
    if (d == std::string::npos) return file;
    return file.substr(0, d);
}

static std::string title_of(const char *dir, const std::string &file)
{
    char p[MAX_PATH];
    _snprintf(p, MAX_PATH, "%s\\%s", dir, file.c_str());
    FILE *f = fopen(p, "r");
    if (!f) return file;
    char line[512];
    std::string title = file;
    while (fgets(line, sizeof(line), f)) {
        const char *s = strstr(line, "print(");
        if (!s) continue;
        const char *q = strchr(s, '"');
        if (!q) q = strchr(s, '\'');
        if (!q) continue;
        char qch = *q++;
        const char *e = strchr(q, qch);
        if (!e || e == q) continue;
        title.assign(q, e - q);
        break;
    }
    fclose(f);
    return title;
}

static void collect_lua(const char *dir, int enabled, std::vector<Plugin> *out)
{
    char pat[MAX_PATH];
    _snprintf(pat, MAX_PATH, "%s\\*.lua", dir);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pat, &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        Plugin p;
        p.file = fd.cFileName;
        p.title = title_of(dir, p.file);
        p.enabled = enabled;
        char full[MAX_PATH];
        _snprintf(full, MAX_PATH, "%s\\%s", dir, fd.cFileName);
        p.has_param = lua_uses_param(full);
        p.param_default = p.has_param ? lua_param_default(full) : 0;
        p.param = p.has_param ? read_param(dir, p.file) : 0;
        out->push_back(p);
    } while (FindNextFileA(h, &fd));
    FindClose(h);
}

static int move_tree(const char *src, const char *dst)
{
    SHFILEOPSTRUCTA op;
    memset(&op, 0, sizeof(op));
    char from[MAX_PATH + 2];
    char to[MAX_PATH + 2];
    memset(from, 0, sizeof(from));
    memset(to, 0, sizeof(to));
    strncpy(from, src, MAX_PATH);
    strncpy(to, dst, MAX_PATH);
    op.wFunc = FO_MOVE;
    op.pFrom = from;
    op.pTo = to;
    op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT | FOF_NOCOPYSECURITYATTRIBS;
    return SHFileOperationA(&op) == 0;
}

static int set_enabled(const Plugin &p, int enable)
{
    char mods[MAX_PATH], plug[MAX_PATH];
    _snprintf(mods, MAX_PATH, "%s\\mods", g_game);
    _snprintf(plug, MAX_PATH, "%s\\plugins", g_game);
    CreateDirectoryA(mods, 0);
    CreateDirectoryA(plug, 0);

    const char *fromdir = enable ? plug : mods;
    const char *todir = enable ? mods : plug;

    char src[MAX_PATH], dst[MAX_PATH];
    _snprintf(src, MAX_PATH, "%s\\%s", fromdir, p.file.c_str());
    _snprintf(dst, MAX_PATH, "%s\\%s", todir, p.file.c_str());
    if (GetFileAttributesA(src) == INVALID_FILE_ATTRIBUTES) return 0;
    if (!MoveFileExA(src, dst, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED)) return 0;

    std::string stem = stem_of(p.file);
    char srcd[MAX_PATH], dstd[MAX_PATH];
    _snprintf(srcd, MAX_PATH, "%s\\%s", fromdir, stem.c_str());
    _snprintf(dstd, MAX_PATH, "%s\\%s", todir, stem.c_str());
    if (GetFileAttributesA(srcd) != INVALID_FILE_ATTRIBUTES)
        move_tree(srcd, dstd);

    char srcp[MAX_PATH], dstp[MAX_PATH];
    param_path(srcp, MAX_PATH, fromdir, p.file);
    param_path(dstp, MAX_PATH, todir, p.file);
    if (GetFileAttributesA(srcp) != INVALID_FILE_ATTRIBUTES)
        MoveFileExA(srcp, dstp, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);

    char hash[MAX_PATH];
    _snprintf(hash, MAX_PATH, "%s\\ufo_mods.hash", g_game);
    DeleteFileA(hash);
    return 1;
}

static void refresh_list()
{
    g_refreshing = 1;
    SendMessage(g_list, LVM_DELETEALLITEMS, 0, 0);
    if (!g_game[0]) {
        g_refreshing = 0;
        set_status("Could not find UFO.exe. Put this program in the game folder.");
        return;
    }

    char mods[MAX_PATH], plug[MAX_PATH];
    _snprintf(mods, MAX_PATH, "%s\\mods", g_game);
    _snprintf(plug, MAX_PATH, "%s\\plugins", g_game);
    CreateDirectoryA(mods, 0);
    CreateDirectoryA(plug, 0);

    std::vector<Plugin> all;
    collect_lua(mods, 1, &all);
    collect_lua(plug, 0, &all);
    for (size_t a = 0; a < all.size(); a++)
        for (size_t b = a + 1; b < all.size(); b++)
            if (_stricmp(all[a].title.c_str(), all[b].title.c_str()) > 0) {
                Plugin t = all[a]; all[a] = all[b]; all[b] = t;
            }

    int on = 0;
    for (size_t i = 0; i < all.size(); i++) {
        LVITEMA it;
        memset(&it, 0, sizeof(it));
        it.mask = LVIF_TEXT | LVIF_PARAM;
        it.iItem = (int)i;
        it.pszText = (LPSTR)all[i].title.c_str();
        it.lParam = (LPARAM)i;
        int row = (int)SendMessageA(g_list, LVM_INSERTITEMA, 0, (LPARAM)&it);

        it.mask = LVIF_TEXT;
        it.iSubItem = 1;
        it.pszText = (LPSTR)all[i].file.c_str();
        SendMessageA(g_list, LVM_SETITEMA, 0, (LPARAM)&it);

        char pbuf[32];
        if (all[i].has_param) {
            if (all[i].param) _snprintf(pbuf, 32, "%d", all[i].param);
            else strcpy(pbuf, "default");
        } else {
            strcpy(pbuf, "-");
        }
        it.iSubItem = 2;
        it.pszText = pbuf;
        SendMessageA(g_list, LVM_SETITEMA, 0, (LPARAM)&it);

        Plugin *heap = new Plugin(all[i]);
        LVITEMA lp;
        memset(&lp, 0, sizeof(lp));
        lp.mask = LVIF_PARAM;
        lp.iItem = row;
        lp.lParam = (LPARAM)heap;
        SendMessageA(g_list, LVM_SETITEMA, 0, (LPARAM)&lp);
        ListView_SetCheckState(g_list, row, all[i].enabled);
        if (all[i].enabled) on++;
    }
    g_refreshing = 0;

    char st[256];
    _snprintf(st, 256, "%s  —  %d Enabled, %d Parked in plugins\\",
        g_game, on, (int)all.size() - on);
    set_status(st);
}

static Plugin *plugin_at(int row)
{
    LVITEMA it;
    memset(&it, 0, sizeof(it));
    it.mask = LVIF_PARAM;
    it.iItem = row;
    SendMessageA(g_list, LVM_GETITEMA, 0, (LPARAM)&it);
    return (Plugin *)it.lParam;
}

static void show_param_for(Plugin *p)
{
    if (!g_paramEdit) return;
    g_paramLoading = 1;
    if (!p || !p->has_param) {
        SetWindowTextA(g_paramEdit, "");
        EnableWindow(g_paramEdit, FALSE);
        SetWindowTextA(g_paramHint, "");
    } else {
        EnableWindow(g_paramEdit, TRUE);
        if (p->param) {
            char b[32];
            _snprintf(b, 32, "%d", p->param);
            SetWindowTextA(g_paramEdit, b);
        } else if (p->param_default) {
            char b[32];
            _snprintf(b, 32, "%d", p->param_default);
            SetWindowTextA(g_paramEdit, b);
        } else {
            SetWindowTextA(g_paramEdit, "");
        }
        if (p->param_default) {
            char h[64];
            _snprintf(h, 64, "Default: %d", p->param_default);
            SetWindowTextA(g_paramHint, h);
        } else {
            SetWindowTextA(g_paramHint, "");
        }
    }
    g_paramLoading = 0;
}

static void save_selected_param()
{
    if (g_paramLoading || !g_game[0] || !g_paramEdit) return;
    int row = ListView_GetNextItem(g_list, -1, LVNI_SELECTED);
    if (row < 0) return;
    Plugin *p = plugin_at(row);
    if (!p || !p->has_param) return;
    char buf[32];
    GetWindowTextA(g_paramEdit, buf, 32);
    int v = atoi(buf);
    if (v < 0) v = 0;
    const char *dir = p->enabled ? "mods" : "plugins";
    char folder[MAX_PATH];
    _snprintf(folder, MAX_PATH, "%s\\%s", g_game, dir);
    write_param(folder, p->file, v);
    p->param = v;
    char pbuf[32];
    if (v) _snprintf(pbuf, 32, "%d", v);
    else strcpy(pbuf, "default");
    LVITEMA it;
    memset(&it, 0, sizeof(it));
    it.mask = LVIF_TEXT;
    it.iItem = row;
    it.iSubItem = 2;
    it.pszText = pbuf;
    SendMessageA(g_list, LVM_SETITEMA, 0, (LPARAM)&it);
    char hash[MAX_PATH];
    _snprintf(hash, MAX_PATH, "%s\\ufo_mods.hash", g_game);
    DeleteFileA(hash);
    set_status("Parameter saved. Restart the game to apply.");
}

static void free_items()
{
    int n = (int)SendMessage(g_list, LVM_GETITEMCOUNT, 0, 0);
    for (int i = 0; i < n; i++) {
        LVITEMA it;
        memset(&it, 0, sizeof(it));
        it.mask = LVIF_PARAM;
        it.iItem = i;
        SendMessageA(g_list, LVM_GETITEMA, 0, (LPARAM)&it);
        delete (Plugin *)it.lParam;
    }
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE: {
        INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_LISTVIEW_CLASSES };
        InitCommonControlsEx(&icc);

        g_main = hwnd;
        HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        g_hint = CreateWindowA("STATIC",
            "Tick a plugin to enable it (moves into mods\\). Untick to park it in plugins\\, which the game does not read. Restart the game after changes.",
            WS_CHILD | WS_VISIBLE, 12, 10, 1040, 36, hwnd, 0, 0, 0);
        SendMessage(g_hint, WM_SETFONT, (WPARAM)font, TRUE);

        g_list = CreateWindowA(WC_LISTVIEWA, "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SINGLESEL,
            12, 50, 500, 288, hwnd, (HMENU)100, 0, 0);
        ListView_SetExtendedListViewStyle(g_list, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

        LVCOLUMNA col;
        memset(&col, 0, sizeof(col));
        col.mask = LVCF_TEXT | LVCF_WIDTH;
        col.pszText = (LPSTR)"Plugin";
        col.cx = 220;
        SendMessageA(g_list, LVM_INSERTCOLUMNA, 0, (LPARAM)&col);
        col.pszText = (LPSTR)"File";
        col.cx = 160;
        SendMessageA(g_list, LVM_INSERTCOLUMNA, 1, (LPARAM)&col);
        col.pszText = (LPSTR)"Param";
        col.cx = 70;
        SendMessageA(g_list, LVM_INSERTCOLUMNA, 2, (LPARAM)&col);

        HWND hdr = CreateWindowA("STATIC", "Display Fix Settings",
            WS_CHILD | WS_VISIBLE, 530, 50, 430, 22, hwnd, 0, 0, 0);
        HWND lmode = CreateWindowA("STATIC", "Display Mode",
            WS_CHILD | WS_VISIBLE, 530, 80, 430, 18, hwnd, 0, 0, 0);
        g_modeCombo = CreateWindowA("COMBOBOX", "",
            WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
            530, 100, 430, 200, hwnd, (HMENU)10, 0, 0);
        SendMessageA(g_modeCombo, CB_ADDSTRING, 0, (LPARAM)"16:9 — widescreen 3D, HUD stretched");
        SendMessageA(g_modeCombo, CB_SETITEMDATA, 0, 169);
        SendMessageA(g_modeCombo, CB_ADDSTRING, 0, (LPARAM)"4:3 — largest 4:3 that fits, HUD unstretched");
        SendMessageA(g_modeCombo, CB_SETITEMDATA, 1, 43);
        g_modeHint = CreateWindowA("STATIC", "",
            WS_CHILD | WS_VISIBLE, 530, 130, 430, 48, hwnd, 0, 0, 0);

        HWND lfps = CreateWindowA("STATIC", "Frame Rate Cap",
            WS_CHILD | WS_VISIBLE, 530, 186, 430, 18, hwnd, 0, 0, 0);
        g_fpsCombo = CreateWindowA("COMBOBOX", "",
            WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
            530, 206, 430, 200, hwnd, (HMENU)11, 0, 0);
        for (int i = 0; i < kFpsN; i++) {
            char t[16];
            _snprintf(t, 16, "%d", kFps[i]);
            SendMessageA(g_fpsCombo, CB_ADDSTRING, 0, (LPARAM)t);
        }

        g_chkBorder = CreateWindowA("BUTTON", "Borderless",
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 530, 250, 300, 22, hwnd, (HMENU)12, 0, 0);
        g_chkCrisp = CreateWindowA("BUTTON", "Crisp UI (sharper text)",
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 530, 276, 300, 22, hwnd, (HMENU)13, 0, 0);
        g_chkLog = CreateWindowA("BUTTON", "Logging",
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 530, 302, 300, 22, hwnd, (HMENU)14, 0, 0);
        g_chkIntro = CreateWindowA("BUTTON", "Skip Intro",
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 530, 328, 200, 22, hwnd, (HMENU)15, 0, 0);
        g_chkDev = CreateWindowA("BUTTON", "Developer Console (Shift+~)",
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 530, 352, 230, 22, hwnd, (HMENU)16, 0, 0);

        g_paramLabel = CreateWindowA("STATIC", "Parameter",
            WS_CHILD | WS_VISIBLE, 12, 348, 70, 20, hwnd, 0, 0, 0);
        g_paramEdit = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_LEFT, 84, 344, 70, 24, hwnd, (HMENU)20, 0, 0);
        g_paramHint = CreateWindowA("STATIC", "",
            WS_CHILD | WS_VISIBLE, 162, 348, 350, 20, hwnd, 0, 0, 0);
        EnableWindow(g_paramEdit, FALSE);

        HWND b1 = CreateWindowA("BUTTON", "Refresh", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            12, 380, 100, 28, hwnd, (HMENU)1, 0, 0);
        HWND b2 = CreateWindowA("BUTTON", "Open Mods Folder", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            122, 380, 140, 28, hwnd, (HMENU)2, 0, 0);
        HWND b3 = CreateWindowA("BUTTON", "Open Plugins Folder", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            272, 380, 160, 28, hwnd, (HMENU)3, 0, 0);
        g_regBtn = CreateWindowA("BUTTON", "Apply Registry Fix",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 12, 414, 500, 26, hwnd, (HMENU)6, 0, 0);
        g_darkChk = CreateWindowA("BUTTON", "Dark Mode",
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 530, 396, 150, 24, hwnd, (HMENU)4, 0, 0);
        HWND launch = CreateWindowA("BUTTON", "Launch Game",
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 780, 378, 200, 44, hwnd, (HMENU)5, 0, 0);

        HWND kids[] = { hdr, lmode, lfps, g_modeCombo, g_fpsCombo, g_modeHint, g_chkBorder, g_chkCrisp, g_chkLog, g_chkIntro, g_chkDev, b1, b2, b3, g_regBtn, g_darkChk, launch, g_list, g_paramLabel, g_paramEdit, g_paramHint };
        for (int i = 0; i < (int)(sizeof(kids) / sizeof(kids[0])); i++)
            SendMessage(kids[i], WM_SETFONT, (WPARAM)font, TRUE);

        g_status = CreateWindowA("STATIC", "Display and plugin changes apply on the next launch.",
            WS_CHILD | WS_VISIBLE, 12, 450, 1040, 22, hwnd, 0, 0, 0);
        SendMessage(g_status, WM_SETFONT, (WPARAM)font, TRUE);

        if (!find_game(g_game, MAX_PATH)) g_game[0] = 0;
        load_dark();
        SendMessage(g_darkChk, BM_SETCHECK, g_dark ? BST_CHECKED : BST_UNCHECKED, 0);
        load_launch_flags();
        refresh_list();
        load_display_fields();
        refresh_reg_button();
        apply_theme(hwnd);
        return 0;
    }
    case WM_ERASEBKGND:
        if (g_dark) {
            RECT rc;
            GetClientRect(hwnd, &rc);
            FillRect((HDC)wParam, &rc, g_brDark);
            return 1;
        }
        break;
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLOREDIT: {
        HDC dc = (HDC)wParam;
        if (g_dark) {
            SetTextColor(dc, RGB(240, 240, 240));
            SetBkColor(dc, RGB(32, 32, 32));
            return (LRESULT)g_brDark;
        }
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            free_items();
            refresh_list();
        } else if (LOWORD(wParam) == 4) {
            g_dark = (SendMessage(g_darkChk, BM_GETCHECK, 0, 0) == BST_CHECKED) ? 1 : 0;
            save_dark();
            apply_theme(hwnd);
        } else if (LOWORD(wParam) == 5) {
            launch_game();
        } else if (LOWORD(wParam) == 6) {
            toggle_registry_fix();
        } else if (LOWORD(wParam) == 15 || LOWORD(wParam) == 16) {
            save_launch_flags();
        } else if (LOWORD(wParam) == 2 && g_game[0]) {
            char p[MAX_PATH];
            _snprintf(p, MAX_PATH, "%s\\mods", g_game);
            CreateDirectoryA(p, 0);
            ShellExecuteA(hwnd, "open", p, 0, 0, SW_SHOWNORMAL);
        } else if (LOWORD(wParam) == 3 && g_game[0]) {
            char p[MAX_PATH];
            _snprintf(p, MAX_PATH, "%s\\plugins", g_game);
            CreateDirectoryA(p, 0);
            ShellExecuteA(hwnd, "open", p, 0, 0, SW_SHOWNORMAL);
        } else if (LOWORD(wParam) == 10 || LOWORD(wParam) == 11 || LOWORD(wParam) == 17) {
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                if (LOWORD(wParam) == 10) update_mode_hint();
                save_display_fields();
            }
        } else if (LOWORD(wParam) == 12 || LOWORD(wParam) == 13 || LOWORD(wParam) == 14) {
            save_display_fields();
        } else if (LOWORD(wParam) == 20 && HIWORD(wParam) == EN_KILLFOCUS) {
            save_selected_param();
        }
        return 0;
    case WM_NOTIFY: {
        LPNMHDR nh = (LPNMHDR)lParam;
        if (nh->idFrom == 100 && nh->code == NM_CUSTOMDRAW && g_dark) {
            NMLVCUSTOMDRAW *cd = (NMLVCUSTOMDRAW *)lParam;
            if (cd->nmcd.dwDrawStage == CDDS_PREPAINT) return CDRF_NOTIFYITEMDRAW;
            if (cd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT) {
                cd->clrText = RGB(240, 240, 240);
                cd->clrTextBk = RGB(32, 32, 32);
                return CDRF_NEWFONT;
            }
        }
        if (nh->idFrom == 100 && nh->code == LVN_ITEMCHANGED) {
            NMLISTVIEW *lv = (NMLISTVIEW *)lParam;
            if (!(lv->uChanged & LVIF_STATE)) break;
            if (g_refreshing) break;
            if ((lv->uNewState & LVIS_SELECTED) && !(lv->uOldState & LVIS_SELECTED))
                show_param_for(plugin_at(lv->iItem));
            BOOL was = (lv->uOldState & LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(2);
            BOOL ison = (lv->uNewState & LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(2);
            if (was == ison) break;
            LVITEMA it;
            memset(&it, 0, sizeof(it));
            it.mask = LVIF_PARAM;
            it.iItem = lv->iItem;
            SendMessageA(g_list, LVM_GETITEMA, 0, (LPARAM)&it);
            Plugin *p = (Plugin *)it.lParam;
            if (!p) break;
            if (!set_enabled(*p, ison)) {
                ListView_SetCheckState(g_list, lv->iItem, was);
                set_status("Could not move that plugin.");
            } else {
                p->enabled = ison;
                char st[256];
                _snprintf(st, 256, "%s → %s  (restart the game to apply)",
                    p->title.c_str(), ison ? "mods\\" : "plugins\\");
                set_status(st);
            }
        }
        return 0;
    }
    case WM_DESTROY:
        free_items();
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE, LPSTR cmd, int show)
{
    int regCmd = handle_registry_cmdline(cmd);
    if (regCmd == 1) return 0;
    if (regCmd == 2) return 1;

    g_brDark = CreateSolidBrush(RGB(32, 32, 32));
    g_brList = CreateSolidBrush(RGB(32, 32, 32));
    load_dark();

    WNDCLASSEXA wc;
    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = inst;
    wc.hbrBackground = g_dark ? g_brDark : (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "UfoModMgr";
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hIcon = LoadIconA(inst, MAKEINTRESOURCEA(IDI_APP));
    wc.hIconSm = (HICON)LoadImageA(inst, MAKEINTRESOURCEA(IDI_APP), IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
    if (!wc.hIconSm) wc.hIconSm = wc.hIcon;
    RegisterClassExA(&wc);

    HWND hwnd = CreateWindowA("UfoModMgr", "UFO Aftermath Community Mod Loader",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 1100, 548, 0, 0, inst, 0);
    ShowWindow(hwnd, show);

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
