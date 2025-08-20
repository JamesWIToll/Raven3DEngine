//
// Created by wesley on 8/11/25.
//

#ifndef RAVEN3DENGINECORE_APPEVENT_H
#define RAVEN3DENGINECORE_APPEVENT_H

namespace Raven3DEngineCore::Events {


    class AppEvent : public Event {
    protected:
        RAVEN_FLOAT _deltaTime {};
    public:
        [[nodiscard]] RAVEN_FLOAT getDeltaTime() const { return _deltaTime; }
    };

    class AppUpdateEvent final : public AppEvent {
    public:

        AppUpdateEvent() {
            RAVEN_LOG_FLUSH();
            RAVEN_UPDATE_TIME_TICK();
            _deltaTime = RAVEN_UPDATE_DELTA();
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
        AppPostUpdateEvent() {
            _deltaTime = RAVEN_UPDATE_DELTA();
        }

        EVENT_TYPE_GETTERS(AppPostUpdate)

        void logEvent() const override {
#ifdef RAVEN_DEBUG_LOG_APP_EVENTS
            RAVEN_LOG_DEBUG("AppPostUpdate Event triggered with deltaTime: {} ", _deltaTime);
#endif
        }
    };

    class AppPreRenderEvent final : public AppEvent {
    public:

        AppPreRenderEvent() {
            RAVEN_RENDER_TIME_TICK();
            _deltaTime = RAVEN_RENDER_DELTA();
        }

        EVENT_TYPE_GETTERS(AppPreRender)

        void logEvent() const override {
#ifdef RAVEN_DEBUG_LOG_APP_EVENTS
            RAVEN_LOG_DEBUG("AppPreRender Event triggered with deltaTime: {} ", _deltaTime);
#endif
        }
    };

    class AppRenderEvent final : public AppEvent {
    public:
        AppRenderEvent() {
            _deltaTime = RAVEN_RENDER_DELTA();
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
        AppPostRenderEvent() {
            _deltaTime = RAVEN_RENDER_DELTA();
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