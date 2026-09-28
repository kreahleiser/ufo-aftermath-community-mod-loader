#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

#ifndef APIENTRY
#define APIENTRY __stdcall
#endif

#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_SCISSOR_TEST     0x00000C11
#define GL_VIEWPORT         0x0BA2
#define GL_SCISSOR_BOX      0x0C10
#define GL_TEXTURE_2D       0x0DE1
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S   0x2802
#define GL_TEXTURE_WRAP_T   0x2803
#define GL_NEAREST          0x2600
#define GL_LINEAR           0x2601
#define GL_CLAMP_TO_EDGE    0x812F
#define GL_POLYGON_SMOOTH   0x0B41
#define GL_LINE_SMOOTH      0x0B20

#define UFO_DISPLAY_VERSION "1.0.3"

static const int VIRT_W = 1024;
static const int VIRT_H = 768;

static HMODULE g_realGl;
static HMODULE g_self;
static HWND    g_hwnd;
static int     g_mode = 169;      // 169 or 43
static int     g_borderless = 1;
static int     g_frameLimit = 60;
static int     g_crispUi = 1;
static int     g_in2D;
static int     g_vpX, g_vpY, g_vpW, g_vpH;
static int     g_winW, g_winH;
static int     g_inited;
static int     g_logged;
static int     g_cursorLogged;
static int     g_logging;
static int     g_borderlessApplied;
static int     g_vsyncTried;
static HHOOK   g_msgHook;
static FILE*   g_log;
static LARGE_INTEGER g_qpcFreq;
static LARGE_INTEGER g_qpcLast;

static int (WINAPI *orig_GetSystemMetrics)(int);
static BOOL (WINAPI *orig_GetCursorPos)(LPPOINT);
static BOOL (WINAPI *orig_SetCursorPos)(int, int);
static BOOL (WINAPI *orig_GetWindowRect)(HWND, LPRECT);
static LONG (WINAPI *orig_ChangeDisplaySettingsA)(DEVMODEA*, DWORD);
static BOOL (WINAPI *orig_EnumDisplaySettingsA)(LPCSTR, DWORD, DEVMODEA*);
static BOOL (WINAPI *orig_SwapBuffers)(HDC);
static BOOL (WINAPI *orig_AdjustWindowRectEx)(LPRECT, DWORD, BOOL, DWORD);

static void (APIENTRY *orig_glViewport)(int, int, int, int);
static void (APIENTRY *orig_glScissor)(int, int, int, int);
static void (APIENTRY *orig_glClear)(unsigned int);
static void (APIENTRY *orig_glOrtho)(double, double, double, double, double, double);
static void (APIENTRY *orig_glGetIntegerv)(unsigned int, int*);
static void (APIENTRY *orig_glDisable)(unsigned int);
static void (APIENTRY *orig_glEnable)(unsigned int);
static unsigned char (APIENTRY *orig_glIsEnabled)(unsigned int);
static void (APIENTRY *orig_glBindTexture)(unsigned int, unsigned int);
static void (APIENTRY *orig_glTexParameteri)(unsigned int, unsigned int, int);
static BOOL (WINAPI *orig_wglMakeCurrent)(HDC, HGLRC);
static PROC (WINAPI *orig_wglGetProcAddress)(LPCSTR);
static HDC  (WINAPI *orig_wglGetCurrentDC)(void);

static void (APIENTRY *orig_gluPerspective)(double, double, double, double);
static void (APIENTRY *orig_gluOrtho2D)(double, double, double, double);

void InitForwards(HMODULE real);
static void InstallMessageHook(HWND hwnd);

static void Log(const char* fmt, ...)
{
    if (!g_log) return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(g_log, fmt, ap);
    va_end(ap);
    fflush(g_log);
}

static void GetSelfDir(char* buf, size_t n)
{
    GetModuleFileNameA(g_self, buf, (DWORD)n);
    char* slash = strrchr(buf, '\\');
    if (slash) slash[1] = 0;
    else buf[0] = 0;
}

