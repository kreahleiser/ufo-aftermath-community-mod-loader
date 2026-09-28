#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0501
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shellapi.h>
#include <stdio.h>
#include <string.h>
#include "gamepath.h"
#include "resource.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

static HWND g_main;
static HWND g_path;
static HWND g_status;
static HWND g_openBtn;
static char g_game[MAX_PATH];
static char g_payload[MAX_PATH];

static void set_status(const char *s)
{
    if (g_status) SetWindowTextA(g_status, s);
}

static void self_dir(char *out, int n)
{
    GetModuleFileNameA(NULL, out, n);
    char *slash = strrchr(out, '\\');
    if (slash) slash[1] = 0;
    else out[0] = 0;
}

static int payload_ready(void)
{
    char p[MAX_PATH];
    _snprintf(p, MAX_PATH, "%sopengl32.dll", g_payload);
    if (GetFileAttributesA(p) == INVALID_FILE_ATTRIBUTES) return 0;
    _snprintf(p, MAX_PATH, "%sufo_mod_manager.exe", g_payload);
    if (GetFileAttributesA(p) == INVALID_FILE_ATTRIBUTES) return 0;
    return 1;
}

static int same_path(const char *a, const char *b)
{
    char aa[MAX_PATH], bb[MAX_PATH];
    if (!GetFullPathNameA(a, MAX_PATH, aa, 0)) strncpy(aa, a, MAX_PATH - 1);
    if (!GetFullPathNameA(b, MAX_PATH, bb, 0)) strncpy(bb, b, MAX_PATH - 1);
    return _stricmp(aa, bb) == 0;
}

static int copy_one(const char *src, const char *dst, int overwrite)
{
    if (same_path(src, dst)) return 1;
    if (!CopyFileA(src, dst, overwrite ? FALSE : TRUE)) {
        DWORD e = GetLastError();
        if (!overwrite && (e == ERROR_FILE_EXISTS || e == ERROR_ALREADY_EXISTS))
            return 1;
        SetLastError(e);
        return 0;
    }
    return 1;
}

static int copy_tree(const char *src, const char *dst, int overwrite)
{
    CreateDirectoryA(dst, 0);
    char spec[MAX_PATH];
    _snprintf(spec, MAX_PATH, "%s\\*", src);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(spec, &fd);
    if (h == INVALID_HANDLE_VALUE) return 1;
    int ok = 1;
    do {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, "..")) continue;
        char from[MAX_PATH], to[MAX_PATH];
        _snprintf(from, MAX_PATH, "%s\\%s", src, fd.cFileName);
        _snprintf(to, MAX_PATH, "%s\\%s", dst, fd.cFileName);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (!copy_tree(from, to, overwrite)) { ok = 0; break; }
        } else if (!copy_one(from, to, overwrite)) {
            ok = 0;
            break;
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return ok;
}

