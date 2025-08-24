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

        virtual void RenderMesh(RenderData3D &renderData, Scene::TransformData3D &transformData) = 0;
    public:
        explicit IRenderer(const RAVEN_U_INT viewportID) : _viewportID(viewportID) {}

        virtual ~IRenderer() override = default;

        virtual void ReleaseRenderData(RenderData3D *data) = 0;

        virtual void QueueForRender(RenderData3D *data, Scene::TransformData3D *transform) = 0;
        virtual void AddLight(LightData3D *data) = 0;
        virtual void SetActiveCam(CameraData3D *data, Scene::TransformData3D *camTransform) = 0;
        virtual RAVEN_U_INT LoadTexture(RAVEN_INT width, RAVEN_INT height, RAVEN_BYTE* data, RAVEN_INT numComps) = 0;

        virtual void Initialize() = 0;
        virtual void RenderFrame() = 0;

        [[nodiscard]] RAVEN_U_INT GetViewport1ID () const  { return _viewportID; };
    };

}



#endif //IRENDERER_H
