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
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,
        WindowResized,
        WindowMoved,
        WindowClosed,
        AppUpdate,
        AppRender,
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
}

#endif //EVENT_H
