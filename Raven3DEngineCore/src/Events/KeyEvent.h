//
// Created by wesley on 8/6/25.
//

#ifndef KEYEVENT_H
#define KEYEVENT_H

namespace Raven3DEngineCore::Events {

    class KeyboardConnectedEvent final : public InputDeviceEvent {
    public:
        explicit KeyboardConnectedEvent(const Input::DeviceInfo &device) : InputDeviceEvent(device) {}

        EVENT_TYPE_GETTERS(KeyboardConnected);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("KeyboardConnected Event triggered by Device {} ( {} )", _deviceInfo.id, _deviceInfo.name);
        }
    };

    class KeyboardDisconnectedEvent final : public InputDeviceEvent {
    public:
        explicit KeyboardDisconnectedEvent(const Input::DeviceInfo &device) : InputDeviceEvent(device) {}

        EVENT_TYPE_GETTERS(KeyboardDisconnected);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("KeyboardDisconnected Event triggered by Device {} ( {} )", _deviceInfo.id, _deviceInfo.name);
        }
    };

    class KeyEvent : public InputDeviceEvent {
    public:
        [[nodiscard]] Input::Key::KeyCode getKeyCode() const {
            return _keyCode;
        }
    protected:
        explicit KeyEvent(const Input::Key::KeyCode code, const Input::DeviceInfo &device) : InputDeviceEvent(device), _keyCode(code) {}

        Input::Key::KeyCode _keyCode;
    };

    class KeyPressedEvent final : public KeyEvent {
        bool _isRepeat;
    public:
        explicit KeyPressedEvent(const Input::Key::KeyCode code,  const Input::DeviceInfo &device, const bool isRepeat = false) : KeyEvent(code, device), _isRepeat(isRepeat) {}

        [[nodiscard]] bool isRepeatEvent() const {
            return _isRepeat;
        }

        EVENT_TYPE_GETTERS(KeyPressed);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("KeyPressed Event Triggered - Key: {}, Repeated: {} on Device {} ( {} )", Input::Key::getKeyName(_keyCode), _isRepeat ? "yes" : "no", _deviceInfo.id, _deviceInfo.name);
        }
    };

    class KeyReleasedEvent final : public KeyEvent {
    public:
        explicit KeyReleasedEvent(const Input::Key::KeyCode code, const Input::DeviceInfo &device) : KeyEvent(code, device) {}

        EVENT_TYPE_GETTERS(KeyReleased);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("KeyReleased Event Triggered - Key: {} on Device {} ( {} )", Input::Key::getKeyName(_keyCode), _deviceInfo.id, _deviceInfo.name);
        }
    };
}

#endif //KEYEVENT_H