static void WriteDefaultIni(const char* path)
{
    if (GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES) return;
    static const char kIni[] =
        "; UFO Aftermath display helper " UFO_DISPLAY_VERSION "\r\n"
        "; Created automatically on first launch. Delete this file to restore defaults.\r\n"
        "\r\n"
        "[Display]\r\n"
        "; 169 = 16:9 3D + HUD stretched to 16:9, pillarboxed on ultrawide (default)\r\n"
        "; 43  = largest 4:3 that fits (HUD unstretched, still much bigger than 1024x768)\r\n"
        "Mode=169\r\n"
        "\r\n"
        "; 1 = borderless fullscreen on the current monitor\r\n"
        "; 0 = do not resize the window\r\n"
        "Borderless=1\r\n"
        "\r\n"
        "; Cap FPS. Aftermath UI/geoscape break if the GPU runs uncapped (common on AMD).\r\n"
        "; 60 is what the engine was built for. 0 = unlimited.\r\n"
        "FrameLimit=60\r\n"
        "\r\n"
        "; 1 = nearest-neighbour UI textures (kills bitmap-font cell borders when scaled)\r\n"
        "; 0 = leave the game's linear filtering\r\n"
        "CrispUi=1\r\n"
        "\r\n"
        "; 1 = write ufo_display.log next to the DLL (for bug reports)\r\n"
        "Logging=0\r\n";
    HANDLE h = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    DWORD written = 0;
    WriteFile(h, kIni, (DWORD)(sizeof(kIni) - 1), &written, NULL);
    CloseHandle(h);
}

static void LoadIni()
{
    char path[MAX_PATH];
    GetSelfDir(path, MAX_PATH);
    strcat_s(path, "ufo_display.ini");
    WriteDefaultIni(path);
    g_mode = GetPrivateProfileIntA("Display", "Mode", 169, path);
    g_borderless = GetPrivateProfileIntA("Display", "Borderless", 1, path);
    g_logging = GetPrivateProfileIntA("Display", "Logging", 0, path);
    g_frameLimit = GetPrivateProfileIntA("Display", "FrameLimit", 60, path);
    g_crispUi = GetPrivateProfileIntA("Display", "CrispUi", 1, path);
    if (g_mode != 43 && g_mode != 169) g_mode = 169;
    if (g_frameLimit < 0) g_frameLimit = 0;
}

static void OpenLog()
{
    if (!g_logging) return;
    char path[MAX_PATH];
    GetSelfDir(path, MAX_PATH);
    strcat_s(path, "ufo_display.log");
    g_log = fopen(path, "w");
    Log("UFO display helper %s (mode=%d borderless=%d cap=%d)\n",
        UFO_DISPLAY_VERSION, g_mode, g_borderless, g_frameLimit);
}

static float TargetAspect()
{
    return (g_mode == 43) ? (4.0f / 3.0f) : (16.0f / 9.0f);
}

static void ComputeLetterbox(int winW, int winH)
{
    g_winW = winW;
    g_winH = winH;
    float want = TargetAspect();
    float have = (winH > 0) ? (float)winW / (float)winH : want;
    if (have > want) {
        g_vpH = winH;
        g_vpW = (int)(winH * want + 0.5f);
        g_vpX = (winW - g_vpW) / 2;
        g_vpY = 0;
    } else {
        g_vpW = winW;
        g_vpH = (int)(winW / want + 0.5f);
        g_vpX = 0;
        g_vpY = (winH - g_vpH) / 2;
    }
    if (g_vpW < 1) g_vpW = 1;
    if (g_vpH < 1) g_vpH = 1;
}

static void RefreshClientSize()
{
    if (!g_hwnd) return;
    RECT rc;
    if (!GetClientRect(g_hwnd, &rc)) return;
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;
    if (w < 2 || h < 2) return;
    ComputeLetterbox(w, h);
}

