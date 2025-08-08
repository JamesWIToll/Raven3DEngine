//
// Created by wesley on 8/2/25.
//

#ifndef RAVENAPP_H
#define RAVENAPP_H

namespace Raven3DEngineCore {
    class RavenApp {
        Rendering::IRenderer *_renderer;
        Window::IRenderWindow *_window;
        Events::EventHandler *_eventHandler;
        bool _quit = false;

    public:
        RavenApp(const std::string &appName, const int &pixelWidth, const int &pixelHeight);

        ~RavenApp();
        void run() const;
        void quitApp();
    };
}



#endif //RAVENAPP_H
