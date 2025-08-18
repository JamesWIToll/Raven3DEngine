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
    };

    extern AppClock globalAppClock;
}

#define RAVEN_UPDATE_DELTA() globalAppClock.getUpdateTimeDelta()
#define RAVEN_RENDER_DELTA() globalAppClock.getRenderTimeDelta()
#define RAVEN_UPDATE_TIME_TICK() globalAppClock.OnUpdate()
#define RAVEN_RENDER_TIME_TICK() globalAppClock.OnRender()

#endif //RAVEN3DENGINECORE_APPCLOCK_H