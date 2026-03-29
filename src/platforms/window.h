//
// Created by jhone on 29/03/2026.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <cstdint>

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

    virtual void setKeyCallback(std::function<void(int key, int scancode, int action, int mods)> callback) = 0;

    virtual void setResizeCallback(std::function<void(int width, int height)> callback) =0;
    virtual void useVsync(bool useVsync)=0;
};


#endif //WINDOW_H
