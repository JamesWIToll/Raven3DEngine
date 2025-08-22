#include <utility>

//
// Created by wesley on 8/6/25.
//

#ifndef EVENT_H
#define EVENT_H

namespace Raven3DEngineCore::Events {


    enum class EventType {
        None = 0,
        KeyPressed,
        KeyReleased,
        KeyboardConnected,
        KeyboardDisconnected,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,
        MouseConnected,
        MouseDisconnected,
        WindowResized,
        WindowMoved,
        WindowClosed,
        AppUpdate,
        AppPostUpdate,
        AppPreRender,
        AppRender,
        AppPostRender,
        GamePadButtonPressed,
        GamePadButtonReleased,
        GamePadAxisMoved,
        GamePadConnected,
        GamePadDisconnected,
    };

#define EVENT_TYPE_GETTERS(type) static EventType getEventTypeStatic() { return EventType::type; } \
                                 virtual EventType getEventType() const override { return getEventTypeStatic(); }

    class Event {
    public:
        virtual ~Event() = default;

        virtual void logEvent() const = 0;

        [[nodiscard]] virtual EventType getEventType() const = 0;
    };

    class InputDeviceEvent : public Event {
    protected:
        Input::DeviceInfo _deviceInfo;
        explicit InputDeviceEvent(Input::DeviceInfo  deviceInfo) : _deviceInfo(std::move(deviceInfo)) {}
    public:
        [[nodiscard]] Input::DeviceInfo getDeviceInfo() const { return _deviceInfo; }
    };
}

#endif //EVENT_H
