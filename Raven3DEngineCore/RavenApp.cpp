//
// Created by wesley on 8/2/25.
//

#include "RavenApp.h"
#include "SDL3/SDL_events.h"

using namespace Raven3DEngineCore;

RavenApp::RavenApp(const std::string& appName, const int &pixelWidth, const int &pixelHeight) {
    renderer = new Rendering::OpenGLRenderer();
    renderer->InitializeWindow(appName, pixelWidth, pixelHeight, glm::vec3(0.1f, 0.3f, 0.4f));

}

RavenApp::~RavenApp() {
    delete renderer;
}

void RavenApp::run() const {
    bool quit = false;

    while (!quit) {

        renderer->RenderFrame();
    }
}