static void ApplyBorderless(HDC dc)
{
    HWND hwnd = WindowFromDC(dc);
    if (!hwnd) hwnd = g_hwnd;
    if (!hwnd) return;
    g_hwnd = hwnd;
    InstallMessageHook(hwnd);

    if (!g_borderless) {
        RefreshClientSize();
        return;
    }

    HMONITOR mon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    if (!GetMonitorInfoA(mon, &mi)) return;

    int x = mi.rcMonitor.left;
    int y = mi.rcMonitor.top;
    int w = mi.rcMonitor.right - mi.rcMonitor.left;
    int h = mi.rcMonitor.bottom - mi.rcMonitor.top;

    RECT wr;
    GetWindowRect(hwnd, &wr);
    LONG style = GetWindowLongA(hwnd, GWL_STYLE);
    int already = (wr.left == x && wr.top == y && wr.right == x + w && wr.bottom == y + h
        && (style & WS_POPUP) && !(style & WS_CAPTION));
    if (already && g_borderlessApplied) {
        ComputeLetterbox(w, h);
        return;
    }

    style &= ~(WS_CAPTION | WS_THICKFRAME | WS_BORDER | WS_DLGFRAME | WS_SYSMENU);
    style |= WS_POPUP | WS_VISIBLE;
    LONG ex = GetWindowLongA(hwnd, GWL_EXSTYLE);
    ex &= ~(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME);
    ex |= WS_EX_APPWINDOW;
    SetWindowLongA(hwnd, GWL_STYLE, style);
    SetWindowLongA(hwnd, GWL_EXSTYLE, ex);
    SetWindowPos(hwnd, HWND_TOP, x, y, w, h, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    ComputeLetterbox(w, h);
    g_borderlessApplied = 1;
    Log("borderless %dx%d at %d,%d\n", w, h, x, y);
}

static int IsMouseMessage(UINT msg)
{
    return msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST;
}

static LPARAM ClientToVirtualLParam(LPARAM lp)
{
    if (g_vpW <= 0 || g_vpH <= 0) return lp;
    int x = (short)LOWORD(lp);
    int y = (short)HIWORD(lp);
    x = (int)((double)(x - g_vpX) * (double)VIRT_W / (double)g_vpW + 0.5);
    y = (int)((double)(y - g_vpY) * (double)VIRT_H / (double)g_vpH + 0.5);
    return MAKELPARAM((WORD)x, (WORD)y);
}

static LRESULT CALLBACK GetMsgHook(int code, WPARAM wp, LPARAM lp)
{
    if (code >= 0 && lp) {
        MSG* m = (MSG*)lp;
        if (m->hwnd && m->hwnd == g_hwnd && IsMouseMessage(m->message) && g_vpW > 0) {
            m->lParam = ClientToVirtualLParam(m->lParam);
            POINT p;
            p.x = (short)LOWORD(m->lParam);
            p.y = (short)HIWORD(m->lParam);
            m->pt = p;
        }
    }
    return CallNextHookEx(g_msgHook, code, wp, lp);
}

static void InstallMessageHook(HWND hwnd)
{
    if (g_msgHook || !hwnd) return;
    DWORD tid = GetWindowThreadProcessId(hwnd, NULL);
    g_msgHook = SetWindowsHookExA(WH_GETMESSAGE, GetMsgHook, NULL, tid);
    Log("WH_GETMESSAGE hook=%p tid=%lu\n", g_msgHook, (unsigned long)tid);
}

static int IsFullVirtual(int x, int y, int w, int h)
{
    if (x == 0 && y == 0 && w == VIRT_W && h == VIRT_H) return 1;
    if (x == 0 && y == 0 && g_winW > 0 && w >= g_winW - 1 && h >= g_winH - 1) return 1;
    if (x == 0 && y == 0 && w >= 640 && h >= 480) {
        float a = (float)w / (float)h;
        if (fabs(a - 4.0f / 3.0f) < 0.03f && w >= VIRT_W - 8) return 1;
    }
    return 0;
}

static void MapRect(int x, int y, int w, int h, int* ox, int* oy, int* ow, int* oh)
{
    if (g_vpW <= 0) {
        *ox = x; *oy = y; *ow = w; *oh = h;
        return;
    }
    if (IsFullVirtual(x, y, w, h)) {
        *ox = g_vpX; *oy = g_vpY; *ow = g_vpW; *oh = g_vpH;
        return;
    }
    *ox = g_vpX + (int)((long long)x * g_vpW / VIRT_W);
    *oy = g_vpY + (int)((long long)y * g_vpH / VIRT_H);
    *ow = (int)((long long)w * g_vpW / VIRT_W);
    *oh = (int)((long long)h * g_vpH / VIRT_H);
    if (*ow < 1) *ow = 1;
    if (*oh < 1) *oh = 1;
}

extern "C" void APIENTRY glViewport(int x, int y, int w, int h)
{
    RefreshClientSize();
    int ox, oy, ow, oh;
    MapRect(x, y, w, h, &ox, &oy, &ow, &oh);
    if (g_logged < 40) {
        Log("glViewport %d,%d %dx%d -> %d,%d %dx%d (fb %dx%d letterbox %d,%d %dx%d)\n",
            x, y, w, h, ox, oy, ow, oh, g_winW, g_winH, g_vpX, g_vpY, g_vpW, g_vpH);
        g_logged++;
    }
    orig_glViewport(ox, oy, ow, oh);
}

extern "C" void APIENTRY glScissor(int x, int y, int w, int h)
{
    int ox, oy, ow, oh;
    MapRect(x, y, w, h, &ox, &oy, &ow, &oh);
    orig_glScissor(ox, oy, ow, oh);
}

extern "C" void APIENTRY glClear(unsigned int mask)
{
    if ((mask & GL_COLOR_BUFFER_BIT) && orig_glViewport && g_winW > 0) {
        unsigned char scissor = 0;
        if (orig_glIsEnabled) scissor = orig_glIsEnabled(GL_SCISSOR_TEST);
        orig_glViewport(0, 0, g_winW, g_winH);
        if (orig_glDisable) orig_glDisable(GL_SCISSOR_TEST);
        orig_glClear(mask);
        if (scissor && orig_glEnable) orig_glEnable(GL_SCISSOR_TEST);
        orig_glViewport(g_vpX, g_vpY, g_vpW, g_vpH);
        return;
    }
    orig_glClear(mask);
}

static void ApplyCrispUi(void)
{
    if (!g_crispUi || !g_in2D || !orig_glTexParameteri) return;
    orig_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    orig_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    orig_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    orig_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if (orig_glDisable) {
        orig_glDisable(GL_POLYGON_SMOOTH);
        orig_glDisable(GL_LINE_SMOOTH);
    }
}

static void SetUiPass(int on)
{
    g_in2D = on ? 1 : 0;
    if (g_in2D) ApplyCrispUi();
}

extern "C" void APIENTRY glOrtho(double l, double r, double b, double t, double zn, double zf)
{
    if (g_logged < 50) {
        Log("glOrtho l=%.3f r=%.3f b=%.3f t=%.3f\n", l, r, b, t);
        g_logged++;
    }
    orig_glOrtho(l, r, b, t, zn, zf);
    SetUiPass(1);
}

extern "C" void APIENTRY glBindTexture(unsigned int target, unsigned int texture)
{
    orig_glBindTexture(target, texture);
    if (target == GL_TEXTURE_2D) ApplyCrispUi();
}

extern "C" void APIENTRY glTexParameteri(unsigned int target, unsigned int pname, int param)
{
    if (g_crispUi && g_in2D && target == GL_TEXTURE_2D) {
        if (pname == GL_TEXTURE_MAG_FILTER || pname == GL_TEXTURE_MIN_FILTER)
            param = GL_NEAREST;
        else if (pname == GL_TEXTURE_WRAP_S || pname == GL_TEXTURE_WRAP_T)
            param = GL_CLAMP_TO_EDGE;
    }
    orig_glTexParameteri(target, pname, param);
}

extern "C" void APIENTRY glEnable(unsigned int cap)
{
    if (g_crispUi && g_in2D && (cap == GL_POLYGON_SMOOTH || cap == GL_LINE_SMOOTH))
        return;
    orig_glEnable(cap);
}

extern "C" void APIENTRY glDisable(unsigned int cap)
{
    orig_glDisable(cap);
}

static void RealToVirtualRect(int rx, int ry, int rw, int rh, int* vx, int* vy, int* vw, int* vh)
{
    if (g_vpW <= 0 || g_vpH <= 0) {
        *vx = rx; *vy = ry; *vw = rw; *vh = rh;
        return;
    }
    *vx = (int)((double)(rx - g_vpX) * (double)VIRT_W / (double)g_vpW + 0.5);
    *vy = (int)((double)(ry - g_vpY) * (double)VIRT_H / (double)g_vpH + 0.5);
    *vw = (int)((double)rw * (double)VIRT_W / (double)g_vpW + 0.5);
    *vh = (int)((double)rh * (double)VIRT_H / (double)g_vpH + 0.5);
    if (*vw < 1) *vw = 1;
    if (*vh < 1) *vh = 1;
}

extern "C" void APIENTRY glGetIntegerv(unsigned int pname, int* params)
{
    orig_glGetIntegerv(pname, params);
    if (!params) return;
    if (pname == GL_VIEWPORT || pname == GL_SCISSOR_BOX) {
        int vx, vy, vw, vh;
        RealToVirtualRect(params[0], params[1], params[2], params[3], &vx, &vy, &vw, &vh);
        params[0] = vx;
        params[1] = vy;
        params[2] = vw;
        params[3] = vh;
    }
}

extern "C" BOOL WINAPI wglMakeCurrent(HDC dc, HGLRC rc)
{
    BOOL ok = orig_wglMakeCurrent(dc, rc);
    if (ok && dc) {
        ApplyBorderless(dc);
        RefreshClientSize();
    }
    return ok;
}

extern "C" PROC WINAPI wglGetProcAddress(LPCSTR name)
{
    if (name) {
        if (!strcmp(name, "glViewport")) return (PROC)glViewport;
        if (!strcmp(name, "glScissor")) return (PROC)glScissor;
        if (!strcmp(name, "glClear")) return (PROC)glClear;
        if (!strcmp(name, "glOrtho")) return (PROC)glOrtho;
        if (!strcmp(name, "glGetIntegerv")) return (PROC)glGetIntegerv;
        if (!strcmp(name, "glBindTexture")) return (PROC)glBindTexture;
        if (!strcmp(name, "glTexParameteri")) return (PROC)glTexParameteri;
        if (!strcmp(name, "glEnable")) return (PROC)glEnable;
        if (!strcmp(name, "glDisable")) return (PROC)glDisable;
    }
    return orig_wglGetProcAddress(name);
}

static void APIENTRY hook_gluPerspective(double fovy, double aspect, double zn, double zf)
{
    /* Use the active viewport's aspect. The main world view is 16:9; the
       tactical face portraits are tiny 4:3-ish viewports and look crushed
       if they inherit the widescreen frustum. */
    double a = aspect;
    if (orig_glGetIntegerv) {
        int vp[4] = {0, 0, 0, 0};
        orig_glGetIntegerv(GL_VIEWPORT, vp);
        if (vp[3] > 0) a = (double)vp[2] / (double)vp[3];
    }
    if (g_logged < 80) {
        Log("gluPerspective fovy=%.4f aspect=%.4f -> %.4f\n", fovy, aspect, a);
        g_logged++;
    }
    orig_gluPerspective(fovy, a, zn, zf);
    SetUiPass(0);
}

static void APIENTRY hook_gluOrtho2D(double l, double r, double b, double t)
{
    if (g_logged < 70) {
        Log("gluOrtho2D l=%.3f r=%.3f b=%.3f t=%.3f\n", l, r, b, t);
        g_logged++;
    }
    orig_gluOrtho2D(l, r, b, t);
    SetUiPass(1);
}

static int WINAPI hook_GetSystemMetrics(int idx)
{
    switch (idx) {
    case SM_CXSCREEN:
    case SM_CXFULLSCREEN:
    case SM_CXVIRTUALSCREEN:
        return VIRT_W;
    case SM_CYSCREEN:
    case SM_CYFULLSCREEN:
    case SM_CYVIRTUALSCREEN:
        return VIRT_H;
    default:
        return orig_GetSystemMetrics(idx);
    }
}

static BOOL WINAPI hook_GetCursorPos(LPPOINT lp)
{
    if (!lp || !g_hwnd || g_vpW <= 0) return orig_GetCursorPos(lp);
    POINT p;
    if (!orig_GetCursorPos(&p)) return FALSE;
    ScreenToClient(g_hwnd, &p);
    double x = (double)(p.x - g_vpX) * VIRT_W / (double)g_vpW;
    double y = (double)(p.y - g_vpY) * VIRT_H / (double)g_vpH;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > VIRT_W) x = VIRT_W;
    if (y > VIRT_H) y = VIRT_H;
    lp->x = (LONG)x;
    lp->y = (LONG)y;
    if (g_cursorLogged < 8) {
        Log("GetCursorPos client-mapped -> %ld,%ld (letterbox %d,%d %dx%d)\n",
            lp->x, lp->y, g_vpX, g_vpY, g_vpW, g_vpH);
        g_cursorLogged++;
    }
    return TRUE;
}

