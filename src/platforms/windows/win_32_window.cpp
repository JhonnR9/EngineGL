#include "win_32_window.h"
#include <glad.h>
#include <GL/GL.h>
#include <iostream>
#include <thread>
#include <windowsx.h>
#include "wglext.h"


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

    constexpr char class_name[] = "Win32OpenGLWindow";

    const WNDPROC wndProc = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        Win32Window *window = nullptr;

        if (uMsg == WM_NCCREATE) {
            const auto *cs = reinterpret_cast<CREATESTRUCT *>(lParam);
            window = static_cast<Win32Window *>(cs->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        } else {
            if ((window = reinterpret_cast<Win32Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA)))) {
                return window->WindowProc(hwnd, uMsg, wParam, lParam);
            }

        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    };


    WNDCLASS wc = {};
    wc.lpfnWndProc = wndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = class_name;
    wc.cbWndExtra = sizeof(Win32Window *);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClass(&wc)) {
        std::cerr << "Failed to register window class!" << std::endl;
        return false;
    }

    DWORD style;
    constexpr DWORD exStyle = 0;
    RECT rect = {0, 0, width, height};

    if (fullscreen) {
        style = WS_POPUP | WS_VISIBLE;

        const HMONITOR hmon = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
        MONITORINFO mi = {sizeof(mi)};
        if (GetMonitorInfo(hmon, &mi)) {
            rect = mi.rcMonitor;
        }
    } else {
        style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

        AdjustWindowRectEx(&rect, style, FALSE, exStyle);
    }

    hwnd = CreateWindowEx(
        exStyle,
        class_name,
        title,
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

    create_opengl_context();

    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return false;
    }


    typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);

    if (const auto wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"))) {
        wglSwapIntervalEXT(use_vsync);
    }

    running = true;
    return true;
}

void Win32Window::poll_events() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_QUIT) running = false;
    }
}

bool Win32Window::should_close() const {
    return !running;
}

void Win32Window::close() {
    ReleaseDC(hwnd, hdc);
    if (glContext) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(glContext);
        glContext = nullptr;
    }
    if (hwnd) {
        DestroyWindow(hwnd);
        hwnd = nullptr;
    }
    running = false;
}


void Win32Window::swap_buffers() {
    if (hwnd && glContext) {
        SwapBuffers(hdc);
    }
}

bool Win32Window::is_mouse_down(const UINT msg) {
    return msg == WM_LBUTTONDOWN ||
          msg == WM_RBUTTONDOWN ||
          msg == WM_MBUTTONDOWN;
}

MouseButton Win32Window::get_mouse_button( const UINT msg) {
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

void Win32Window::create_opengl_context() {
    hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    const int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);

    const HGLRC dummy = wglCreateContext(hdc);
    wglMakeCurrent(hdc, dummy);

    const auto wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(
        GetAnyGLFuncAddress("wglCreateContextAttribsARB")
    );

    const int contextAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    glContext = wglCreateContextAttribsARB(hdc, nullptr, contextAttribs);
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(dummy);
    wglMakeCurrent(hdc, glContext);

}
