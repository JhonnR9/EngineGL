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

    std::function<void(const Event& e)>eventCallback;

    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    Win32Window();

    bool init(const char *title, int width, int height, bool fullscreen) override;

    void update() override;

    bool shouldClose() const override;

    void close() override;

    WindowSize getSize() const override {
        return size;
    }

    void setEventCallback(std::function<void(const Event &e)> eventCallback) override {
        this->eventCallback = eventCallback;
    }

};

#endif // WIN_32_WINDOW_H
