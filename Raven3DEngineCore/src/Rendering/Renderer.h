//
// Created by wesley on 8/4/25.
//

#ifndef IRENDERER_H
#define IRENDERER_H

#define MAX_LIGHTS 50

namespace Raven3DEngineCore::Rendering {

    class IRenderer : public Events::EventNotifier {
    protected:
        RAVEN_U_INT _viewportID;

        virtual void RenderMesh(RenderData &renderData, Scene::TransformData &transformData) = 0;
    public:
        explicit IRenderer(const RAVEN_U_INT viewportID) : _viewportID(viewportID) {}
        ~IRenderer() override = default;

        virtual void ReleaseRenderData(RenderData *data) = 0;

        virtual void QueueForRender(RenderData *data, Scene::TransformData *transform) = 0;
        virtual void AddLight(LightData *data) = 0;
        virtual void SetActiveCam(CameraData *data, Scene::TransformData *camTransform) = 0;
        virtual RAVEN_U_INT LoadTexture(RAVEN_INT width, RAVEN_INT height, RAVEN_BYTE* data, RAVEN_INT numComps) = 0;

        virtual void Initialize() = 0;
        virtual void RenderFrame() = 0;

        [[nodiscard]] RAVEN_U_INT GetViewportID () const  { return _viewportID; };
    };


    class OpenGLRenderer final : public IRenderer {
        void RenderMesh(RenderData &renderData, Scene::TransformData &transformData) override;

        std::vector<std::pair<RenderData*, Scene::TransformData*>> _renderData {};
        std::vector<std::pair<RenderData*, Scene::TransformData*>> _transparentRenderData {};
        LightData *_lights[MAX_LIGHTS] {};
        RAVEN_INT _numLights {};

        CameraData* _cameraData {};
        Scene::TransformData* _camTransform {};

        GLShader _mainShader {};

        static void LoadBuffers(RenderData *data);

    public:
        explicit OpenGLRenderer(const RAVEN_U_INT viewportID) : IRenderer(viewportID)  {}
        ~OpenGLRenderer() override {
            RAVEN_LOG_INFO("OpenGL Renderer Shut Down");
        }

        void ReleaseRenderData(RenderData *data) override;

        void QueueForRender(RenderData *data, Scene::TransformData *transform) override;
        void AddLight(LightData *data) override;
        void SetActiveCam(CameraData *data, Scene::TransformData *camTransform) override;
        RAVEN_U_INT LoadTexture(RAVEN_INT width, RAVEN_INT height, RAVEN_BYTE* data, RAVEN_INT numComps) override;
        void Initialize() override;
        void RenderFrame() override;
    };
}



#endif //IRENDERER_H
