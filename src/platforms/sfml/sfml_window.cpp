#include "sfml_window.h"

#include <iostream>
#include <SFML/Window/Event.hpp>

bool sfml_window::init(const char *title, int width, int height, bool fullscreen) {
    sf::VideoMode mode(sf::Vector2u(width, height));
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antiAliasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;
    settings.sRgbCapable = false;


    if (fullscreen) {
        mode = sf::VideoMode::getDesktopMode();
        window = std::make_unique<sf::Window>(mode, title, sf::Style::Default, sf::State::Fullscreen, settings);
    } else {
        window = std::make_unique<sf::Window>(mode, title, sf::Style::Resize | sf::Style::Close, sf::State::Windowed, settings);
    }
   if (window) {
       if (!gladLoadGL()) {
           std::cerr << "Failed to initialize GLAD!" << std::endl;
           return false;
       }
   }
    return window != nullptr;
}

void sfml_window::poll_events() {
    while (auto maybeEvent = window->pollEvent()) {
        const sf::Event &event = maybeEvent.value();

        if (event.is<sf::Event::Closed>()) {
            if (eventCallback) eventCallback(ResizeEvent(0, 0));
            window->close();
        }

        // Resize
        if (const auto* resized = event.getIf<sf::Event::Resized>()) {
            if (eventCallback)
                eventCallback(ResizeEvent(resized->size.x, resized->size.y));
        }


        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
            if (eventCallback) {

                int translatedKey = translate_sfml_key(keyPressed->code);

                int mods = 0;
                if (keyPressed->shift)   mods |= SHIFT_FLAG; // SFML 3.0 KeyPressed tem bools de mods!
                if (keyPressed->control) mods |= CTRL_FLAG;
                if (keyPressed->alt)     mods |= ALT_FLAG;

                eventCallback(KeyEvent(translatedKey,
                                       static_cast<int>(keyPressed->scancode),
                                       KeyAction::Press, mods));
            }
        }

        if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>()) {
            if (eventCallback) {
                int translatedKey = translate_sfml_key(keyReleased->code);
                int mods = 0;
                // Repetir a lógica de mods se o seu sistema exigir mods no release
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) mods |= SHIFT_FLAG;

                eventCallback(KeyEvent(translatedKey,
                                       static_cast<int>(keyReleased->scancode),
                                       KeyAction::Release, mods));
            }
        }

        if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
            if (eventCallback)
                eventCallback(MouseMoveEvent(mouseMove->position.x, mouseMove->position.y));
        }

        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            MouseButton btn = MouseButton::Left;
            if (mousePressed->button == sf::Mouse::Button::Right) btn = MouseButton::Right;
            else if (mousePressed->button == sf::Mouse::Button::Middle) btn = MouseButton::Middle;

            if (eventCallback)
                eventCallback(MouseButtonEvent(btn, ButtonAction::Press,
                                               mousePressed->position.x,
                                               mousePressed->position.y));
        }

        if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
            MouseButton btn = MouseButton::Left;
            if (mouseReleased->button == sf::Mouse::Button::Right) btn = MouseButton::Right;
            else if (mouseReleased->button == sf::Mouse::Button::Middle) btn = MouseButton::Middle;

            if (eventCallback)
                eventCallback(MouseButtonEvent(btn, ButtonAction::Release,
                                               mouseReleased->position.x,
                                               mouseReleased->position.y));
        }

        // Mouse: Scroll
        if (const auto* mouseWheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
            if (eventCallback)
                eventCallback(MouseWheelEvent(mouseWheel->delta));
        }
    }
}


int sfml_window::translate_sfml_key(sf::Keyboard::Key key) {
    using sf::Keyboard::Key;

    // Mapeamento das letras (SFML A=0...Z=25 -> ASCII 65...90)
    if (key >= Key::A && key <= Key::Z) {
        return 'A' + (static_cast<int>(key) - static_cast<int>(Key::A));
    }

    // Mapeamento dos números (SFML Num0...Num9 -> ASCII 48...57)
    if (key >= Key::Num0 && key <= Key::Num9) {
        return '0' + (static_cast<int>(key) - static_cast<int>(Key::Num0));
    }

    switch (key) {
        case Key::Up:     return 38; // Key::UP
        case Key::Down:   return 40; // Key::DOWN
        case Key::Left:   return 37; // Key::LEFT
        case Key::Right:  return 39; // Key::RIGHT
        case Key::Space:  return 32;
        case Key::Enter:  return 13;
        case Key::Escape: return 27;
        case Key::LShift:
        case Key::RShift: return 16;
        case Key::LControl:
        case Key::RControl: return 17;
        case Key::LAlt:
        case Key::RAlt:   return 18;
        case Key::Tab:    return 9;
        case Key::Backspace: return 8;
        case Key::Delete: return 46;
        default: return 0;
    }
}

bool sfml_window::should_close() const {
    return !window || !window->isOpen();
}

void sfml_window::close() {
    if (window) {
        window->close();
    }
}

Size sfml_window::get_size() const {
    if (window) {
        const auto size = window->getSize();
        return { static_cast<int>(size.x), static_cast<int>(size.y) };
    }
    return {0, 0};
}

void sfml_window::set_vsync(bool use_vsync) {
    if (window) {
        window->setVerticalSyncEnabled(use_vsync);
    }
}

void sfml_window::set_event_callback(std::function<void(const Event &e)> callback) {
    eventCallback = callback;
}

void sfml_window::swap_buffers() {
    if (window) {
        window->display();
    }
}
