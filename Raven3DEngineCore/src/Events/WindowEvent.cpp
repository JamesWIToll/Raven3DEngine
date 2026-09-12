#include "Events/WindowEvent.h"
#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Events;

WindowEvent::WindowEvent(Window::IRenderWindow *window) : _window(window) {
}

void WindowCloseEvent::logEvent() const {
    RAVEN_LOG_DEBUG("WindowClose Event Triggered for window: {}", _window->GetName());
}

void WindowResizeEvent::logEvent() const {
    RAVEN_LOG_DEBUG("WindowResizeEvent Event Triggered with new width {} and height {} for window {}", _width, _height, _window->GetName());
}
 
