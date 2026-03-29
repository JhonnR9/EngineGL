#ifndef WIN_32_WINDOW_H
#define WIN_32_WINDOW_H

#include "window.h"
#include <windows.h>
#include <functional>

class Win32Window : public Window {
    HWND hwnd;
    HGLRC glContext;
    WindowSize size;
    bool running = false;
    std::function<void(int key, int scancode, int action, int mods)> keyCallback;

    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void createGLContext();
    void destroyGLContext();

public:
    Win32Window();

    bool init(const char *title, int width, int height, bool fullscreen) override;

    void update() override;

    bool shouldClose() const override;

    void close() override;

    WindowSize getSize() const override {
        return size;
    }

    void setKeyCallback(std::function<void(int, int, int, int)> callback) override {
        keyCallback = callback;
    }
};

#endif // WIN_32_WINDOW_H
