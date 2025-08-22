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
    public:
        ~IRenderWindow() override = default;
        //returns created viewport ID
        virtual RAVEN_U_INT Initialize(Rendering::RenderAPI api, const std::string &name, const RAVEN_INT &pixelWidth, const RAVEN_INT &pixelHeight) = 0;
        virtual void MakeCurrent() = 0;
        virtual void CaptureMouse() = 0;
        virtual void ReleaseMouse() = 0;
        virtual bool MouseCaptured() = 0;
        virtual void GetWindowDimensions(RAVEN_INT &out_width, RAVEN_INT &out_height) = 0;
        virtual void UpdateWindow() = 0;
        virtual void SwapWindow() = 0;
    };

}


#endif //WINDOW_H