static BOOL WINAPI hook_SetCursorPos(int x, int y)
{
    if (!g_hwnd || g_vpW <= 0) return orig_SetCursorPos(x, y);
    POINT p;
    p.x = g_vpX + (int)((long long)x * g_vpW / VIRT_W);
    p.y = g_vpY + (int)((long long)y * g_vpH / VIRT_H);
    ClientToScreen(g_hwnd, &p);
    return orig_SetCursorPos(p.x, p.y);
}

static BOOL WINAPI hook_GetWindowRect(HWND hwnd, LPRECT rc)
{
    if (rc && g_hwnd && hwnd == g_hwnd) {
        rc->left = 0;
        rc->top = 0;
        rc->right = VIRT_W;
        rc->bottom = VIRT_H;
        return TRUE;
    }
    return orig_GetWindowRect(hwnd, rc);
}

static LONG WINAPI hook_ChangeDisplaySettingsA(DEVMODEA* dev, DWORD flags)
{
    if (dev && g_logged < 20) {
        Log("ChangeDisplaySettings %ux%u @%u flags=0x%lX -> swallowed\n",
            (unsigned)dev->dmPelsWidth, (unsigned)dev->dmPelsHeight,
            (unsigned)dev->dmDisplayFrequency, (unsigned long)flags);
        g_logged++;
    }
    return DISP_CHANGE_SUCCESSFUL;
}

