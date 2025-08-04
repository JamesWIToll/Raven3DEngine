//
// Created by wesley on 8/2/25.
//

#include "RavenApp.h"
#include <iostream>
#include "SDL3/SDL_events.h"

using namespace RavEngineCore;

RavenApp::RavenApp(const std::string& appName, const int &pixelWidth, const int &pixelHeight) {
    renderer = new Rendering::OpenGLRenderer();
    renderer->InitializeWindow(appName, pixelWidth, pixelHeight, glm::vec3(0.1f, 0.3f, 0.5f));

}

RavenApp::~RavenApp() {
    delete renderer;
}

void RavenApp::run() const {
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    quit = true;
                    break;
                default: break;
            }
        }

        renderer->RenderFrame();
    }
}


