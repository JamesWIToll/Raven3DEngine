//
// Created by wesley on 8/6/25.
//

#ifndef WINDOW_H
#define WINDOW_H

namespace Raven3DEngineCore::Window {
    class IRenderWindow : public Events::EventNotifier {
    public:
        ~IRenderWindow() override = default;
        virtual void Initialize(Rendering::RenderAPI api, const std::string &name, const RAVEN_INT &pixelWidth, const RAVEN_INT &pixelHeight) = 0;
        virtual void CaptureMouse() = 0;
        virtual void ReleaseMouse() = 0;
        virtual bool MouseCaptured() = 0;
        virtual void GetWindowDimensions(RAVEN_INT &out_width, RAVEN_INT &out_height) = 0;
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

        void GetWindowDimensions(RAVEN_INT &out_width, RAVEN_INT &out_height) override;
        void CaptureMouse() override;
        void ReleaseMouse() override;
        bool MouseCaptured() override;
        void Initialize(Rendering::RenderAPI api, const std::string &name, const RAVEN_INT &pixelWidth, const RAVEN_INT &pixelHeight) override;
        void UpdateWindow() override;
        void SwapWindow() override;

    };

}


#endif //WINDOW_H