static BOOL WINAPI hook_EnumDisplaySettingsA(LPCSTR device, DWORD mode, DEVMODEA* dev)
{
    BOOL ok = orig_EnumDisplaySettingsA(device, mode, dev);
    if (ok && dev && mode != ENUM_CURRENT_SETTINGS && mode != ENUM_REGISTRY_SETTINGS) {
        /* keep enumerating so the launcher still has a list; game uses an index */
    }
    if (ok && dev && (mode == ENUM_CURRENT_SETTINGS || mode == ENUM_REGISTRY_SETTINGS)) {
        dev->dmPelsWidth = (DWORD)VIRT_W;
        dev->dmPelsHeight = (DWORD)VIRT_H;
        dev->dmFields |= DM_PELSWIDTH | DM_PELSHEIGHT;
    }
    return ok;
}

static BOOL WINAPI hook_AdjustWindowRectEx(LPRECT rc, DWORD style, BOOL menu, DWORD ex)
{
    if (rc) {
        rc->left = 0;
        rc->top = 0;
        rc->right = VIRT_W;
        rc->bottom = VIRT_H;
        return TRUE;
    }
    return orig_AdjustWindowRectEx(rc, style, menu, ex);
}

static void TryEnableVsync()
{
    if (g_vsyncTried || !orig_wglGetProcAddress) return;
    g_vsyncTried = 1;
    typedef BOOL (WINAPI *PFN)(int);
    PFN swapint = (PFN)orig_wglGetProcAddress("wglSwapIntervalEXT");
    if (swapint) {
        swapint(1);
        Log("wglSwapIntervalEXT(1) ok\n");
    }
}

