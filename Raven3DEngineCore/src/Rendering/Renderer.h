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

        virtual ~IRenderer() = default;

        virtual void Initialize(const glm::vec3 & clearColor) = 0;
        virtual void RenderFrame() = 0;
    };


    class OpenGLRenderer : public IRenderer {
    public:
        OpenGLRenderer() = default;
        ~OpenGLRenderer() override {
            RAVEN_LOG_INFO("OpenGL Renderer Shut Down");
        }
        void Initialize(const glm::vec3 & clearColor) override;
        void RenderFrame() override;
    };
}



#endif //IRENDERER_H
