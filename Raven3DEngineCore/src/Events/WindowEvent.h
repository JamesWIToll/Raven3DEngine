//
// Created by wesley on 8/6/25.
//

#ifndef WINDOWEVENT_H
#define WINDOWEVENT_H
#include <Raven3DEngineCore.h>

namespace Raven3DEngineCore::Events {
    class WindowCloseEvent final: public Event {
    public:

        EVENT_TYPE_GETTERS(WindowClosed)

        void logEvent() const override {
            RAVEN_LOG_DEBUG("WindowClose Event Triggered");
        }
    };

    class WindowResizeEvent final: public Event {
        unsigned int _width, _height;
    public:
        WindowResizeEvent(const int width, const int height) : _width(width), _height(height) {}

        [[nodiscard]] unsigned int getWidth() const { return _width; }
        [[nodiscard]] unsigned int getHeight() const { return _height; }

        EVENT_TYPE_GETTERS(WindowResized);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("WindowResizeEvent Event Triggered with new width {} and height {}", _width, _height);
        }
    };

}

#endif //WINDOWEVENT_H
