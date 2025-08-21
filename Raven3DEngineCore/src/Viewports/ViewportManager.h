//
// Created by wesley on 8/20/2025.
//

#ifndef RAVEN3DENGINECORE_VIEWPORTMANAGER_H
#define RAVEN3DENGINECORE_VIEWPORTMANAGER_H


namespace Raven3DEngineCore::Viewports {

    struct Viewport {
        RAVEN_U_INT width{}, height{};
        RAVEN_INT x_offset{}, y_offset{};
        RAVEN_FLOAT clearColor[4] {0.1, 0.3, 0.5, 1.0}; //rgba 0.0 -> 1.0
        RAVEN_FLOAT borderColor[4] {0.0, 0.0, 0.0, 1.0};
        RAVEN_U_INT borderWidth = 0;
        Rendering::RenderAPI renderAPI;
    };

    class ViewportManager {
        std::map<RAVEN_U_INT, Viewport> _viewports {};
        RAVEN_U_INT _nextID {0};
    public:

        RAVEN_U_INT AddViewport(Viewport vp) {
            const RAVEN_U_INT newID = _nextID++;
            _viewports.emplace(newID, vp);
            return newID;
        }

        bool RemoveViewport(const RAVEN_U_INT id) {
            if (!_viewports.contains(id)) {
                return false;
            }
            _viewports.erase(id);
            return true;
        }

        bool ResizeViewport(const RAVEN_U_INT id, const RAVEN_U_INT width, const RAVEN_U_INT height) {
            if (!_viewports.contains(id)) {
                return false;
            }
            _viewports[id].width = width;
            _viewports[id].height = height;
            return true;
        }

        bool MoveViewport(const RAVEN_U_INT id, const RAVEN_INT x, const RAVEN_INT y) {
            if (!_viewports.contains(id)) {
                return false;
            }
            _viewports[id].x_offset += x;
            _viewports[id].y_offset += y;
            return true;
        }

        bool SetViewportOffset(const RAVEN_U_INT id, RAVEN_INT x, RAVEN_INT y) {
            if (!_viewports.contains(id)) {
                return false;
            }
            _viewports[id].x_offset = x;
            _viewports[id].y_offset = y;
            return true;
        }

        Viewport *GetViewport(const RAVEN_U_INT id) {
            if (!_viewports.contains(id)) {
                return nullptr;
            }
            return &_viewports[id];
        }
    };

    extern ViewportManager *globalViewportManager;
}


#endif //RAVEN3DENGINECORE_VIEWPORTMANAGER_H