//
// Created by jhone on 04/04/2026.
//

#ifndef SFML_WINDOW_H
#define SFML_WINDOW_H

#include <memory>.
#include <glad.h>
#include <SFML/Window.hpp>
#include "platforms/window.h"


namespace sf {
    class Window;
}

class sfml_window : public Window {
    std::unique_ptr<sf::Window> window{nullptr};
    enum KeyMods {
        SHIFT_FLAG = 1 << 0, // 0001
        CTRL_FLAG = 1 << 1, // 0010
        ALT_FLAG = 1 << 2 // 0100
    };

    static int translate_sfml_key(sf::Keyboard::Key key);

    std::function<void(const Event &e)> eventCallback;
public:
    bool init(const char *title, int width, int height, bool fullscreen) override;

    void poll_events() override;

    bool should_close() const override;

    void close() override;

    Size get_size() const override;

    void set_vsync(bool use_vsync) override;

    void set_event_callback(std::function<void(const Event &e)>) override;

    void swap_buffers() override;
};


#endif //SFML_WINDOW_H