static LONG install_to(const char *game)
{
    if (!has_ufo_exe(game)) return ERROR_PATH_NOT_FOUND;
    char src[MAX_PATH], dst[MAX_PATH];
    _snprintf(src, MAX_PATH, "%sopengl32.dll", g_payload);
    _snprintf(dst, MAX_PATH, "%s\\opengl32.dll", game);
    if (!copy_one(src, dst, 1)) return GetLastError();
    _snprintf(src, MAX_PATH, "%sufo_mod_manager.exe", g_payload);
    _snprintf(dst, MAX_PATH, "%s\\ufo_mod_manager.exe", game);
    if (!copy_one(src, dst, 1)) return GetLastError();

    _snprintf(src, MAX_PATH, "%splugins", g_payload);
    if (GetFileAttributesA(src) == INVALID_FILE_ATTRIBUTES)
        _snprintf(src, MAX_PATH, "%sbundled_plugins", g_payload);
    if (GetFileAttributesA(src) != INVALID_FILE_ATTRIBUTES) {
        _snprintf(dst, MAX_PATH, "%s\\plugins", game);
        if (!copy_tree(src, dst, 0)) return GetLastError();
    }
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

static int elevate_install(const char *game)
{
    char exe[MAX_PATH];
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    char params[MAX_PATH + 40];
    _snprintf(params, sizeof(params), "--install \"%s\"", game);
    SHELLEXECUTEINFOA sei;
    memset(&sei, 0, sizeof(sei));
    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = g_main;
    sei.lpVerb = "runas";
    sei.lpFile = exe;
    sei.lpParameters = params;
    sei.nShow = SW_HIDE;
    if (!ShellExecuteExA(&sei)) {
        DWORD err = GetLastError();
        if (err == ERROR_CANCELLED || err == 1223) return -1;
        return 0;
    }
    if (!sei.hProcess) return 0;
    WaitForSingleObject(sei.hProcess, INFINITE);
    DWORD code = 1;
    GetExitCodeProcess(sei.hProcess, &code);
    CloseHandle(sei.hProcess);
    return (code == 0) ? 1 : 0;
}

static void show_path(void)
{
    if (g_path) SetWindowTextA(g_path, g_game);
}

static void browse_ufo(void)
{
    char file[MAX_PATH];
    file[0] = 0;
    OPENFILENAMEA ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = g_main;
    ofn.lpstrFilter = "UFO.exe\0UFO.exe\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = file;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Select UFO.exe in the game folder";
    ofn.lpstrDefExt = "exe";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_DONTADDTORECENT;
    if (g_game[0]) ofn.lpstrInitialDir = g_game;
    if (!GetOpenFileNameA(&ofn)) return;
    char dir[MAX_PATH];
    if (!game_folder_from_ufo_exe(file, dir, MAX_PATH)) {
        MessageBoxA(g_main, "Pick UFO.exe. That is the Aftermath program file in the game folder.",
            "UFO Aftermath Community Mod Loader", MB_OK | MB_ICONINFORMATION);
        return;
    }
    strncpy(g_game, dir, MAX_PATH - 1);
    g_game[MAX_PATH - 1] = 0;
    show_path();
    set_status("Game folder set from UFO.exe.");
}

static void do_install(void)
{
    if (!payload_ready()) {
        set_status("Keep this installer next to opengl32.dll from the zip.");
        MessageBoxA(g_main,
            "This setup program needs to sit next to opengl32.dll and ufo_mod_manager.exe from the download.",
            "UFO Aftermath Community Mod Loader", MB_OK | MB_ICONWARNING);
        return;
    }
    if (!g_game[0] || !has_ufo_exe(g_game)) {
        set_status("Select UFO.exe in the Aftermath game folder.");
        return;
    }

    LONG e;
    if (!process_is_elevated()) {
        e = install_to(g_game);
        if (e == ERROR_ACCESS_DENIED) {
            int r = elevate_install(g_game);
            if (r < 0) {
                set_status("Install cancelled.");
                return;
            }
            if (!r) {
                MessageBoxA(g_main,
                    "Windows needs administrator permission to copy files into that folder.",
                    "UFO Aftermath Community Mod Loader", MB_OK | MB_ICONWARNING);
                set_status("Could not copy files. Administrator permission is required.");
                return;
            }
            e = ERROR_SUCCESS;
        }
    } else {
        e = install_to(g_game);
    }

    if (e != ERROR_SUCCESS) {
        char msg[256];
        _snprintf(msg, 256, "Could not copy files (error %lu).", (unsigned long)e);
        set_status(msg);
        return;
    }
    set_status("Installed. Open the Mod Manager, then start the game from Steam.");
    if (g_openBtn) EnableWindow(g_openBtn, TRUE);
}

static void open_manager(void)
{
    char mgr[MAX_PATH];
    _snprintf(mgr, MAX_PATH, "%s\\ufo_mod_manager.exe", g_game);
    if (GetFileAttributesA(mgr) == INVALID_FILE_ATTRIBUTES) {
        set_status("Mod Manager is not in that folder yet. Install first.");
        return;
    }
    ShellExecuteA(g_main, "open", mgr, 0, g_game, SW_SHOWNORMAL);
}

static int handle_cmdline(const char *cmd)
{
    if (!cmd) return 0;
    while (*cmd == ' ') cmd++;
    if (_strnicmp(cmd, "--install", 9) != 0) return 0;
    cmd += 9;
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
    self_dir(g_payload, MAX_PATH);
    if (!payload_ready() || !has_ufo_exe(path)) return 2;
    LONG e = install_to(path);
    return (e == ERROR_SUCCESS) ? 1 : 2;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE: {
        g_main = hwnd;
        HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        HWND t1 = CreateWindowA("STATIC",
            "This copies the display fix and mod manager next to UFO.exe. Steam libraries are found on disk. If the folder is wrong, browse and pick UFO.exe.",
            WS_CHILD | WS_VISIBLE, 16, 12, 600, 48, hwnd, 0, 0, 0);
        HWND t2 = CreateWindowA("STATIC", "Game folder",
            WS_CHILD | WS_VISIBLE, 16, 68, 200, 18, hwnd, 0, 0, 0);
        g_path = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_READONLY,
            16, 88, 470, 24, hwnd, (HMENU)10, 0, 0);
        HWND browse = CreateWindowA("BUTTON", "Browse...",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 494, 86, 120, 28, hwnd, (HMENU)2, 0, 0);
        HWND inst = CreateWindowA("BUTTON", "Install",
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 16, 132, 140, 36, hwnd, (HMENU)1, 0, 0);
        g_openBtn = CreateWindowA("BUTTON", "Open Mod Manager",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 166, 132, 160, 36, hwnd, (HMENU)4, 0, 0);
        HWND close = CreateWindowA("BUTTON", "Close",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 494, 132, 120, 36, hwnd, (HMENU)3, 0, 0);
        EnableWindow(g_openBtn, FALSE);
        g_status = CreateWindowA("STATIC", "",
            WS_CHILD | WS_VISIBLE, 16, 180, 600, 40, hwnd, 0, 0, 0);
        HWND kids[] = { t1, t2, g_path, browse, inst, g_openBtn, close, g_status };
        for (int i = 0; i < (int)(sizeof(kids) / sizeof(kids[0])); i++)
            SendMessage(kids[i], WM_SETFONT, (WPARAM)font, TRUE);

        if (!payload_ready())
            set_status("Keep this installer next to opengl32.dll and ufo_mod_manager.exe from the zip.");
        else if (g_game[0])
            set_status("Aftermath found on a Steam library folder. Install, or browse to pick a different UFO.exe.");
        else
            set_status("No Steam copy found. Browse and pick UFO.exe in the game folder.");
        show_path();
        if (g_game[0]) {
            char mgr[MAX_PATH];
            _snprintf(mgr, MAX_PATH, "%s\\ufo_mod_manager.exe", g_game);
            if (GetFileAttributesA(mgr) != INVALID_FILE_ATTRIBUTES)
                EnableWindow(g_openBtn, TRUE);
        }
        return 0;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) do_install();
        else if (LOWORD(wParam) == 2) browse_ufo();
        else if (LOWORD(wParam) == 3) DestroyWindow(hwnd);
        else if (LOWORD(wParam) == 4) open_manager();
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE, LPSTR cmd, int show)
{
    self_dir(g_payload, MAX_PATH);
    int handled = handle_cmdline(cmd);
    if (handled == 1) return 0;
    if (handled == 2) return 1;

    g_game[0] = 0;
    find_steam_ufo(g_game, MAX_PATH);

    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_WIN95_CLASSES };
    InitCommonControlsEx(&icc);

    WNDCLASSEXA wc;
    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = inst;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "UfoModSetup";
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hIcon = LoadIconA(inst, MAKEINTRESOURCEA(IDI_APP));
    wc.hIconSm = (HICON)LoadImageA(inst, MAKEINTRESOURCEA(IDI_APP), IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
    if (!wc.hIconSm) wc.hIconSm = wc.hIcon;
    RegisterClassExA(&wc);

    HWND hwnd = CreateWindowA("UfoModSetup", "UFO Aftermath Community Mod Loader Setup",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 650, 270, 0, 0, inst, 0);
    ShowWindow(hwnd, show);

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
