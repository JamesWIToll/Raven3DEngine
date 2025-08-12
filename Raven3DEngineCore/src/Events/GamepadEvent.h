#include <utility>

//
// Created by wesley on 8/8/25.
//

#ifndef GAMEPADEVENT_H
#define GAMEPADEVENT_H


namespace Raven3DEngineCore::Events {

    class GamepadConnectedEvent final: public InputDeviceEvent {
    public:
        explicit GamepadConnectedEvent(const Input::DeviceInfo &device) : InputDeviceEvent(device) {}

        EVENT_TYPE_GETTERS(GamePadConnected)

        void logEvent() const override {
            RAVEN_LOG_DEBUG("GamepadConnected Event Triggered for Device {} ( {} )", _deviceInfo.id, _deviceInfo.name);
        }
    };

    class GamepadDisconnectedEvent final: public InputDeviceEvent {
    public:
        explicit GamepadDisconnectedEvent(const Input::DeviceInfo &device) : InputDeviceEvent(device) {}

        EVENT_TYPE_GETTERS(GamePadDisconnected)

        void logEvent() const override {
            RAVEN_LOG_DEBUG("GamepadDisconnected Event Triggered for Device {}, ( {} )", _deviceInfo.id, _deviceInfo.name);
        }
    };

    class GamepadButtonEvent : public InputDeviceEvent {
    protected:
        Input::Gamepad::GamepadButtonCode _button;

        explicit GamepadButtonEvent(const Input::Gamepad::GamepadButtonCode button, const Input::DeviceInfo &device) :  InputDeviceEvent(device), _button(button) {}

    public:
        [[nodiscard]] Input::Gamepad::GamepadButtonCode GetButton() const { return _button; }
    };

    class GamepadButtonPressedEvent final : public GamepadButtonEvent {
    public:
        explicit GamepadButtonPressedEvent(const Input::Gamepad::GamepadButtonCode button, const Input::DeviceInfo &device): GamepadButtonEvent(button, device) {}

        EVENT_TYPE_GETTERS(GamePadButtonPressed);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("GamepadButtonPressed Event triggered with button: {} on Device {} ( {} )", Input::Gamepad::getButtonName(_button), _deviceInfo.id, _deviceInfo.name);
        }
    };

    class GamepadButtonReleasedEvent final : public GamepadButtonEvent {
    public:
        explicit GamepadButtonReleasedEvent(const Input::Gamepad::GamepadButtonCode button, const Input::DeviceInfo &device): GamepadButtonEvent(button, device) {}

        EVENT_TYPE_GETTERS(GamePadButtonReleased);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("GamepadButtonReleased Event triggered with button: {} on Device {} ( {} )", Input::Gamepad::getButtonName(_button), _deviceInfo.id, _deviceInfo.name);
        }
    };

    class GamepadAxisEvent final : public InputDeviceEvent {
        Input::Gamepad::GamepadAxisCode _axis;
        RAVEN_FLOAT _value;
    public:
        explicit GamepadAxisEvent(const Input::Gamepad::GamepadAxisCode axis, const RAVEN_FLOAT value, const Input::DeviceInfo &device): InputDeviceEvent(device), _axis(axis), _value(value) {}

        [[nodiscard]] Input::Gamepad::GamepadAxisCode GetAxis() const { return _axis; }
        [[nodiscard]] RAVEN_FLOAT GetValue() const { return _value; }

        EVENT_TYPE_GETTERS(GamePadAxisMoved);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("GamepadAxis Event triggered with axis: {}, value: {} on Device {}, ( {} )", Input::Gamepad::getAxisName(_axis), _value, _deviceInfo.id, _deviceInfo.name);
        }
    };

}



#endif //GAMEPADEVENT_H
