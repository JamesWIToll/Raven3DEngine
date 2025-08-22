//
// Created by wesley on 8/6/25.
//

#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

namespace Raven3DEngineCore::Events {

    class MouseConnectedEvent final : public InputDeviceEvent {
    public:
        explicit MouseConnectedEvent(const Input::DeviceInfo &device, Window::IRenderWindow * window) : InputDeviceEvent(device, window) {}

        EVENT_TYPE_GETTERS(MouseConnected);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("MouseConnected Event triggered by Device {} ( {} )", _deviceInfo.id, _deviceInfo.name);
        }
    };

    class MouseDisconnectedEvent final : public InputDeviceEvent {
    public:
        explicit MouseDisconnectedEvent(const Input::DeviceInfo &device, Window::IRenderWindow * window) : InputDeviceEvent(device, window) {}

        EVENT_TYPE_GETTERS(MouseDisconnected);


        void logEvent() const override {
            RAVEN_LOG_DEBUG("MouseDisconnected Event triggered by Device {} ( {} )", _deviceInfo.id, _deviceInfo.name);
        }
    };

    class MouseButtonEvent : public InputDeviceEvent {
    protected:
        Input::Mouse::MouseCode _button;
        explicit MouseButtonEvent(const Input::Mouse::MouseCode button, const Input::DeviceInfo &device, Window::IRenderWindow * window) : InputDeviceEvent(device, window), _button(button) {}
    public:

        [[nodiscard]] Input::Mouse::MouseCode getButton() const { return _button; }
    };

    class MousePressedEvent final : public MouseButtonEvent {
    public:
        explicit MousePressedEvent(const Input::Mouse::MouseCode button, const Input::DeviceInfo &device, Window::IRenderWindow * window) : MouseButtonEvent(button, device, window) {}
        EVENT_TYPE_GETTERS(MouseButtonPressed)

        void logEvent() const override {
            RAVEN_LOG_DEBUG("MousePressed Event triggered by {} button on Device {} ( {} )", Input::Mouse::getMouseCodeName(_button), _deviceInfo.id, _deviceInfo.name);
        }
    };

    class MouseReleasedEvent final : public MouseButtonEvent {
    public:
        explicit MouseReleasedEvent(const Input::Mouse::MouseCode button, const Input::DeviceInfo &device, Window::IRenderWindow * window) : MouseButtonEvent(button, device, window) {}
        EVENT_TYPE_GETTERS(MouseButtonReleased);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("MouseReleased Event triggered by {} button on Device {} ( {} )", Input::Mouse::getMouseCodeName(_button), _deviceInfo.id, _deviceInfo.name);
        }
    };

    class MouseMovedEvent final : public InputDeviceEvent {
        RAVEN_FLOAT _xPos, _yPos, _xDelta, _yDelta;
    public:
        MouseMovedEvent(const RAVEN_FLOAT x, const RAVEN_FLOAT y, const RAVEN_FLOAT xDelta, const RAVEN_FLOAT yDelta,
            const Input::DeviceInfo &device, Window::IRenderWindow * window): InputDeviceEvent(device, window), _xPos(x), _yPos(y), _xDelta(xDelta), _yDelta(yDelta){}

        [[nodiscard]] RAVEN_FLOAT getX() const { return _xPos; }
        [[nodiscard]] RAVEN_FLOAT getY() const { return _yPos; }
        [[nodiscard]] RAVEN_FLOAT getXDelta() const { return _xDelta; }
        [[nodiscard]] RAVEN_FLOAT getYDelta() const { return _yDelta; }

        EVENT_TYPE_GETTERS(MouseMoved);
        void logEvent() const override {
            RAVEN_LOG_DEBUG("MouseMoved Event triggered - x: {}, y: {}, xDelta: {}, yDelta: {} - on Device {} ( {} )",
                _xPos, _yPos, _xDelta, _yDelta, _deviceInfo.id, _deviceInfo.name);
        }
    };

    class MouseScrolledEvent final : public InputDeviceEvent {
        RAVEN_FLOAT _xOffset, _yOffset;
    public:
        MouseScrolledEvent(const RAVEN_FLOAT x, const RAVEN_FLOAT y, const Input::DeviceInfo &device, Window::IRenderWindow * window): InputDeviceEvent(device, window), _xOffset(x), _yOffset(y) {}

        [[nodiscard]] RAVEN_FLOAT getXOffset() const { return _xOffset; }
        [[nodiscard]] RAVEN_FLOAT getYOffset() const { return _yOffset; }

        EVENT_TYPE_GETTERS(MouseScrolled);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("MouseScrolled Event triggered - x: {}, y: {} on Device {} ( {} )", _xOffset, _yOffset, _deviceInfo.id, _deviceInfo.name);
        }
    };

}

#endif //MOUSEEVENT_H
