//
// Created by wesley on 8/6/25.
//
#include <Raven3DEngineCore.h>

#include "glm/gtx/io.hpp"

using namespace Raven3DEngineCore::Window;

static bool SDLCheck(const bool success) {
    if (!success) {
        RAVEN_LOG_ERROR("SDL3 - ERROR: {}", SDL_GetError());
    }
    return success;
}

SDLWindow::~SDLWindow()  {
    ReleaseMouse();
    if (_renderAPI == Rendering::RenderAPI::OPENGL) {
        SDLCheck(SDL_GL_DestroyContext(_context));
    }
    SDL_DestroyWindow(_window);
    _context = nullptr;
    _surface = nullptr;
    _window = nullptr;
    RAVEN_LOG_INFO("SDL Window Closed");
}
void SDLWindow::GetWindowDimensions(RAVEN_INT &out_width, RAVEN_INT &out_height) {
    SDLCheck(SDL_GetWindowSize(_window, &out_width, &out_height));
}

void SDLWindow::MakeCurrent() {
    if (_renderAPI == Rendering::RenderAPI::OPENGL) {
        SDLCheck(SDL_GL_MakeCurrent(_window, _context));
    }
}

void SDLWindow::CaptureMouse() {
    SDLCheck(SDL_SetWindowMouseGrab(_window, true));
    SDLCheck(SDL_HideCursor());
}

void SDLWindow::ReleaseMouse() {
    SDLCheck(SDL_SetWindowMouseGrab(_window, false));
    SDLCheck(SDL_ShowCursor());
}

bool SDLWindow::MouseCaptured() {
    return SDL_GetWindowMouseGrab(_window);
}

RAVEN_U_INT SDLWindow::Initialize(const Rendering::RenderAPI api, const std::string &name, const RAVEN_INT &pixelWidth, const RAVEN_INT &pixelHeight) {
    SDLCheck(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS));

    _name = name;

    _eventHandler->RegisterEventListener(Events::EventType::AppUpdate, [this] (const Events::Event &) { UpdateWindow(); });
    _eventHandler->RegisterEventListener(Events::EventType::AppPostRender, [this] (const Events::Event &) { SwapWindow(); });

    _renderAPI = api;

    _width = pixelWidth;
    _height = pixelHeight;

    std::string rendererName = name;

    if (_renderAPI == Rendering::RenderAPI::OPENGL) {
        _window = SDL_CreateWindow(name.c_str(), pixelWidth, pixelHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        SDLCheck(_window != nullptr);

        RAVEN_INT glContextFlags = 0;
        SDLCheck(SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &glContextFlags));
        SDLCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, glContextFlags | SDL_GL_CONTEXT_DEBUG_FLAG));
        _context = SDL_GL_CreateContext(_window);
        SDLCheck(_context != nullptr);

        SDLCheck(SDL_GL_MakeCurrent(_window, _context));
        rendererName = "OpenGL";
    }

    auto vp = Viewports::Viewport();
    vp.width = _width - 10;
    vp.height = _height - 10;
    vp.x_offset = 5;
    vp.y_offset = 5;
    vp.renderAPI = _renderAPI;
    vp.borderWidth = 5;
    vp.borderColor[0] = 0.05f;
    vp.borderColor[1] = 0.1f;
    vp.borderColor[2] = 0.2f;
    vp.borderColor[3] = 1.0f;
    vp.window = this;
    const auto vpID = Viewports::globalViewportManager->AddViewport(vp);

    const auto kbInfo = Input::InputDeviceRegistry::registerDevice(Input::DeviceType::KEYBOARD, "SDL_KEYBOARD - " + GetName());
    _eventHandler->Notify(Events::KeyboardConnectedEvent(*kbInfo, this));
    const auto mouseInfo = Input::InputDeviceRegistry::registerDevice(Input::DeviceType::MOUSE, "SDL_MOUSE - " + GetName());
    _eventHandler->Notify(Events::MouseConnectedEvent(*mouseInfo, this));

    RAVEN_INT gamePadCount = 0;
    const auto connectedGamePads = SDL_GetGamepads(&gamePadCount);
    SDLCheck(connectedGamePads != nullptr);
    for (RAVEN_INT i = 0; i < gamePadCount; i++) {
        const RAVEN_U_INT gamePadId = connectedGamePads[i];
        SDLCheck(SDL_OpenGamepad(gamePadId));
        const auto deviceInfo = Input::InputDeviceRegistry::registerDevice(Input::DeviceType::GAMEPAD, "SDL_GAMEPAD_" + std::to_string(gamePadId) + " - " + GetName());
        _eventHandler->Notify(Events::GamepadConnectedEvent(*deviceInfo, this));
    }

    RAVEN_LOG_INFO("SDL3 Window Initialized for {} Renderer", rendererName);
    return vpID;
}

