//
// Created by wesley on 8/2/25.
//

#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore;

static const auto loggerListener = new Logging::ConsoleLog();

static RAVEN_INT windowCount = 0;
static RAVEN_U_LONG appEventListenerID = Events::GetNextEventListenerID();

Viewports::ViewportManager *Viewports::globalViewportManager;

RavenApp::RavenApp(const std::string& appName, const RAVEN_INT &pixelWidth, const RAVEN_INT &pixelHeight) {
    Viewports::globalViewportManager = new Viewports::ViewportManager();
    this->appName = appName;

    RAVEN_REGISTER_LOG_LISTENER(loggerListener);
    RAVEN_LOG_DEBUG("Debug mode active");

    _eventHandler = new Events::EventHandler();
    Viewports::globalViewportManager->SetEventHandler(_eventHandler);

    _eventHandler->RegisterEventListener(Events::EventType::WindowClosed, [this] (const Events::Event &e) {
        if (const auto windowEvent = dynamic_cast<const Events::WindowCloseEvent &>(e);
            !Viewports::globalViewportManager->CloseWindow(windowEvent.getWindow())) {

            delete windowEvent.getWindow();
        }
        if (!Viewports::globalViewportManager->HasViewports()) {
            quitApp();
        }
    }, appEventListenerID);

    _window = new Window::SDLWindow();
    _window->SetEventHandler(_eventHandler);
    _window->Initialize(Rendering::RenderAPI::OPENGL, appName + ": SDL Window " + std::to_string(windowCount), pixelWidth, pixelHeight);
    windowCount++;

    auto vp = Viewports::Viewport();
    vp.width = pixelWidth - 10;
    vp.height = pixelHeight - 10;
    vp.x_offset = 5;
    vp.y_offset = 5;
    vp.renderAPI = Rendering::RenderAPI::OPENGL;
    vp.borderWidth = 5;
    vp.borderColor[0] = 0.05f;
    vp.borderColor[1] = 0.1f;
    vp.borderColor[2] = 0.2f;
    vp.borderColor[3] = 1.0f;
    vp.window = _window;
    const auto vpID = Viewports::globalViewportManager->AddViewport(vp);

    _sceneManager = new Scene::SceneManager(vpID);
    _sceneManager->SetEventHandler(_eventHandler);
    _sceneManager->Initialize();

    const auto light = _sceneManager->CreateEntity({Scene::NullEntity, "Light"});
    _sceneManager->ConnectComponents<Rendering::LightData3D, Scene::TransformData3D>(light,
        Rendering::LightData3D{ .type = Rendering::LightType::Directional, .posDir = glm::vec3(10.0, -4.0f, 2.0f), .color = glm::vec3(0.8f, 0.7f, 0.6f), .intensity = 2.0f },
        Scene::TransformData3D{ .localTransform =glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0, 0.0f)) } );


    const Scene::Entity_T camera = _sceneManager->CreateEntity({Scene::NullEntity, "Camera"});
    _sceneManager->ConnectComponents<Rendering::CameraData3D, Scene::TransformData3D>(camera,
        Rendering::CameraData3D{ .fov = 45.0f, .current = true },
        Scene::TransformData3D{ .localTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 20.0f)) });


    _scriptManager = new Scripting::LuaScriptManager(_sceneManager);
    _scriptManager->SetEventHandler(_eventHandler);

    _importer = new Importer::AssimpImporter();
    _importer->Initialize(_sceneManager);
    const auto importedEntity = _importer->ImportFile(std::string(RAVEN_RESOURCE_PATH) + "3D/car.glb", Scene::NullEntity);


     _eventHandler->RegisterEventListener(Events::EventType::KeyPressed, [=, this] (const Events::Event &e) {
         if (const auto keyEvent = dynamic_cast<const Events::KeyPressedEvent &>(e); keyEvent.getKeyCode() == Input::Key::KeyCode::ESCAPE) {
             quitApp();
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::W) {
             auto *transform =_sceneManager->GetComponent<Scene::TransformData3D>(camera);
             transform->localTransform = glm::translate(transform->localTransform,  glm::vec3(0.0f, 0.0f, -1.0f));
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::S) {
             auto *transform =_sceneManager->GetComponent<Scene::TransformData3D>(camera);
             transform->localTransform = glm::translate(transform->localTransform,  glm::vec3(0.0f, 0.0f, 1.0f));
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::A) {
             auto *transform =_sceneManager->GetComponent<Scene::TransformData3D>(camera);
             transform->localTransform = glm::translate(transform->localTransform,  glm::vec3(-1.0f, 0.0f, 0.0f));
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::D) {
             auto *transform =_sceneManager->GetComponent<Scene::TransformData3D>(camera);
             transform->localTransform = glm::translate(transform->localTransform,  glm::vec3(1.0f, 0.0f, 0.0f));
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::Q) {
             auto *transform =_sceneManager->GetComponent<Scene::TransformData3D>(camera);
             transform->localTransform = glm::translate(transform->localTransform,  glm::vec3(0.0f, -1.0f, 0.0f));
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::E) {
             auto *transform =_sceneManager->GetComponent<Scene::TransformData3D>(camera);
             transform->localTransform = glm::translate(transform->localTransform,  glm::vec3(0.0f, 1.0f, 0.0f));
         } else if (keyEvent.getKeyCode() == Input::Key::KeyCode::SPACE) {
             auto *vp1 =Viewports::globalViewportManager->GetViewport(vpID);
             auto *win = new Window::SDLWindow();
             win->SetEventHandler(_eventHandler);
             win->Initialize(vp1->renderAPI, "SDL Window " + std::to_string(windowCount++), vp1->width, vp1->height);
             Viewports::globalViewportManager->MoveToNewWindow(vpID, win);
         }
     }, appEventListenerID);
     _eventHandler->RegisterEventListener(Events::EventType::MouseButtonPressed, [this] (const Events::Event &e) {
         if (const auto mouseEvent = dynamic_cast<const Events::MousePressedEvent &>(e); mouseEvent.getButton() == Input::Mouse::MouseCode::BUTTON_LEFT) {
             mouseEvent.GetWindow()->CaptureMouse();
         } else if (mouseEvent.getButton() == Input::Mouse::MouseCode::BUTTON_RIGHT) {
             mouseEvent.GetWindow()->ReleaseMouse();
         }
     }, appEventListenerID);
     _eventHandler->RegisterEventListener(Events::EventType::MouseMoved, [=, this] (const Events::Event &e) {
         const auto mouseEvent = dynamic_cast<const Events::MouseMovedEvent &>(e);
         if (!mouseEvent.GetWindow()->MouseCaptured()) {
             return;
         }
         auto *transform =_sceneManager->GetComponent<Scene::TransformData3D>(camera);
         transform->localTransform = glm::rotate(transform->localTransform, glm::radians(mouseEvent.getXDelta()*-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
         transform->localTransform = glm::rotate(transform->localTransform, glm::radians(mouseEvent.getYDelta()*-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));

     }, appEventListenerID);

    _sceneManager->PrintSceneGraph();
    RAVEN_LOG_INFO("{} - Application Initialized", this->appName);
}

RavenApp::~RavenApp() {
    _eventHandler->UnregisterListenerAllEvents(appEventListenerID);
    delete _importer;
    delete Viewports::globalViewportManager;
    delete _sceneManager;
    delete _eventHandler;
    RAVEN_LOG_INFO("{} - Application Exiting", this->appName);
}

#define CHECK_QUIT if (_quit) break;

void RavenApp::run() {
    RAVEN_LOG_INFO("{} - Application Running", this->appName);
    while (!_quit) {
        try {
            _eventHandler->Notify(Events::AppUpdateEvent()); CHECK_QUIT
            _eventHandler->Notify(Events::AppPostUpdateEvent()); CHECK_QUIT
            _eventHandler->Notify(Events::AppPreRenderEvent()); CHECK_QUIT
            _eventHandler->Notify(Events::AppRenderEvent()); CHECK_QUIT
            _eventHandler->Notify(Events::AppPostRenderEvent()); CHECK_QUIT
        } catch (const std::exception &e) {
            RAVEN_LOG_ERROR("{} - Application Error: {}", this->appName, e.what());
            _quit = true;
        }

    }
}

void RavenApp::quitApp() {
    _quit = true;
}



