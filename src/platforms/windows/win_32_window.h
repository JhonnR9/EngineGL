#ifndef WIN_32_WINDOW_H
#define WIN_32_WINDOW_H

#include "../window.h"
#include <windows.h>
#include <functional>

class Win32Window : public Window {
    bool use_vsync = {false};

public:
    void set_vsync(const bool use_vsync) override {
        this->use_vsync = use_vsync;
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
    Size size;
    bool running = false;

    std::function<void(const Event &e)> eventCallback;

public:
    void swap_buffers() override;

private:
    static bool is_mouse_down(UINT msg);

    static MouseButton get_mouse_button(UINT msg);

    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void create_opengl_context();

public:
    Win32Window();

    bool init(const char *title, int width, int height, bool fullscreen) override;

    void poll_events() override;

    bool should_close() const override;

    void close() override;

    Size get_size() const override {
        return size;
    }

    void set_event_callback(std::function<void(const Event &e)> eventCallback) override {
        this->eventCallback = eventCallback;
    }
};

#endif // WIN_32_WINDOW_H
