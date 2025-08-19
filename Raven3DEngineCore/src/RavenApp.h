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
        Scripting::IScriptManager *_scriptManager;
        Scene::SceneManager *_sceneManager;
        Importer::IImporter *_importer;
        bool _quit = false;

    public:
        std::string appName;

        RavenApp(const std::string &appName, const RAVEN_INT &pixelWidth, const RAVEN_INT &pixelHeight);

        ~RavenApp();
        void run();
        void quitApp();
    };
}



#endif //RAVENAPP_H
