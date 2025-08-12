//
// Created by wesley on 8/4/25.
//

#ifndef IRENDERER_H
#define IRENDERER_H

namespace Raven3DEngineCore::Rendering {

    enum class RenderAPI : ushort{
        OPENGL,
    };

    class IRenderer : public Events::EventNotifier {
    public:

        virtual ~IRenderer() = default;

        virtual void QueueForRender(RenderData *data, Scene::TransformData *transform) = 0;

        virtual void Initialize(const glm::vec3 &clearColor) = 0;
        virtual void RenderFrame() = 0;
    };


    class OpenGLRenderer final : public IRenderer {
        std::vector<std::pair<RenderData*, Scene::TransformData*>> _renderData {};
        std::vector<std::pair<RenderData*, Scene::TransformData*>> _transparentRenderData {};
    public:
        OpenGLRenderer() = default;
        ~OpenGLRenderer() override {
            RAVEN_LOG_INFO("OpenGL Renderer Shut Down");
        }

        void QueueForRender(RenderData *data, Scene::TransformData *transform) override;
        void Initialize(const glm::vec3 & clearColor) override;
        void RenderFrame() override;
    };
}



#endif //IRENDERER_H
