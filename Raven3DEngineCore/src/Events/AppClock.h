//
// Created by wesley on 8/18/2025.
//

#ifndef RAVEN3DENGINECORE_APPCLOCK_H
#define RAVEN3DENGINECORE_APPCLOCK_H

namespace Raven3DEngineCore::Events {
    class AppClock {
        std::chrono::time_point<std::chrono::high_resolution_clock> _lastUpdateTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> _lastRenderTime;

        //delta in seconds
        RAVEN_FLOAT _lastUpdateDelta {0.0f};
        RAVEN_FLOAT _lastRenderDelta {0.0f};
    public:
        AppClock() : _lastUpdateTime(std::chrono::high_resolution_clock::now()),
                     _lastRenderTime(std::chrono::high_resolution_clock::now()) {}

        ~AppClock() = default;

        void OnUpdate() {
            const auto now = std::chrono::high_resolution_clock::now();
            _lastUpdateDelta = std::chrono::duration_cast<std::chrono::duration<float>>(now - _lastUpdateTime).count();
            _lastUpdateTime = now;
        }

        void OnRender() {
            const auto now = std::chrono::high_resolution_clock::now();
            _lastRenderDelta = std::chrono::duration_cast<std::chrono::duration<float>>(now - _lastRenderTime).count();
            _lastRenderTime = now;
        }

        [[nodiscard]] RAVEN_FLOAT getUpdateTimeDelta() const {
            return _lastUpdateDelta;
        }

        [[nodiscard]] RAVEN_FLOAT getRenderTimeDelta() const {
            return _lastRenderDelta;
        }

        std::chrono::time_point<std::chrono::high_resolution_clock> getLastUpdateTime() const {
            return _lastUpdateTime;
        }

        std::chrono::time_point<std::chrono::high_resolution_clock> getLastRenderTime() const {
            return _lastRenderTime;
        }

        static std::chrono::time_point<std::chrono::high_resolution_clock> getNow() {
            return std::chrono::high_resolution_clock::now();
        }
    };

    extern AppClock globalAppClock;
}

#define RAVEN_UPDATE_DELTA()            Raven3DEngineCore::Events::globalAppClock.getUpdateTimeDelta()
#define RAVEN_RENDER_DELTA()            Raven3DEngineCore::Events::globalAppClock.getRenderTimeDelta()
#define RAVEN_UPDATE_TIME_TICK()        Raven3DEngineCore::Events::globalAppClock.OnUpdate()
#define RAVEN_RENDER_TIME_TICK()        Raven3DEngineCore::Events::globalAppClock.OnRender()
#define RAVEN_GET_NOW()                 Raven3DEngineCore::Events::AppClock::getNow()
#define RAVEN_GET_LAST_UPDATE_TIME()    Raven3DEngineCore::Events::globalAppClock.getLastUpdateTime()
#define RAVEN_GET_LAST_RENDER_TIME()    Raven3DEngineCore::Events::globalAppClock.getLastRenderTime()

#endif //RAVEN3DENGINECORE_APPCLOCK_H