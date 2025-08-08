//
// Created by wesley on 8/4/25.
//

#ifndef IRENDERER_H
#define IRENDERER_H

namespace Raven3DEngineCore::Rendering {


    enum class RenderAPI {
        OPENGL,
    };

    class IRenderer {
    public:
        SDL_Window* window = nullptr;

        virtual ~IRenderer() {
            SDL_DestroyWindow(window);
            window = nullptr;
        };

        virtual void Initialize(const glm::vec3 & clearColor) = 0;
        virtual void RenderFrame() = 0;
    };


    class OpenGLRenderer : public IRenderer {
        SDL_GLContext context = nullptr;
    public:
        OpenGLRenderer() = default;
        ~OpenGLRenderer() override {
            SDL_GL_DestroyContext(context);
            SDL_DestroyWindow(window);
            context = nullptr;
            window = nullptr;
        }
        void Initialize(const glm::vec3 & clearColor) override;
        void RenderFrame() override;
    };
}



#endif //IRENDERER_H
