//
// Created by wesley on 8/11/25.
//

#ifndef RAVEN3DENGINECORE_APPEVENT_H
#define RAVEN3DENGINECORE_APPEVENT_H
#include <c++/14/chrono>

#include "Raven3DEngineCore.h"

namespace Raven3DEngineCore::Events {


    class AppEvent : public Event {
    protected:
        RAVEN_FLOAT _deltaTime {};
    public:
        [[nodiscard]] RAVEN_FLOAT getDeltaTime() const { return _deltaTime; }
    };

    class AppUpdateEvent final : public AppEvent {
    public:
        static std::chrono::system_clock::time_point _lastUpdateTime;
        AppUpdateEvent() {
            const auto now = std::chrono::system_clock::now();
            const auto deltaTime = std::chrono::duration_cast<std::chrono::duration<RAVEN_FLOAT> >(now - _lastUpdateTime).count();
            _lastUpdateTime = now;
            _deltaTime = deltaTime;
        }

        EVENT_TYPE_GETTERS(AppUpdate)

        void logEvent() const override {
#ifdef RAVEN_DEBUG_LOG_APP_EVENTS
            RAVEN_LOG_DEBUG("AppUpdate Event triggered with deltaTime: {} ", _deltaTime);
#endif
        }
    };

    class AppPostUpdateEvent final : public AppEvent {
    public:
        static std::chrono::system_clock::time_point _lastPostUpdateTime;
        AppPostUpdateEvent() {
            const auto now = std::chrono::system_clock::now();
            const auto deltaTime = std::chrono::duration_cast<std::chrono::duration<RAVEN_FLOAT> >(now - _lastPostUpdateTime).count();
            _lastPostUpdateTime = now;
            _deltaTime = deltaTime;
        }

        EVENT_TYPE_GETTERS(AppPostUpdate)

        void logEvent() const override {
#ifdef RAVEN_DEBUG_LOG_APP_EVENTS
            RAVEN_LOG_DEBUG("AppPostUpdate Event triggered with deltaTime: {} ", _deltaTime);
#endif
        }
    };

    class AppPreRenderEvent final : public AppEvent {
        Rendering::IRenderer *_renderer;
    public:
        static std::chrono::system_clock::time_point _lastPreRenderTime;

        AppPreRenderEvent(Rendering::IRenderer *renderer) {
            const auto now = std::chrono::system_clock::now();
            const auto deltaTime = std::chrono::duration_cast<std::chrono::duration<RAVEN_FLOAT> >(now - _lastPreRenderTime).count();
            _lastPreRenderTime = now;
            _deltaTime = deltaTime;
            _renderer = renderer;
        }

        [[nodiscard]] Rendering::IRenderer *getRenderer() const { return _renderer; }

        EVENT_TYPE_GETTERS(AppPreRender)

        void logEvent() const override {
#ifdef RAVEN_DEBUG_LOG_APP_EVENTS
            RAVEN_LOG_DEBUG("AppPreRender Event triggered with deltaTime: {} ", _deltaTime);
#endif
        }
    };

    class AppRenderEvent final : public AppEvent {
    public:
        static std::chrono::system_clock::time_point _lastRenderTime;
        AppRenderEvent() {
            const auto now = std::chrono::system_clock::now();
            const auto deltaTime = std::chrono::duration_cast<std::chrono::duration<RAVEN_FLOAT> >(now - _lastRenderTime).count();
            _lastRenderTime = now;
            _deltaTime = deltaTime;
        }

        EVENT_TYPE_GETTERS(AppRender)

        void logEvent() const override {
#ifdef RAVEN_DEBUG_LOG_APP_EVENTS
            RAVEN_LOG_DEBUG("AppRender Event triggered with deltaTime: {} ", _deltaTime);
#endif
        }
    };

    class AppPostRenderEvent final : public AppEvent {
    public:
        static std::chrono::system_clock::time_point _lastPostRenderTime;
        AppPostRenderEvent() {
            const auto now = std::chrono::system_clock::now();
            const auto deltaTime = std::chrono::duration_cast<std::chrono::duration<RAVEN_FLOAT> >(now - _lastPostRenderTime).count();
            _lastPostRenderTime = now;
            _deltaTime = deltaTime;
        }

        EVENT_TYPE_GETTERS(AppPostRender)

        void logEvent() const override {
#ifdef RAVEN_DEBUG_LOG_APP_EVENTS
            RAVEN_LOG_DEBUG("AppPostRender Event triggered with deltaTime: {} ", _deltaTime);
#endif
        }
    };

}
#endif //RAVEN3DENGINECORE_APPEVENT_H