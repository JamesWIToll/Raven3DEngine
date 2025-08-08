//
// Created by wesley on 8/6/25.
//

#ifndef WINDOW_H
#define WINDOW_H

namespace Raven3DEngineCore::Window {
    class IRenderWindow {
    protected:
        Events::EventHandler _eventHandler;
    public:
        virtual ~IRenderWindow() = default;

        void SetEventHandler(const Events::EventHandler handler) { _eventHandler = handler; }

        virtual void Initialize(Rendering::RenderAPI api, const std::string &name, const int &pixelWidth, const int &pixelHeight) = 0;
        virtual void UpdateWindow() = 0;
        virtual void SwapWindow() = 0;
    };




    class SDLWindow final: public IRenderWindow {
        Rendering::RenderAPI _renderAPI = Rendering::RenderAPI::OPENGL;
        SDL_Window *_window = nullptr;

        SDL_GLContext _context = nullptr;
        SDL_Surface *_surface = nullptr;

    public:
        ~SDLWindow() override {
            if (_renderAPI == Rendering::RenderAPI::OPENGL) {
                SDL_GL_DestroyContext(_context);
            }
            SDL_DestroyWindow(_window);
            _context = nullptr;
            _surface = nullptr;
            _window = nullptr;
            RAVEN_LOG_INFO("SDL Window Closed");
        }

        void Initialize(Rendering::RenderAPI api, const std::string &name, const int &pixelWidth, const int &pixelHeight) override;
        void UpdateWindow() override;
        void SwapWindow() override;

    };

}


#endif //WINDOW_H
