//
// Created by wesley on 8/20/2025.
//

#ifndef RAVEN3DENGINECORE_VIEWPORTMANAGER_H
#define RAVEN3DENGINECORE_VIEWPORTMANAGER_H
#include "Window/SDLWindow.h"


namespace Raven3DEngineCore::Viewports {

    struct Viewport {
        RAVEN_U_INT width{}, height{};
        RAVEN_INT x_offset{}, y_offset{};
        RAVEN_FLOAT clearColor[4] {0.1, 0.3, 0.5, 1.0}; //rgba 0.0 -> 1.0
        RAVEN_FLOAT borderColor[4] {0.0, 0.0, 0.0, 1.0};
        RAVEN_U_INT borderWidth = 0;
        Rendering::RenderAPI renderAPI;
        Rendering::IRenderer* renderer;
        Window::IRenderWindow* window;
    };

    class ViewportManager final : public Events::EventNotifier {
        std::map<RAVEN_U_INT, Viewport> _viewports {};
        RAVEN_U_INT _nextID {0};
    public:
        ~ViewportManager() override {
            for (const auto id: _viewports | std::views::keys) {
                RemoveViewport(id);
            }
            _viewports.clear();
        }

        RAVEN_U_INT AddViewport(Viewport vp) {
            const RAVEN_U_INT newID = _nextID++;

            if (vp.renderAPI == Rendering::RenderAPI::OPENGL) {
                vp.renderer = new Rendering::OpenGLRenderer(newID);
            }
            _viewports.emplace(newID, vp);

            if (vp.renderer == nullptr || vp.window == nullptr) {
                RAVEN_LOG_FATAL("Could not setup a renderer or window for VP: {}", newID);
                return newID;
            }
            vp.renderer->SetEventHandler(vp.window->GetEventHandler());
            vp.renderer->Initialize();
            return newID;
        }

        bool HasViewports() const {
            return !_viewports.empty();
        }

        bool CloseWindow(const Window::IRenderWindow *window) {
            bool foundWindow = false;
            for (auto [id, vp] : _viewports) {
                if (vp.window == window) {
                    foundWindow = true;
                    RemoveViewport(id);
                }
            }
            return foundWindow;
        }

        bool RemoveViewport(const RAVEN_U_INT id) {
            if (!_viewports.contains(id)) {
                return false;
            }
            _eventHandler->Notify(Events::VPTearDownEvent(id));

            delete _viewports[id].renderer;

            bool foundOtherVPWithSameWindow = false;
            for (auto [otherID, otherVP] : _viewports) {
                if (otherID != id && otherVP.window == _viewports[id].window) {
                    foundOtherVPWithSameWindow = true;
                    break;
                }
            }

            if (!foundOtherVPWithSameWindow) {
                delete _viewports[id].window;
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

        std::vector<RAVEN_U_INT> GetViewportsForWindow(const Window::IRenderWindow *window) {
            std::vector<RAVEN_U_INT> viewports;
            for (auto [id, viewport] : _viewports) {
                if (viewport.window == window) {
                    viewports.push_back(id);
                }
            }
            return viewports;
        }

        Window::IRenderWindow *GetWindow(const RAVEN_U_INT id) {
            if (!_viewports.contains(id)) {
                return nullptr;
            }
            return _viewports[id].window;
        }

        Window::IRenderWindow *MoveToNewWindow(const RAVEN_U_INT id) {
            if (!_viewports.contains(id)) {
                return nullptr;
            }
            auto vp = GetViewport(id);
            auto *window = new Window::SDLWindow();

        }
    };

    extern ViewportManager *globalViewportManager;
}


#endif //RAVEN3DENGINECORE_VIEWPORTMANAGER_H