static void LimitFrames()
{
    if (g_frameLimit <= 0 || g_qpcFreq.QuadPart <= 0) return;
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    if (g_qpcLast.QuadPart != 0) {
        const double target = 1.0 / (double)g_frameLimit;
        for (;;) {
            QueryPerformanceCounter(&now);
            double elapsed = (double)(now.QuadPart - g_qpcLast.QuadPart) / (double)g_qpcFreq.QuadPart;
            double remain = target - elapsed;
            if (remain <= 0.0) break;
            if (remain > 0.002) Sleep(1);
            else Sleep(0);
        }
    }
    QueryPerformanceCounter(&g_qpcLast);
}

static BOOL WINAPI hook_SwapBuffers(HDC dc)
{
    if (dc) ApplyBorderless(dc);
    TryEnableVsync();
    BOOL ok = orig_SwapBuffers(dc);
    LimitFrames();
    return ok;
}

static int IATHook(HMODULE client, const char* dll, const char* func, void* hook, void** orig)
{
    if (!client) return 0;
    unsigned char* base = (unsigned char*)client;
    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)base;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return 0;
    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
    IMAGE_DATA_DIRECTORY dir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (!dir.VirtualAddress) return 0;
    IMAGE_IMPORT_DESCRIPTOR* imp = (IMAGE_IMPORT_DESCRIPTOR*)(base + dir.VirtualAddress);
    for (; imp->Name; imp++) {
        const char* name = (const char*)(base + imp->Name);
        if (_stricmp(name, dll) != 0) continue;
        IMAGE_THUNK_DATA* thunk = (IMAGE_THUNK_DATA*)(base + (imp->OriginalFirstThunk ? imp->OriginalFirstThunk : imp->FirstThunk));
        IMAGE_THUNK_DATA* iat = (IMAGE_THUNK_DATA*)(base + imp->FirstThunk);
        for (; thunk->u1.AddressOfData; thunk++, iat++) {
            if (IMAGE_SNAP_BY_ORDINAL(thunk->u1.Ordinal)) continue;
            IMAGE_IMPORT_BY_NAME* ibn = (IMAGE_IMPORT_BY_NAME*)(base + thunk->u1.AddressOfData);
            if (strcmp((char*)ibn->Name, func) != 0) continue;
            DWORD old;
            if (!VirtualProtect(&iat->u1.Function, sizeof(void*), PAGE_EXECUTE_READWRITE, &old)) return 0;
            if (orig && !*orig) *orig = (void*)(uintptr_t)iat->u1.Function;
            iat->u1.Function = (ULONG_PTR)hook;
            VirtualProtect(&iat->u1.Function, sizeof(void*), old, &old);
            Log("IAT hook %s!%s\n", dll, func);
            return 1;
        }
    }
    Log("IAT hook FAILED %s!%s\n", dll, func);
    return 0;
}

