
#include "Events/VPEvent.h"
#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Events;

void VPTearDownEvent::logEvent() const {
    RAVEN_LOG_DEBUG("TearDown Event triggered for viewport: {}", _vpID);
}


void VPWindowConnectedEvent::logEvent() const {
    RAVEN_LOG_DEBUG("Window connected event triggered for viewport: {}", _vpID);
}
