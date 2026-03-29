#ifndef WIN_32_WINDOW_H
#define WIN_32_WINDOW_H

#include "window.h"
#include <windows.h>
#include <functional>

class Win32Window : public Window {
    bool use_vsync = {false};

public:
    void useVsync(bool useVsync) override {
        this->use_vsync = useVsync;
    }

private:
    HWND hwnd;
    HDC hdc;
    HGLRC glContext;
    WindowSize size;
    bool running = false;
    std::function<void(int key, int scancode, int action, int mods)> keyCallback;
    std::function<void(int width, int height)> resize_callback;

    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    Win32Window();

    bool init(const char *title, int width, int height, bool fullscreen) override;

    void update() override;

    bool shouldClose() const override;

    void setResizeCallback(std::function<void(int width, int height)> callback) override {
        this->resize_callback = callback;
    }

    void close() override;

    WindowSize getSize() const override {
        return size;
    }

    void setKeyCallback(std::function<void(int, int, int, int)> callback) override {
        keyCallback = callback;
    }
};

#endif // WIN_32_WINDOW_H
