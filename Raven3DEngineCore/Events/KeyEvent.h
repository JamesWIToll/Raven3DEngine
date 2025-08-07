//
// Created by wesley on 8/6/25.
//

#ifndef KEYEVENT_H
#define KEYEVENT_H
#include "Event.h"
#include "../Input/KeyCode.h"

namespace Raven3DEngineCore::Events {
    class KeyEvent : public Event {
    public:
        Input::Key::KeyCode getKeyCode() const {
            return _keyCode;
        }
    protected:
        explicit KeyEvent(Input::Key::KeyCode code) : _keyCode(code) {}

        Input::Key::KeyCode _keyCode;
    };

    class KeyPressedEvent final : public KeyEvent {
    public:
        explicit KeyPressedEvent(const Input::Key::KeyCode code, const bool isRepeat = false) : KeyEvent(code), _isRepeat(isRepeat) {}

        bool isRepeatEvent() const {
            return _isRepeat;
        }

        EVENT_TYPE_GETTERS(KeyPressed);

    private:
        bool _isRepeat;
    };

    class KeyReleasedEvent final : public KeyEvent {
    public:
        explicit KeyReleasedEvent(const Input::Key::KeyCode code) : KeyEvent(code) {}

        EVENT_TYPE_GETTERS(KeyReleased);
    };
}

#endif //KEYEVENT_H
