//
// Created by jhone on 29/03/2026.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <functional>

#include "components/components.h"
#include "events/event.h"


class Window {
public:
    virtual ~Window() = default;

    virtual bool init(const char *title, int width, int height, bool fullscreen) = 0;

    virtual void poll_events() = 0;

    virtual bool should_close() const = 0;

    virtual void close() = 0;

    virtual Size get_size() const = 0;

    virtual void set_vsync(bool use_vsync)=0;
    virtual void set_event_callback(std::function<void(const Event& e)>)=0;
    virtual void swap_buffers()=0;

};


#endif //WINDOW_H
