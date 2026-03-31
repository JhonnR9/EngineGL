#include "win_32_window.h"
#include <glad/glad.h>
#include <GL/GL.h>
#include <wglext.h>
#include <iostream>
#include <thread>
#include <windowsx.h>


static void *GetAnyGLFuncAddress(const char *name) {
    void *p = (void *) wglGetProcAddress(name);
    if (!p || p == (void *) 0x1 || p == (void *) 0x2 || p == (void *) 0x3 || p == (void *) -1) {
        HMODULE module = LoadLibraryA("opengl32.dll");
        p = (void *) GetProcAddress(module, name);
    }
    return p;
}

Win32Window::Win32Window() : hwnd(nullptr), hdc(nullptr), glContext(nullptr), size{800, 600} {
}

bool Win32Window::init(const char *title, int width, int height, bool fullscreen) {
    size.width = width;
    size.height = height;

    std::string tempTitle(title);
    std::wstring wideTitle(tempTitle.begin(), tempTitle.end());

    constexpr wchar_t CLASS_NAME[] = L"Win32OpenGLWindow";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = StaticWindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = CLASS_NAME;
    wc.cbWndExtra = sizeof(Win32Window *);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClassW(&wc)) {
        std::cerr << "Failed to register window class!" << std::endl;
        return false;
    }

    DWORD style;
    DWORD exStyle = 0;
    RECT rect = {0, 0, width, height};

    if (fullscreen) {
        style = WS_POPUP | WS_VISIBLE;

        HMONITOR hmon = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
        MONITORINFO mi = {sizeof(mi)};
        if (GetMonitorInfo(hmon, &mi)) {
            rect = mi.rcMonitor;
        }
    } else {
        style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

        AdjustWindowRectEx(&rect, style, FALSE, exStyle);
    }


    hwnd = CreateWindowExW(
        exStyle,
        CLASS_NAME,
        wideTitle.c_str(),
        style,
        (fullscreen ? rect.left : CW_USEDEFAULT),
        (fullscreen ? rect.top : CW_USEDEFAULT),
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr, nullptr, GetModuleHandle(nullptr), this
    );

    if (!hwnd) {
        std::cerr << "Failed to create window!" << std::endl;
        return false;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);

    HGLRC dummy = wglCreateContext(hdc);
    wglMakeCurrent(hdc, dummy);

    auto wglCreateContextAttribsARB =
            static_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(GetAnyGLFuncAddress("wglCreateContextAttribsARB"));

    const int contextAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    glContext = wglCreateContextAttribsARB(hdc, 0, contextAttribs);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(dummy);
    wglMakeCurrent(hdc, glContext);

    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return false;
    }


    typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
            reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));

    if (wglSwapIntervalEXT) {
        wglSwapIntervalEXT(use_vsync);
    }

    running = true;
    return true;
}

void Win32Window::update() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_QUIT) running = false;
    }


    if (hwnd && glContext) {
        SwapBuffers(hdc);
    }
}

bool Win32Window::shouldClose() const {
    return !running;
}

void Win32Window::close() {
    ReleaseDC(hwnd, hdc);
    if (glContext) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(glContext);
        glContext = nullptr;
    }
    if (hwnd) {
        DestroyWindow(hwnd);
        hwnd = nullptr;
    }
    running = false;
}

LRESULT CALLBACK Win32Window::StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Win32Window *window = nullptr;
    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT *cs = reinterpret_cast<CREATESTRUCT *>(lParam);
        window = static_cast<Win32Window *>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<Win32Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (window) return window->WindowProc(hwnd, uMsg, wParam, lParam);

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static bool is_mouse_down(UINT msg) {
    return msg == WM_LBUTTONDOWN ||
           msg == WM_RBUTTONDOWN ||
           msg == WM_MBUTTONDOWN;
}

static MouseButton get_mouse_button(UINT msg) {
    switch (msg) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP: return MouseButton::Left;

        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP: return MouseButton::Right;

        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP: return MouseButton::Middle;

        default: return MouseButton::Left;
    }
}


LRESULT Win32Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_SIZE: {
            size.width = GET_X_LPARAM(lParam);
            size.height = GET_Y_LPARAM(lParam);

            if (this->eventCallback) {
                const ResizeEvent event(size.width, size.height);
                this->eventCallback(event);
            }

            return 0;
        }

        case WM_CLOSE:
            close();
            return 0;
        case WM_DESTROY:
            running = false;
            return 0;
        case WM_KEYDOWN:
        case WM_KEYUP: {
            const int key = static_cast<int>(wParam);
            const int scancode = (lParam >> 16) & 0xFF;

            const KeyAction action = (uMsg == WM_KEYDOWN)? KeyAction::Press: KeyAction::Release;

            int mods = 0;
            if (GetKeyState(VK_SHIFT) & 0x8000) mods |= SHIFT_FLAG;
            if (GetKeyState(VK_CONTROL) & 0x8000) mods |= CTRL_FLAG;
            if (GetKeyState(VK_MENU) & 0x8000) mods |= ALT_FLAG;

            if (this->eventCallback) {
                const KeyEvent event(key, scancode, action, mods);
                this->eventCallback(event);
            }

            return 0;
        }

        case WM_MOUSEMOVE: {
            const int x = GET_X_LPARAM(lParam);
            const int y = GET_Y_LPARAM(lParam);


            if (this->eventCallback) {
                const MouseMoveEvent event(x, y);
                this->eventCallback(event);
            }


            return 0;
        }

        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP: {
            const int x = GET_X_LPARAM(lParam);
            const int y = GET_Y_LPARAM(lParam);

            const ButtonAction action = is_mouse_down(uMsg)? ButtonAction::Press : ButtonAction::Release;
            const MouseButton button = get_mouse_button(uMsg);

            if (this->eventCallback) {
                const MouseButtonEvent event(button, action, x, y);
                this->eventCallback(event);
            }

            return 0;
        }

        case WM_MOUSEWHEEL: {
            int raw_delta = GET_WHEEL_DELTA_WPARAM(wParam);
            float normalized = raw_delta / 120.0f;

            if (this->eventCallback) {
                const MouseWheelEvent event(normalized);
                this->eventCallback(event);
            }

            return 0;
        }
        default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
}
