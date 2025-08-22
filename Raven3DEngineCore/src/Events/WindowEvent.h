//
// Created by wesley on 8/6/25.
//

#ifndef WINDOWEVENT_H
#define WINDOWEVENT_H
#include <Raven3DEngineCore.h>

namespace Raven3DEngineCore::Events {

    class WindowEvent : public Event {
    protected:
        Window::IRenderWindow *_window = nullptr;
    public:
        explicit WindowEvent(Window::IRenderWindow *window) : _window(window) {}

        Window::IRenderWindow *getWindow() const { return _window; }
    };

    class WindowCloseEvent final: public WindowEvent {
    public:
        WindowCloseEvent(Window::IRenderWindow *window): WindowEvent(window) {}

        EVENT_TYPE_GETTERS(WindowClosed)

        void logEvent() const override {
            RAVEN_LOG_DEBUG("WindowClose Event Triggered for window: {}", _window->GetName());
        }
    };

    class WindowResizeEvent final: public WindowEvent {
        RAVEN_U_INT _width, _height;
    public:
        WindowResizeEvent(const RAVEN_INT width, const RAVEN_INT height, Window::IRenderWindow * window) : WindowEvent(window), _width(width), _height(height) {}

        [[nodiscard]] RAVEN_U_INT getWidth() const { return _width; }
        [[nodiscard]] RAVEN_U_INT getHeight() const { return _height; }

        EVENT_TYPE_GETTERS(WindowResized);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("WindowResizeEvent Event Triggered with new width {} and height {} for window {}", _width, _height, _window->GetName());
        }
    };

}

#endif //WINDOWEVENT_H
