//
// Created by wesley on 8/23/25.
//

#ifndef RAVEN3DENGINECORE_VPEVENT_H
#define RAVEN3DENGINECORE_VPEVENT_H

namespace Raven3DEngineCore::Events {
    class VPEvent : public Event {
    protected:
        RAVEN_U_INT _vpID;
    public:
        explicit VPEvent(const RAVEN_U_INT &viewportID) : _vpID(viewportID) {}

        [[nodiscard]] RAVEN_U_INT GetViewportID() const { return _vpID; }
    };

    class VPTearDownEvent final : public VPEvent {
    public:
        explicit VPTearDownEvent(const RAVEN_U_INT &viewportID) : VPEvent(viewportID) {}

        EVENT_TYPE_GETTERS(ViewportTearDown)

        void logEvent() const override {
            RAVEN_LOG_DEBUG("TearDown Event triggered for viewport: {}", _vpID);
        }
    };

    class VPWindowConnectedEvent final : public VPEvent {
    public:
        explicit VPWindowConnectedEvent(const RAVEN_U_INT &viewportID) : VPEvent(viewportID) {}
        EVENT_TYPE_GETTERS(ViewportWindowConnected)

        void logEvent() const override {
            RAVEN_LOG_DEBUG("Window connected event triggered for viewport: {}", _vpID);
        }
    };

}

#endif //RAVEN3DENGINECORE_VPEVENT_H