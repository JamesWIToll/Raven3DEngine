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
            default: break;
        }
    }
}

void SDLWindow::SwapWindow() {
    if (_window == nullptr) { return; }

    if (_renderAPI == Rendering::RenderAPI::OPENGL) {
        SDL_GL_SwapWindow(_window);
    }
}

