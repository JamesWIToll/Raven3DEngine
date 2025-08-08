//
// Created by wesley on 8/6/25.
//

#ifndef WINDOWEVENT_H
#define WINDOWEVENT_H
#include <Raven3DEngineCore.h>

namespace Raven3DEngineCore::Events {
    class WindowCloseEvent final: public Event {
        EVENT_TYPE_GETTERS(WindowClosed);
    };

    class WindowResizeEvent final: public Event {
        unsigned int _width, _height;
    public:
        WindowResizeEvent(int width, int height) : _width(width), _height(height) {}

        [[nodiscard]] unsigned int getWidth() const { return _width; }
        [[nodiscard]] unsigned int getHeight() const { return _height; }

        EVENT_TYPE_GETTERS(WindowResized);
    };

}

#endif //WINDOWEVENT_H