static FARPROC Must(HMODULE m, const char* n)
{
    FARPROC p = GetProcAddress(m, n);
    if (!p) {
        char buf[256];
        wsprintfA(buf, "ufo display helper: missing %s", n);
        MessageBoxA(NULL, buf, "UFO display helper", MB_ICONERROR);
    }
    return p;
}

static void InitHooks()
{
    if (g_inited) return;
    g_inited = 1;
    LoadIni();
    OpenLog();
    QueryPerformanceFrequency(&g_qpcFreq);
    timeBeginPeriod(1);

    wchar_t sys[MAX_PATH];
    GetSystemDirectoryW(sys, MAX_PATH);
    wcscat_s(sys, L"\\opengl32.dll");
    g_realGl = LoadLibraryW(sys);
    if (!g_realGl) {
        MessageBoxA(NULL, "Could not load system opengl32.dll", "UFO display helper", MB_ICONERROR);
        return;
    }
    InitForwards(g_realGl);

    orig_glViewport = (void (APIENTRY*)(int,int,int,int))Must(g_realGl, "glViewport");
    orig_glScissor = (void (APIENTRY*)(int,int,int,int))Must(g_realGl, "glScissor");
    orig_glClear = (void (APIENTRY*)(unsigned int))Must(g_realGl, "glClear");
    orig_glOrtho = (void (APIENTRY*)(double,double,double,double,double,double))Must(g_realGl, "glOrtho");
    orig_glGetIntegerv = (void (APIENTRY*)(unsigned int,int*))Must(g_realGl, "glGetIntegerv");
    orig_glDisable = (void (APIENTRY*)(unsigned int))Must(g_realGl, "glDisable");
    orig_glEnable = (void (APIENTRY*)(unsigned int))Must(g_realGl, "glEnable");
    orig_glIsEnabled = (unsigned char (APIENTRY*)(unsigned int))Must(g_realGl, "glIsEnabled");
    orig_glBindTexture = (void (APIENTRY*)(unsigned int, unsigned int))Must(g_realGl, "glBindTexture");
    orig_glTexParameteri = (void (APIENTRY*)(unsigned int, unsigned int, int))Must(g_realGl, "glTexParameteri");
    orig_wglMakeCurrent = (BOOL (WINAPI*)(HDC,HGLRC))Must(g_realGl, "wglMakeCurrent");
    orig_wglGetProcAddress = (PROC (WINAPI*)(LPCSTR))Must(g_realGl, "wglGetProcAddress");
    orig_wglGetCurrentDC = (HDC (WINAPI*)(void))Must(g_realGl, "wglGetCurrentDC");

    HMODULE glu = LoadLibraryA("glu32.dll");
    if (glu) {
        orig_gluPerspective = (void (APIENTRY*)(double,double,double,double))GetProcAddress(glu, "gluPerspective");
        orig_gluOrtho2D = (void (APIENTRY*)(double,double,double,double))GetProcAddress(glu, "gluOrtho2D");
    }

    HMODULE user = GetModuleHandleA("user32.dll");
    orig_GetSystemMetrics = (int (WINAPI*)(int))GetProcAddress(user, "GetSystemMetrics");
    orig_GetCursorPos = (BOOL (WINAPI*)(LPPOINT))GetProcAddress(user, "GetCursorPos");
    orig_SetCursorPos = (BOOL (WINAPI*)(int,int))GetProcAddress(user, "SetCursorPos");
    orig_GetWindowRect = (BOOL (WINAPI*)(HWND,LPRECT))GetProcAddress(user, "GetWindowRect");
    orig_ChangeDisplaySettingsA = (LONG (WINAPI*)(DEVMODEA*,DWORD))GetProcAddress(user, "ChangeDisplaySettingsA");
    orig_EnumDisplaySettingsA = (BOOL (WINAPI*)(LPCSTR,DWORD,DEVMODEA*))GetProcAddress(user, "EnumDisplaySettingsA");
    orig_AdjustWindowRectEx = (BOOL (WINAPI*)(LPRECT,DWORD,BOOL,DWORD))GetProcAddress(user, "AdjustWindowRectEx");

    HMODULE gdi = GetModuleHandleA("gdi32.dll");
    orig_SwapBuffers = (BOOL (WINAPI*)(HDC))GetProcAddress(gdi, "SwapBuffers");

    HMODULE exe = GetModuleHandleA(NULL);
    IATHook(exe, "USER32.dll", "GetSystemMetrics", hook_GetSystemMetrics, (void**)&orig_GetSystemMetrics);
    IATHook(exe, "USER32.dll", "GetCursorPos", hook_GetCursorPos, (void**)&orig_GetCursorPos);
    IATHook(exe, "USER32.dll", "SetCursorPos", hook_SetCursorPos, (void**)&orig_SetCursorPos);
    IATHook(exe, "USER32.dll", "GetWindowRect", hook_GetWindowRect, (void**)&orig_GetWindowRect);
    IATHook(exe, "USER32.dll", "ChangeDisplaySettingsA", hook_ChangeDisplaySettingsA, (void**)&orig_ChangeDisplaySettingsA);
    IATHook(exe, "USER32.dll", "EnumDisplaySettingsA", hook_EnumDisplaySettingsA, (void**)&orig_EnumDisplaySettingsA);
    IATHook(exe, "USER32.dll", "AdjustWindowRectEx", hook_AdjustWindowRectEx, (void**)&orig_AdjustWindowRectEx);
    IATHook(exe, "GDI32.dll", "SwapBuffers", hook_SwapBuffers, (void**)&orig_SwapBuffers);
    if (orig_gluPerspective)
        IATHook(exe, "GLU32.dll", "gluPerspective", hook_gluPerspective, (void**)&orig_gluPerspective);
    if (orig_gluOrtho2D)
        IATHook(exe, "GLU32.dll", "gluOrtho2D", hook_gluOrtho2D, (void**)&orig_gluOrtho2D);
}

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH) {
        g_self = inst;
        DisableThreadLibraryCalls(inst);
        InitHooks();
    } else if (reason == DLL_PROCESS_DETACH) {
        if (g_msgHook) { UnhookWindowsHookEx(g_msgHook); g_msgHook = NULL; }
        timeEndPeriod(1);
        if (g_log) { fclose(g_log); g_log = NULL; }
    }
    return TRUE;
}
