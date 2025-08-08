//
// Created by wesley on 8/2/25.
//

#include <Raven3DEngineCore.h>

#include "assimp/Logger.hpp"

using namespace Raven3DEngineCore;

RavenApp::RavenApp(const std::string& appName, const int &pixelWidth, const int &pixelHeight) {
    auto loggerListener = new Logging::ConsoleLog();
    RAVEN_REGISTER_LOG_LISTENER(loggerListener);
    RAVEN_LOG_INFO("hello {}", appName);
    RAVEN_LOG_DEBUG("debug {}", appName);
    RAVEN_LOG_WARNING("warn {}, {}", appName, 1);
    RAVEN_LOG_ERROR("error {}, {}", appName, 1);
    RAVEN_LOG_FATAL("fatal {}, {}", appName, 1);

    _eventHandler = new Events::EventHandler();
    _eventHandler->RegisterEventListener(Events::EventType::WindowClosed, [this] (const Events::Event &) { quitApp(); });
    _window = new Window::SDLWindow();
    _window->SetEventHandler(*_eventHandler);
    _window->Initialize(Rendering::RenderAPI::OPENGL, appName, pixelWidth, pixelHeight);
    _renderer = new Rendering::OpenGLRenderer();
    _renderer->Initialize(glm::vec3(0.1f, 0.3f, 0.4f));
}

RavenApp::~RavenApp() {
    delete _renderer;
    delete _window;
}

void RavenApp::run() const {
    while (!_quit) {
        _window->UpdateWindow();
        _renderer->RenderFrame();
        _window->SwapWindow();
    }
}

void RavenApp::quitApp() {
    _quit = true;
}



