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
    enum KeyMods {
        SHIFT_FLAG = 1 << 0, // 0001
        CTRL_FLAG = 1 << 1, // 0010
        ALT_FLAG = 1 << 2 // 0100
    };

    HWND hwnd;
    HDC hdc;
    HGLRC glContext;
    WindowSize size;
    bool running = false;

    struct Callbacks {
        std::function<void(int key, int scancode, int action, int mods)> keyCallback;
        std::function<void(int width, int height)> resize_callback;
        std::function<void(int x, int y)> mouseMoveCallback;
        std::function<void(int button, int action, int x, int y)> mouseButtonCallback;
        std::function<void(int delta)>mouseWheelCallback;
    };

    Callbacks callbacks{};

    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    Win32Window();

    bool init(const char *title, int width, int height, bool fullscreen) override;

    void update() override;

    bool shouldClose() const override;

    void setResizeCallback(std::function<void(int width, int height)> callback) override {
        this->callbacks.resize_callback = callback;
    }

    void close() override;

    WindowSize getSize() const override {
        return size;
    }

    void setKeyCallback(std::function<void(int, int, int, int)> callback) override {
        this->callbacks.keyCallback = callback;
    }

    void setMouseMoveCallback(std::function<void(int x, int y)> mouseMoveCallback) override {
        this->callbacks.mouseMoveCallback = mouseMoveCallback;
    }

    void setMouseButtonCallback(std::function<void(int button, int action, int x, int y)> mouseButtonCallback) override {
        this->callbacks.mouseButtonCallback = mouseButtonCallback;
    }

    void setMouseWheelCallback(std::function<void(int delta)> mouseWheelCallback) override {
        this->callbacks.mouseWheelCallback = mouseWheelCallback;
    }
};

#endif // WIN_32_WINDOW_H
