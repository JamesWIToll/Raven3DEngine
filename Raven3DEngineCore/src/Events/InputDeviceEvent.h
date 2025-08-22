//
// Created by wesley on 8/22/2025.
//

#ifndef RAVEN3DENGINECORE_INPUTDEVICEEVENT_H
#define RAVEN3DENGINECORE_INPUTDEVICEEVENT_H

namespace Raven3DEngineCore::Events {
    class InputDeviceEvent : public Event {
    protected:
        Input::DeviceInfo _deviceInfo;
        Window::IRenderWindow *_window = nullptr;
        explicit InputDeviceEvent(Input::DeviceInfo  deviceInfo, Window::IRenderWindow * window) : _deviceInfo(std::move(deviceInfo)), _window(window) {}
    public:
        [[nodiscard]] Input::DeviceInfo GetDeviceInfo() const { return _deviceInfo; }
        [[nodiscard]] Window::IRenderWindow * GetWindow() const { return _window; }
    };
}

#endif //RAVEN3DENGINECORE_INPUTDEVICEEVENT_H