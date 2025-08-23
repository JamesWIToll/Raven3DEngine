//
// Created by wesley on 8/22/2025.
//

#ifndef RAVEN3DENGINECORE_SDLWINDOW_H
#define RAVEN3DENGINECORE_SDLWINDOW_H

namespace Raven3DEngineCore::Window {

    class SDLWindow final: public IRenderWindow {
        Rendering::RenderAPI _renderAPI = Rendering::RenderAPI::OPENGL;
        SDL_Window *_window = nullptr;
        SDL_Surface *_surface = nullptr;
        std::string _name {};

    public:
        ~SDLWindow() override;

        bool SDLCheck(bool success);

        void GetWindowDimensions(RAVEN_INT &out_width, RAVEN_INT &out_height) override;
        void MakeCurrent() override;
        void CaptureMouse() override;
        void ReleaseMouse() override;
        bool MouseCaptured() override;
        //returns created viewport id
        void Initialize(Rendering::RenderAPI api, const std::string &name, const RAVEN_INT &pixelWidth, const RAVEN_INT &pixelHeight) override;
        void UpdateWindow() override;
        void SwapWindow() override;
        std::string GetName() override { return _name; }

    };

    extern SDL_GLContext sharedSDLGLContext;

    extern void ShutdownSharedSDLWindowData();
}

#endif //RAVEN3DENGINECORE_SDLWINDOW_H