//
// Created by wesley on 8/6/25.
//

#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H
#include <Raven3DEngineCore.h>

namespace Raven3DEngineCore::Events {
    class MouseButtonEvent : public Event {
    protected:
        Input::Mouse::MouseCode _button;
        explicit MouseButtonEvent(const Input::Mouse::MouseCode button) : _button(button) {}
    public:

        [[nodiscard]] Input::Mouse::MouseCode getButton() const { return _button; }
    };

    class MousePressedEvent final : public MouseButtonEvent {
    public:
        explicit MousePressedEvent(const Input::Mouse::MouseCode button) : MouseButtonEvent(button) {}
        EVENT_TYPE_GETTERS(MouseButtonPressed);
    };

    class MouseReleasedEvent final : public MouseButtonEvent {
    public:
        explicit MouseReleasedEvent(const Input::Mouse::MouseCode button) : MouseButtonEvent(button) {}
        EVENT_TYPE_GETTERS(MouseButtonReleased);
    };

    class MouseMovedEvent final : public Event {
        float _xPos, _yPos;
    public:
        MouseMovedEvent(const float x, const float y): _xPos(x), _yPos(y) {}

        [[nodiscard]] float getX() const { return _xPos; }
        [[nodiscard]] float getY() const { return _yPos; }

        EVENT_TYPE_GETTERS(MouseMoved);
    };

    class MouseScrolledEvent final : public Event {
        float _xOffset, _yOffset;
    public:
        MouseScrolledEvent(const float x, const float y): _xOffset(x), _yOffset(y) {}

        [[nodiscard]] float getXOffset() const { return _xOffset; }
        [[nodiscard]] float getYOffset() const { return _yOffset; }

        EVENT_TYPE_GETTERS(MouseScrolled);
    };

}

#endif //MOUSEEVENT_H
