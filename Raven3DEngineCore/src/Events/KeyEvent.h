//
// Created by wesley on 8/6/25.
//

#ifndef KEYEVENT_H
#define KEYEVENT_H
#include <Raven3DEngineCore.h>

namespace Raven3DEngineCore::Events {
    class KeyEvent : public Event {
    public:
        [[nodiscard]] Input::Key::KeyCode getKeyCode() const {
            return _keyCode;
        }
    protected:
        explicit KeyEvent(Input::Key::KeyCode code) : _keyCode(code) {}

        Input::Key::KeyCode _keyCode;
    };

    class KeyPressedEvent final : public KeyEvent {
        bool _isRepeat;
    public:
        explicit KeyPressedEvent(const Input::Key::KeyCode code, const bool isRepeat = false) : KeyEvent(code), _isRepeat(isRepeat) {}

        [[nodiscard]] bool isRepeatEvent() const {
            return _isRepeat;
        }

        EVENT_TYPE_GETTERS(KeyPressed);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("KeyPressed Event Triggered - Key: {}, Repeated: {}", Input::Key::getKeyName(_keyCode), _isRepeat ? "yes" : "no");
        }
    };

    class KeyReleasedEvent final : public KeyEvent {
    public:
        explicit KeyReleasedEvent(const Input::Key::KeyCode code) : KeyEvent(code) {}

        EVENT_TYPE_GETTERS(KeyReleased);

        void logEvent() const override {
            RAVEN_LOG_DEBUG("KeyReleased Event Triggered - Key: {}", Input::Key::getKeyName(_keyCode));
        }
    };
}

#endif //KEYEVENT_H
