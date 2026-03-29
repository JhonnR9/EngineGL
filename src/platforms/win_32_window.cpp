#include "win_32_window.h"
#include <glad/glad.h>
#include <GL/GL.h>
#include <wglext.h>
#include <iostream>

static void* GetAnyGLFuncAddress(const char* name) {
    void* p = (void*)wglGetProcAddress(name);
    if (!p || p == (void*)0x1 || p == (void*)0x2 || p == (void*)0x3 || p == (void*)-1) {
        HMODULE module = LoadLibraryA("opengl32.dll");
        p = (void*)GetProcAddress(module, name);
    }
    return p;
}

Win32Window::Win32Window() : hwnd(nullptr), glContext(nullptr), size{800, 600}, running(false) {}

bool Win32Window::init(const char* title, int width, int height, bool fullscreen) {
    size.width = width;
    size.height = height;

    constexpr wchar_t CLASS_NAME[] = L"Win32OpenGLWindow";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = StaticWindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = CLASS_NAME;
    wc.cbWndExtra = sizeof(Win32Window*);

    if (!RegisterClassW(&wc)) {
        std::cerr << "Failed to register window class!" << std::endl;
        return false;
    }

    hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        std::wstring(title, title + strlen(title)).c_str(),
        WS_OVERLAPPEDWINDOW | (fullscreen ? WS_POPUP : 0),
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, GetModuleHandle(nullptr), this
    );

    if (!hwnd) {
        std::cerr << "Failed to create window!" << std::endl;
        return false;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    HDC hdc = GetDC(hwnd);

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
        HDC hdc = GetDC(hwnd);
        SwapBuffers(hdc);
        ReleaseDC(hwnd, hdc);
    }
}

bool Win32Window::shouldClose() const {
    return !running;
}

void Win32Window::close() {
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
    Win32Window* window = nullptr;
    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = static_cast<Win32Window*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
    } else {
        window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (window) return window->WindowProc(hwnd, uMsg, wParam, lParam);
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT Win32Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_SIZE:
            size.width = LOWORD(lParam);
            size.height = HIWORD(lParam);
            return 0;
        case WM_CLOSE:
            close();
            return 0;
        case WM_DESTROY:
            running = false;
            return 0;
        case WM_KEYDOWN:
        case WM_KEYUP:
            if (keyCallback) keyCallback(static_cast<int>(wParam), 0, uMsg == WM_KEYDOWN ? 1 : 0, 0);
            return 0;
        default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

    }

}
