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
    _renderer->Initialize(glm::vec3(0.1f, 0.3f, 0.4f), pixelWidth, pixelHeight);

    _sceneManager = new Scene::SceneManager();
    _sceneManager->SetEventHandler(_eventHandler);
    _sceneManager->Initialize();
    const auto light = _sceneManager->CreateEntity({Scene::NullEntity, "Light"});
    _sceneManager->ConnectComponents<Rendering::LightData, Scene::TransformData>(light,
        Rendering::LightData{ .type = Rendering::LightType::Directional, .posDir = glm::vec3(10.0, -4.0f, 2.0f), .color = glm::vec3(0.8f, 0.7f, 0.6f), .intensity = 2.0f },
        Scene::TransformData{ .localTransform =glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0, 0.0f)) } );


    const Scene::Entity_T camera = _sceneManager->CreateEntity({Scene::NullEntity, "Camera"});
    _sceneManager->ConnectComponents<Rendering::CameraData, Scene::TransformData>(camera,
        Rendering::CameraData{ .fov = 45.0f, .current = true },
        Scene::TransformData{ .localTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 20.0f)) });


    _scriptManager = new Scripting::LuaScriptManager(_sceneManager);
    _scriptManager->SetEventHandler(_eventHandler);

    _importer = new Importer::AssimpImporter();
    _importer->Initialize(_sceneManager, _renderer);
    const auto importedEntity = _importer->Import3DFile(std::string(RAVEN_RESOURCE_PATH) + "3D/car.glb", Scene::NullEntity);


     _eventHandler->RegisterEventListener(Events::EventType::KeyPressed, [=, this] (const Events::Event &e) {
         if (const auto keyEvent = dynamic_cast<const Events::KeyPressedEvent &>(e); keyEvent.getKeyCode() == Input::Key::KeyCode::ESCAPE) {
             quitApp();
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::W) {
             auto *transform =_sceneManager->GetComponent<Scene::TransformData>(camera);
             transform->localTransform = glm::translate(transform->localTransform,  glm::vec3(0.0f, 0.0f, -1.0f));
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::S) {
             auto *transform =_sceneManager->GetComponent<Scene::TransformData>(camera);
             transform->localTransform = glm::translate(transform->localTransform,  glm::vec3(0.0f, 0.0f, 1.0f));
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::A) {
             auto *transform =_sceneManager->GetComponent<Scene::TransformData>(camera);
             transform->localTransform = glm::translate(transform->localTransform,  glm::vec3(-1.0f, 0.0f, 0.0f));
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::D) {
             auto *transform =_sceneManager->GetComponent<Scene::TransformData>(camera);
             transform->localTransform = glm::translate(transform->localTransform,  glm::vec3(1.0f, 0.0f, 0.0f));
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::Q) {
             auto *transform =_sceneManager->GetComponent<Scene::TransformData>(camera);
             transform->localTransform = glm::translate(transform->localTransform,  glm::vec3(0.0f, -1.0f, 0.0f));
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::E) {
             auto *transform =_sceneManager->GetComponent<Scene::TransformData>(camera);
             transform->localTransform = glm::translate(transform->localTransform,  glm::vec3(0.0f, 1.0f, 0.0f));
         }
     });
     _eventHandler->RegisterEventListener(Events::EventType::MouseButtonPressed, [this] (const Events::Event &e) {
         if (const auto mouseEvent = dynamic_cast<const Events::MousePressedEvent &>(e); mouseEvent.getButton() == Input::Mouse::MouseCode::BUTTON_LEFT) {
             _window->CaptureMouse();
         } else if (mouseEvent.getButton() == Input::Mouse::MouseCode::BUTTON_RIGHT) {
             _window->ReleaseMouse();
         }
     });
     _eventHandler->RegisterEventListener(Events::EventType::MouseMoved, [=, this] (const Events::Event &e) {
         if (!_window->MouseCaptured()) {
             return;
         }
         const auto mouseEvent = dynamic_cast<const Events::MouseMovedEvent &>(e);
         auto *transform =_sceneManager->GetComponent<Scene::TransformData>(camera);
         transform->localTransform = glm::rotate(transform->localTransform, glm::radians(mouseEvent.getXDelta()*-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
         transform->localTransform = glm::rotate(transform->localTransform, glm::radians(mouseEvent.getYDelta()*-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));

     });

    _sceneManager->PrintSceneGraph();
    RAVEN_LOG_INFO("{} - Application Initialized", this->appName);
}

RavenApp::~RavenApp() {
    delete _importer;
    delete _sceneManager;
    delete _renderer;
    delete _window;
    delete _eventHandler;
    RAVEN_LOG_INFO("{} - Application Exiting", this->appName);
}

void RavenApp::run() {
    RAVEN_LOG_INFO("{} - Application Running", this->appName);
    while (!_quit) {
        try {
            _eventHandler->Notify(Events::AppUpdateEvent());
            _eventHandler->Notify(Events::AppPostUpdateEvent());
            _eventHandler->Notify(Events::AppPreRenderEvent(_renderer));
            _eventHandler->Notify(Events::AppRenderEvent());
            _eventHandler->Notify(Events::AppPostRenderEvent());
        } catch (const std::exception &e) {
            RAVEN_LOG_ERROR("{} - Application Error: {}", this->appName, e.what());
            _quit = true;
        }

    }
}

void RavenApp::quitApp() {
    _quit = true;
}



