//
// Created by wesley on 8/2/25.
//

#include <Raven3DEngineCore.h>


using namespace Raven3DEngineCore;

static const auto loggerListener = new Logging::ConsoleLog();

RavenApp::RavenApp(const std::string& appName, const RAVEN_INT &pixelWidth, const RAVEN_INT &pixelHeight) {

    this->appName = appName;

    RAVEN_REGISTER_LOG_LISTENER(loggerListener);
    RAVEN_LOG_DEBUG("Debug mode active");

    _eventHandler = new Events::EventHandler();
    _eventHandler->RegisterEventListener(Events::EventType::WindowClosed, [this] (const Events::Event &) { quitApp(); });

    _window = new Window::SDLWindow();
    _window->SetEventHandler(_eventHandler);
    _window->Initialize(Rendering::RenderAPI::OPENGL, appName, pixelWidth, pixelHeight);

    _renderer = new Rendering::OpenGLRenderer();
    _renderer->SetEventHandler(_eventHandler);
    _renderer->Initialize(glm::vec3(0.1f, 0.3f, 0.4f));

    _sceneManager = new Scene::SceneManager();
    _sceneManager->SetEventHandler(_eventHandler);
    _sceneManager->Initialize();

    _scriptManager = new Scripting::LuaScriptManager(_sceneManager);
    _scriptManager->SetEventHandler(_eventHandler);
}

RavenApp::~RavenApp() {
    delete _renderer;
    delete _window;
    RAVEN_LOG_INFO("{} - Application Exiting", this->appName);
}

void RavenApp::run() const {
    RAVEN_LOG_INFO("{} - Application Running", this->appName);
    while (!_quit) {
        _eventHandler->Notify(Events::AppUpdateEvent());
        _eventHandler->Notify(Events::AppPostUpdateEvent());
        _eventHandler->Notify(Events::AppRenderEvent());
        _eventHandler->Notify(Events::AppPostRenderEvent());
    }
}

void RavenApp::quitApp() {
    _quit = true;
}



