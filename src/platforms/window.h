//
// Created by jhone on 29/03/2026.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include "events/event.h"

struct WindowSize {
    int width;
    int height;
};

class Window {
public:
    virtual ~Window() = default;

    virtual bool init(const char *title, int width, int height, bool fullscreen) = 0;

    virtual void update() = 0;

    virtual bool shouldClose() const = 0;

    virtual void close() = 0;

    virtual WindowSize getSize() const = 0;

    virtual void useVsync(bool useVsync)=0;
    virtual void setEventCallback(std::function<void(const Event& e)>)=0;
};


#endif //WINDOW_H