void SDLWindow::UpdateWindow() {
    const auto windowViewports = Viewports::globalViewportManager->GetViewportsForWindow(this);

    SDL_Event event;

    RAVEN_INT currWidth, currHeight;
    SDLCheck(SDL_GetWindowSize(_window, &currWidth, &currHeight));

    if (SDL_GetWindowMouseGrab(_window)) {
        SDL_WarpMouseInWindow(_window, static_cast<float>(currWidth) / 2, static_cast<float>(currHeight) / 2);
    }

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
                _eventHandler->Notify(Events::WindowCloseEvent(this));
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED: {
                RAVEN_INT newWidth, newHeight;
                SDLCheck(SDL_GetWindowSize(_window, &newWidth, &newHeight));


                for (int i = 0; i < windowViewports.size(); i++) { // NOLINT(*-loop-convert)
                    const auto viewport = Viewports::globalViewportManager->GetViewport(windowViewports[i]);
                    const RAVEN_FLOAT vpWidthProportion = static_cast<RAVEN_FLOAT>(viewport->width) / static_cast<RAVEN_FLOAT>(_width);
                    const RAVEN_FLOAT vpHeightProportion = static_cast<RAVEN_FLOAT>(viewport->height) / static_cast<RAVEN_FLOAT>(_height);
                    viewport->width = static_cast<RAVEN_U_INT>(vpWidthProportion * static_cast<RAVEN_FLOAT>(newWidth));
                    viewport->height = static_cast<RAVEN_U_INT>(vpHeightProportion * static_cast<RAVEN_FLOAT>(newHeight));
                }

                _width = newWidth;
                _height = newHeight;

                _eventHandler->Notify(Events::WindowResizeEvent(newWidth, newHeight, this));
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                const auto button_down = static_cast<Input::Mouse::MouseCode>(event.button.button);
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_MOUSE - " + GetName());
                _eventHandler->Notify(Events::MousePressedEvent(button_down, *deviceInfo, this));
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                const auto button_up = static_cast<Input::Mouse::MouseCode>(event.button.button);
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_MOUSE - " + GetName());
                _eventHandler->Notify(Events::MouseReleasedEvent(button_up, *deviceInfo, this));
                break;
            }
            case SDL_EVENT_MOUSE_MOTION: {
                RAVEN_FLOAT x = event.motion.x;
                RAVEN_FLOAT y = event.motion.y;
                RAVEN_FLOAT xRel = event.motion.xrel;
                RAVEN_FLOAT yRel = event.motion.yrel;
                if (xRel > -2 && xRel < 2) xRel = 0;
                if (yRel > -2 && yRel < 2) yRel = 0;
                if (xRel == 0 && yRel == 0) {
                    break;
                }
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_MOUSE - " + GetName());
                _eventHandler->Notify(Events::MouseMovedEvent(x, y, xRel, yRel, *deviceInfo, this));
                break;
            }
            case SDL_EVENT_MOUSE_WHEEL: {
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_MOUSE - " + GetName());
                _eventHandler->Notify(Events::MouseScrolledEvent(event.wheel.x, event.wheel.y, *deviceInfo, this));
                break;
            }
            case SDL_EVENT_KEY_DOWN: {
                const auto key_down = static_cast<Input::Key::KeyCode>(event.key.scancode);
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_KEYBOARD - " + GetName());
                _eventHandler->Notify(Events::KeyPressedEvent(key_down, *deviceInfo, this, event.key.repeat));
                break;

            }
            case SDL_EVENT_KEY_UP: {
                const auto key_up = static_cast<Input::Key::KeyCode>(event.key.scancode);
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_KEYBOARD - " + GetName());
                _eventHandler->Notify(Events::KeyReleasedEvent(key_up, *deviceInfo, this));
                break;
            }
            case SDL_EVENT_GAMEPAD_ADDED: {
                const auto gamePadId = event.gdevice.which;
                const auto deviceInfo = Input::InputDeviceRegistry::registerDevice(Input::DeviceType::GAMEPAD, "SDL_GAMEPAD_" + std::to_string(gamePadId) + " - " + GetName());
                SDLCheck(SDL_OpenGamepad(gamePadId));
                _eventHandler->Notify(Events::GamepadConnectedEvent(*deviceInfo, this));
                break;
            }
            case SDL_EVENT_GAMEPAD_REMOVED: {
                const auto gamePadId = event.gdevice.which;
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_GAMEPAD_" + std::to_string(gamePadId) + " - " + GetName());
                Input::InputDeviceRegistry::unregisterDevice(deviceInfo->id);
                _eventHandler->Notify(Events::GamepadDisconnectedEvent(*deviceInfo, this));
                break;
            }
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
                const auto button_down = static_cast<Input::Gamepad::GamepadButtonCode>(event.gbutton.button);
                const auto gamepadId = event.gbutton.which;
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_GAMEPAD_" + std::to_string(gamepadId) + " - " + GetName());
                _eventHandler->Notify(Events::GamepadButtonPressedEvent(button_down, *deviceInfo, this));
                break;
            }
            case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                const auto button_up = static_cast<Input::Gamepad::GamepadButtonCode>(event.gbutton.button);
                const auto gamepadId = event.gbutton.which;
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_GAMEPAD_" + std::to_string(gamepadId) + " - " + GetName());
                _eventHandler->Notify(Events::GamepadButtonReleasedEvent(button_up, *deviceInfo, this));
                break;
            }
            case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
                const auto axis = static_cast<Input::Gamepad::GamepadAxisCode>(event.gaxis.axis);
                const auto value = static_cast<RAVEN_FLOAT>(event.gaxis.value);
                const auto gamepadId = event.gaxis.which;
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_GAMEPAD_" + std::to_string(gamepadId) + " - " + GetName());
                _eventHandler->Notify(Events::GamepadAxisEvent(axis, value, *deviceInfo, this));
                break;
            }
            default: break;
        }
    }
}

void SDLWindow::SwapWindow() {
    _deltaTime += RAVEN_UPDATE_DELTA();
    _frameCount++;
    if (_deltaTime >= 1.0f) {
        const auto fps =  _frameCount / _deltaTime;
        SDLCheck(SDL_SetWindowTitle(_window, std::format( "{} : FPS = {}",_name, static_cast<RAVEN_INT>(fps)).c_str()));
        _frameCount = 0;
        _deltaTime = 0.0f;
    }


    if (_renderAPI == Rendering::RenderAPI::OPENGL) {
        SDLCheck(SDL_GL_SwapWindow(_window));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

