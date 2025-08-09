//
// Created by wesley on 8/6/25.
//
#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Window;

void SDLWindow::Initialize(const Rendering::RenderAPI api, const std::string &name, const int &pixelWidth, const int &pixelHeight) {

    _renderAPI = api;

    std::string rendererName = name;

    if (_renderAPI == Rendering::RenderAPI::OPENGL) {
        _window = SDL_CreateWindow(name.c_str(), pixelWidth, pixelHeight, SDL_WINDOW_OPENGL);
        _context = SDL_GL_CreateContext(_window);
        SDL_GL_MakeCurrent(_window, _context);
        rendererName = "OpenGL";
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS);

    int gamePadCount = 0;
    const auto connectedGamePads = SDL_GetGamepads(&gamePadCount);

    for (int i = 0; i < gamePadCount; ++i) {
        const unsigned int gamePadId = connectedGamePads[i];
        SDL_OpenGamepad(gamePadId);
        _eventHandler.Notify(Events::GamepadConnectedEvent(gamePadId));
    }

    RAVEN_LOG_INFO("SDL3 Window Initialized for {} Renderer", rendererName);
}

void SDLWindow::UpdateWindow() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
                _eventHandler.Notify(Events::WindowCloseEvent());
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED: {
                int newWidth, newHeight;
                SDL_GetWindowSize(_window, &newWidth, &newHeight);
                _eventHandler.Notify(Events::WindowResizeEvent(newWidth, newHeight));
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                const auto button_down = static_cast<Input::Mouse::MouseCode>(event.button.button);
                _eventHandler.Notify(Events::MousePressedEvent(button_down));
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                const auto button_up = static_cast<Input::Mouse::MouseCode>(event.button.button);
                _eventHandler.Notify(Events::MouseReleasedEvent(button_up));
                break;
            }
            case SDL_EVENT_MOUSE_MOTION: {
                float x, y; SDL_GetMouseState(&x, &y);
                _eventHandler.Notify(Events::MouseMovedEvent(x, y));
                break;
            }
            case SDL_EVENT_MOUSE_WHEEL: {
                _eventHandler.Notify(Events::MouseScrolledEvent(event.wheel.x, event.wheel.y));
                break;
            }
            case SDL_EVENT_KEY_DOWN: {
                const auto key_down = static_cast<Input::Key::KeyCode>(event.key.scancode);
                _eventHandler.Notify(Events::KeyPressedEvent(key_down, event.key.repeat));
                break;

            }
            case SDL_EVENT_KEY_UP: {
                const auto key_up = static_cast<Input::Key::KeyCode>(event.key.scancode);
                _eventHandler.Notify(Events::KeyReleasedEvent(key_up));
                break;
            }
            case SDL_EVENT_GAMEPAD_ADDED: {
                const auto gamePadId = event.gdevice.which;
                SDL_OpenGamepad(gamePadId);
                _eventHandler.Notify(Events::GamepadConnectedEvent(gamePadId));
                break;
            }
            case SDL_EVENT_GAMEPAD_REMOVED: {
                const auto gamePadId = event.gdevice.which;
                _eventHandler.Notify(Events::GamepadDisconnectedEvent(gamePadId));
                break;
            }
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
                const auto button_down = static_cast<Input::Gamepad::GamepadButtonCode>(event.gbutton.button);
                const auto gamepadId = event.gdevice.which;
                _eventHandler.Notify(Events::GamepadButtonPressedEvent(button_down, gamepadId));
                break;
            }
            case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                const auto button_up = static_cast<Input::Gamepad::GamepadButtonCode>(event.gbutton.button);
                const auto gamepadId = event.gdevice.which;
                _eventHandler.Notify(Events::GamepadButtonReleasedEvent(button_up, gamepadId));
                break;
            }
            case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
                const auto axis = static_cast<Input::Gamepad::GamepadAxisCode>(event.gaxis.axis);
                const auto value = static_cast<float>(event.gaxis.value);
                const auto gamepadId = event.gdevice.which;
                _eventHandler.Notify(Events::GamepadAxisEvent(axis, value, gamepadId));
            }
            default: break;
        }
    }
}

void SDLWindow::SwapWindow() {
    if (_renderAPI == Rendering::RenderAPI::OPENGL) {
        SDL_GL_SwapWindow(_window);
    }
}

