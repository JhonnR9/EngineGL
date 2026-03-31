//
// Created by jhone on 30/03/2026.
//

#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H


enum class MouseButton {
    Left,
    Right,
    Middle
};

enum class ButtonAction {
    Press,
    Release
};

enum  class KeyAction {
    Press,
    Release
};

enum class EventType {
    MouseButton,
    MouseMove,
    MouseWheel,
    Key,
    Mouse,
    Resize
};

struct  Event {

    virtual ~Event() = default;
    virtual EventType get_type() const = 0;
};

struct ResizeEvent final : Event {
    const int new_width, new_height;

    ResizeEvent(const int new_width, const int new_height)
        : new_width(new_width),
          new_height(new_height) {
    }

    EventType get_type() const override {
        return EventType::Resize;
    }
};

struct MouseEvent :  Event {
    EventType get_type() const override {
        return EventType::Mouse;
    }
};

struct MouseButtonEvent final : MouseEvent {
    MouseButtonEvent(MouseButton button, ButtonAction action, int x, int y)
        : button(button), action(action), x(x), y(y) {}

    EventType get_type() const override {
        return EventType::MouseButton;
    }

    const MouseButton button;
    const ButtonAction action;
    const int x;
    const int y;
};


struct MouseMoveEvent final :  MouseEvent {

    MouseMoveEvent(const int x, const int y): x(x), y(y) {
    }

    EventType get_type() const override {
        return EventType::MouseMove;
    }

    const int x;
    const int y;
};

struct MouseWheelEvent final :  MouseEvent {

    explicit MouseWheelEvent(const float delta): delta(delta) {
    }
    EventType get_type() const override {
        return EventType::MouseWheel;
    }

    const float delta;
};

struct KeyEvent final :  Event {

    KeyEvent(const int key, const int scancode, const KeyAction action, const int mods): key(key), scancode(scancode),
        action(action), mods(mods) {
    }
    EventType get_type() const override {
        return EventType::Key;
    }
    const int key;
    const int scancode;
    const KeyAction action;
    const int mods;
};


#endif //INPUT_EVENT_H
