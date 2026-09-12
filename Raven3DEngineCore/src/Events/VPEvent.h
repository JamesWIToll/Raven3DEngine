//
// Created by wesley on 8/23/25.
//

#ifndef RAVEN3DENGINECORE_VPEVENT_H
#define RAVEN3DENGINECORE_VPEVENT_H
#include <RavenForward.h>
#include "Event.h"

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

        void logEvent() const override;
    };

    class VPWindowConnectedEvent final : public VPEvent {
    public:
        explicit VPWindowConnectedEvent(const RAVEN_U_INT &viewportID) : VPEvent(viewportID) {}
        EVENT_TYPE_GETTERS(ViewportWindowConnected)

        void logEvent() const override;
    };

}

#endif //RAVEN3DENGINECORE_VPEVENT_H
