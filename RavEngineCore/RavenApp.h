//
// Created by wesley on 8/2/25.
//

#ifndef RAVENAPP_H
#define RAVENAPP_H
#include <string>

#include "Rendering/IRenderer.h"
#include "SDL3/SDL_video.h"

namespace RavEngineCore {
    class RavenApp {
        Rendering::IRenderer *renderer;
    public:
        RavenApp(const std::string &appName, const int &pixelWidth, const int &pixelHeight);

        ~RavenApp();
        void run() const;
    };
}



#endif //RAVENAPP_H
