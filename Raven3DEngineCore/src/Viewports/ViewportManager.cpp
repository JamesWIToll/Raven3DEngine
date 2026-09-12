
#include "Viewports/ViewportManager.h"
#include <Raven3DEngineCore.h>


using namespace Raven3DEngineCore::Viewports;
using namespace Raven3DEngineCore::Rendering;

RAVEN_U_INT ViewportManager::AddViewport(Viewport vp) {
    const RAVEN_U_INT newID = _nextID++;

    if (vp.renderAPI == Rendering::RenderAPI::OPENGL) {
        vp.renderer = new Rendering::OpenGLRenderer(newID);
    }
    _viewports.emplace(newID, vp);

    if (vp.renderer == nullptr || vp.window == nullptr) {
        RAVEN_LOG_FATAL("Could not setup a renderer or window for VP: {}", newID);
        return newID;
    }
    vp.renderer->SetEventHandler(vp.window->GetEventHandler());
    vp.renderer->Initialize();
    vp.renderer->GetEventHandler()->Notify(Events::VPWindowConnectedEvent{newID});
    return newID;
}

bool ViewportManager::RemoveViewport(const RAVEN_U_INT id) {
    if (!_viewports.contains(id)) {
        return false;
    }
    _eventHandler->Notify(Events::VPTearDownEvent(id));

    delete _viewports[id].renderer;

    bool foundOtherVPWithSameWindow = false;
    for (auto [otherID, otherVP] : _viewports) {
        if (otherID != id && otherVP.window == _viewports[id].window) {
            foundOtherVPWithSameWindow = true;
            break;
        }
    }

    if (!foundOtherVPWithSameWindow) {
        delete _viewports[id].window;
    }

    _viewports.erase(id);
    return true;
}


bool ViewportManager::MoveToNewWindow(const RAVEN_U_INT id, Window::IRenderWindow *window) {
    if (!_viewports.contains(id)) {
        return false;
    }
    const auto vp = GetViewport(id);
    vp->renderer->GetEventHandler()->Notify(Events::VPTearDownEvent{id});
    vp->window = window;
    vp->renderer->GetEventHandler()->Notify(Events::VPWindowConnectedEvent{id});
    return true;
}
