//
// Created by wesley on 8/9/25.
//

#ifndef INPUTDEVICEREGISTRY_H
#define INPUTDEVICEREGISTRY_H

namespace Raven3DEngineCore::Input {

#define DEVICE_ID RAVEN_U_INT

    enum class DeviceType : RAVEN_U_SHORT {
        KEYBOARD,
        MOUSE,
        GAMEPAD,
    };

    struct DeviceInfo {
        DEVICE_ID id;
        DeviceType type;
        std::string name;
    };

    class InputDeviceRegistry {
        std::vector<DeviceInfo> devices {};
        DEVICE_ID nextID = 0;
    public:
        static InputDeviceRegistry* getRegistry();
        static DeviceInfo* registerDevice(DeviceType deviceType, const std::string &name);
        static void unregisterDevice(DEVICE_ID deviceID);
        static DeviceInfo* findDeviceByID(DEVICE_ID deviceID);
        static DeviceInfo* findFirstDeviceWithName(const std::string &name);
    };

    extern InputDeviceRegistry _inputDeviceRegistry;
}

#endif //INPUTDEVICEREGISTRY_H
