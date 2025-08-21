//
// Created by wesley on 8/6/25.
//

#ifndef WINDOW_H
#define WINDOW_H

namespace Raven3DEngineCore::Window {
    class IRenderWindow : public Events::EventNotifier {
    protected:
        RAVEN_INT _frameCount = 0;
        RAVEN_U_INT _width = 0, _height = 0;
        RAVEN_FLOAT _deltaTime = 0.0f;
        std::vector<RAVEN_U_INT> _viewports;
    public:
        ~IRenderWindow() override = default;
        virtual void Initialize(Rendering::RenderAPI api, const std::string &name, const RAVEN_INT &pixelWidth, const RAVEN_INT &pixelHeight) = 0;
        virtual void CaptureMouse() = 0;
        virtual void ReleaseMouse() = 0;
        virtual bool MouseCaptured() = 0;
        virtual void GetWindowDimensions(RAVEN_INT &out_width, RAVEN_INT &out_height) = 0;
        virtual void UpdateWindow() = 0;
        virtual void SwapWindow() = 0;

        virtual Rendering::IRenderer* GetRenderer() = 0;
    };

    class SDLWindow final: public IRenderWindow {
        Rendering::RenderAPI _renderAPI = Rendering::RenderAPI::OPENGL;
        SDL_Window *_window = nullptr;
        Rendering::IRenderer *_renderer = nullptr;

        SDL_GLContext _context = nullptr;
        SDL_Surface *_surface = nullptr;
        std::string _name {};

    public:
        ~SDLWindow() override;

        void GetWindowDimensions(RAVEN_INT &out_width, RAVEN_INT &out_height) override;
        void CaptureMouse() override;
        void ReleaseMouse() override;
        bool MouseCaptured() override;
        void Initialize(Rendering::RenderAPI api, const std::string &name, const RAVEN_INT &pixelWidth, const RAVEN_INT &pixelHeight) override;
        void UpdateWindow() override;
        void SwapWindow() override;

        Rendering::IRenderer* GetRenderer() override;

    };

}


#endif //WINDOW_H
