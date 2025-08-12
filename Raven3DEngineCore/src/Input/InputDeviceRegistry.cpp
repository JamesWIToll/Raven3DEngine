//
// Created by wesley on 8/9/25.
//

#include <Raven3DEngineCore.h>

namespace Raven3DEngineCore::Input {
    InputDeviceRegistry _inputDeviceRegistry {};

    InputDeviceRegistry* InputDeviceRegistry::getRegistry() {
        return &_inputDeviceRegistry;
    }

    DeviceInfo* InputDeviceRegistry::registerDevice(const DeviceType deviceType, const std::string &name) {
        const DEVICE_ID id = _inputDeviceRegistry.nextID++;
        _inputDeviceRegistry.devices.push_back({id, deviceType, name});
        RAVEN_LOG_DEBUG("Device {} ( {} ) registered with Device Registry", id, name);
        return &_inputDeviceRegistry.devices.back();
    }

    void InputDeviceRegistry::unregisterDevice(DEVICE_ID deviceID) {
        bool found = false;
        for (auto i = 0; i < _inputDeviceRegistry.devices.size() && !found; i++) {
            if (_inputDeviceRegistry.devices[i].id == deviceID) {
                RAVEN_LOG_DEBUG("Device {} ( {} ) unregistered from Device Registry", _inputDeviceRegistry.devices[i].id, _inputDeviceRegistry.devices[i].name);
                _inputDeviceRegistry.devices.erase(_inputDeviceRegistry.devices.begin() + i);
                found = true;
            }
        }
        if (!found) {
            RAVEN_LOG_ERROR("DEVICE {} Not Found In Device Registry", deviceID);
        }
    }

    DeviceInfo* InputDeviceRegistry::findDeviceByID(const DEVICE_ID deviceID) {
        for (auto & device : _inputDeviceRegistry.devices) {
            if (device.id == deviceID) {
                return &device;
            }
        }
        RAVEN_LOG_ERROR("DEVICE {} Not Found In Device Registry", deviceID);
        return nullptr;
    }

    DeviceInfo* InputDeviceRegistry::findFirstDeviceWithName(const std::string &name) {
        for (auto & device : _inputDeviceRegistry.devices) {
            if (device.name == name) {
                return &device;
            }
        }
        RAVEN_LOG_ERROR("DEVICE Name ( {} ) Not Found In Device Registry", name);
        return nullptr;
    }

}