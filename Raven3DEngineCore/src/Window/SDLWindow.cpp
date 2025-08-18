//
// Created by wesley on 8/6/25.
//
#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Window;

void SDLWindow::GetWindowDimensions(RAVEN_INT &out_width, RAVEN_INT &out_height) {
    SDL_GetWindowSize(_window, &out_width, &out_height);
}

void SDLWindow::CaptureMouse() {
    SDL_SetWindowMouseGrab(_window, true);
    SDL_HideCursor();
}

void SDLWindow::ReleaseMouse() {
    SDL_SetWindowMouseGrab(_window, false);
    SDL_ShowCursor();
}

bool SDLWindow::MouseCaptured() {
    return SDL_GetWindowMouseGrab(_window);
}

void SDLWindow::Initialize(const Rendering::RenderAPI api, const std::string &name, const RAVEN_INT &pixelWidth, const RAVEN_INT &pixelHeight) {
    _name = name;

    _eventHandler->RegisterEventListener(Events::EventType::AppUpdate, [this] (const Events::Event &) { UpdateWindow(); });
    _eventHandler->RegisterEventListener(Events::EventType::AppPostRender, [this] (const Events::Event &) { SwapWindow(); });

    _renderAPI = api;

    std::string rendererName = name;

    if (_renderAPI == Rendering::RenderAPI::OPENGL) {
        _window = SDL_CreateWindow(name.c_str(), pixelWidth, pixelHeight, SDL_WINDOW_OPENGL);
        _context = SDL_GL_CreateContext(_window);
        SDL_GL_MakeCurrent(_window, _context);
        rendererName = "OpenGL";
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS);

    const auto kbInfo = Input::InputDeviceRegistry::registerDevice(Input::DeviceType::KEYBOARD, "SDL_KEYBOARD");
    _eventHandler->Notify(Events::KeyboardConnectedEvent(*kbInfo));
    const auto mouseInfo = Input::InputDeviceRegistry::registerDevice(Input::DeviceType::MOUSE, "SDL_MOUSE");
    _eventHandler->Notify(Events::MouseConnectedEvent(*mouseInfo));

    RAVEN_INT gamePadCount = 0;
    const auto connectedGamePads = SDL_GetGamepads(&gamePadCount);
    for (RAVEN_INT i = 0; i < gamePadCount; i++) {
        const RAVEN_U_INT gamePadId = connectedGamePads[i];
        SDL_OpenGamepad(gamePadId);
        const auto deviceInfo = Input::InputDeviceRegistry::registerDevice(Input::DeviceType::GAMEPAD, "SDL_GAMEPAD_" + std::to_string(gamePadId));
        _eventHandler->Notify(Events::GamepadConnectedEvent(*deviceInfo));
    }

    RAVEN_LOG_INFO("SDL3 Window Initialized for {} Renderer", rendererName);
}

void SDLWindow::UpdateWindow() {

    SDL_Event event;

    RAVEN_INT currWidth, currHeight;
    SDL_GetWindowSize(_window, &currWidth, &currHeight);

    if (SDL_GetWindowMouseGrab(_window)) {
        SDL_WarpMouseInWindow(_window, static_cast<float>(currWidth) / 2, static_cast<float>(currHeight) / 2);
    }

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
                _eventHandler->Notify(Events::WindowCloseEvent());
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED: {
                RAVEN_INT newWidth, newHeight;
                SDL_GetWindowSize(_window, &newWidth, &newHeight);
                _eventHandler->Notify(Events::WindowResizeEvent(newWidth, newHeight));
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                const auto button_down = static_cast<Input::Mouse::MouseCode>(event.button.button);
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_MOUSE");
                _eventHandler->Notify(Events::MousePressedEvent(button_down, *deviceInfo));
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                const auto button_up = static_cast<Input::Mouse::MouseCode>(event.button.button);
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_MOUSE");
                _eventHandler->Notify(Events::MouseReleasedEvent(button_up, *deviceInfo));
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
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_MOUSE");
                _eventHandler->Notify(Events::MouseMovedEvent(x, y, xRel, yRel, *deviceInfo));
                break;
            }
            case SDL_EVENT_MOUSE_WHEEL: {
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_MOUSE");
                _eventHandler->Notify(Events::MouseScrolledEvent(event.wheel.x, event.wheel.y, *deviceInfo));
                break;
            }
            case SDL_EVENT_KEY_DOWN: {
                const auto key_down = static_cast<Input::Key::KeyCode>(event.key.scancode);
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_KEYBOARD");
                _eventHandler->Notify(Events::KeyPressedEvent(key_down, *deviceInfo, event.key.repeat));
                break;

            }
            case SDL_EVENT_KEY_UP: {
                const auto key_up = static_cast<Input::Key::KeyCode>(event.key.scancode);
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_KEYBOARD");
                _eventHandler->Notify(Events::KeyReleasedEvent(key_up, *deviceInfo));
                break;
            }
            case SDL_EVENT_GAMEPAD_ADDED: {
                const auto gamePadId = event.gdevice.which;
                const auto deviceInfo = Input::InputDeviceRegistry::registerDevice(Input::DeviceType::GAMEPAD, "SDL_GAMEPAD_" + std::to_string(gamePadId));
                SDL_OpenGamepad(gamePadId);
                _eventHandler->Notify(Events::GamepadConnectedEvent(*deviceInfo));
                break;
            }
            case SDL_EVENT_GAMEPAD_REMOVED: {
                const auto gamePadId = event.gdevice.which;
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_GAMEPAD_" + std::to_string(gamePadId));
                Input::InputDeviceRegistry::unregisterDevice(deviceInfo->id);
                _eventHandler->Notify(Events::GamepadDisconnectedEvent(*deviceInfo));
                break;
            }
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
                const auto button_down = static_cast<Input::Gamepad::GamepadButtonCode>(event.gbutton.button);
                const auto gamepadId = event.gbutton.which;
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_GAMEPAD_" + std::to_string(gamepadId));
                _eventHandler->Notify(Events::GamepadButtonPressedEvent(button_down, *deviceInfo));
                break;
            }
            case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                const auto button_up = static_cast<Input::Gamepad::GamepadButtonCode>(event.gbutton.button);
                const auto gamepadId = event.gbutton.which;
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_GAMEPAD_" + std::to_string(gamepadId));
                _eventHandler->Notify(Events::GamepadButtonReleasedEvent(button_up, *deviceInfo));
                break;
            }
            case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
                const auto axis = static_cast<Input::Gamepad::GamepadAxisCode>(event.gaxis.axis);
                const auto value = static_cast<RAVEN_FLOAT>(event.gaxis.value);
                const auto gamepadId = event.gaxis.which;
                const auto deviceInfo = Input::InputDeviceRegistry::findFirstDeviceWithName("SDL_GAMEPAD_" + std::to_string(gamepadId));
                _eventHandler->Notify(Events::GamepadAxisEvent(axis, value, *deviceInfo));
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
        SDL_SetWindowTitle(_window, std::format( "{} : FPS = {}",_name, static_cast<RAVEN_INT>(fps)).c_str());
        _frameCount = 0;
        _deltaTime = 0.0f;
    }




    if (_renderAPI == Rendering::RenderAPI::OPENGL) {
        SDL_GL_SwapWindow(_window);
    }
}

