//
// Created by wesley on 8/8/25.
//

#ifndef GAMEPADEVENT_H
#define GAMEPADEVENT_H


namespace Raven3DEngineCore::Events {

    class GamepadEvent : public Event {
    protected:
        unsigned int _id = 0;
        explicit GamepadEvent(const unsigned int id) : _id(id) {}
    public:
        [[nodiscard]] unsigned int getGamepadId() const { return _id; }
    };

    class GamepadConnectedEvent final: public GamepadEvent {
    public:
        explicit GamepadConnectedEvent(const unsigned int id) : GamepadEvent(id) {}

        EVENT_TYPE_GETTERS(GamePadConnected)

        void logEvent() const override {
            RAVEN_LOG_DEBUG("GamepadConnected Event Triggered for gamepad: {}", _id);
        }
    };

    class GamepadDisconnectedEvent final: public GamepadEvent {
    public:
        explicit GamepadDisconnectedEvent(const unsigned int id) : GamepadEvent(id) {}

        EVENT_TYPE_GETTERS(GamePadDisconnected)

        void logEvent() const override {
            RAVEN_LOG_DEBUG("GamepadDisconnected Event Triggered for gamepad: {}", _id);
        }
    };

    class GamepadButtonEvent : public GamepadEvent {
    protected:
        Input::Gamepad::GamepadButtonCode _button;

        explicit GamepadButtonEvent(const Input::Gamepad::GamepadButtonCode button, const unsigned int id) :  GamepadEvent(id), _button(button) {}

    public:
        [[nodiscard]] Input::Gamepad::GamepadButtonCode GetButton() const { return _button; }
    };

    class GamepadButtonPressedEvent final : public GamepadButtonEvent {
    public:
        explicit GamepadButtonPressedEvent(const Input::Gamepad::GamepadButtonCode button, const unsigned int id): GamepadButtonEvent(button, id) {}

        EVENT_TYPE_GETTERS(GamePadButtonPressed);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("GamepadButtonPressed Event triggered with button: {}", Input::Gamepad::getButtonName(_button));
        }
    };

    class GamepadButtonReleasedEvent final : public GamepadButtonEvent {
    public:
        explicit GamepadButtonReleasedEvent(const Input::Gamepad::GamepadButtonCode button, const unsigned int id): GamepadButtonEvent(button, id) {}

        EVENT_TYPE_GETTERS(GamePadButtonReleased);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("GamepadButtonReleased Event triggered with button: {}", Input::Gamepad::getButtonName(_button));
        }
    };

    class GamepadAxisEvent final : public GamepadEvent {
        Input::Gamepad::GamepadAxisCode _axis;
        float _value;
    public:
        explicit GamepadAxisEvent(const Input::Gamepad::GamepadAxisCode axis, const float value, const unsigned int id): GamepadEvent(id), _axis(axis), _value(value) {}

        [[nodiscard]] Input::Gamepad::GamepadAxisCode GetAxis() const { return _axis; }
        [[nodiscard]] float GetValue() const { return _value; }

        EVENT_TYPE_GETTERS(GamePadAxisMoved);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("GamepadAxis Event triggered with axis: {}, value: {}", Input::Gamepad::getAxisName(_axis), _value);
        }
    };

}



#endif //GAMEPADEVENT_H
