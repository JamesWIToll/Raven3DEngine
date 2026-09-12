//
// Created by wesley on 9/10/2026.
//

#ifndef RAVEN3DENGINECORE_RAVENFORWARD_H
#define RAVEN3DENGINECORE_RAVENFORWARD_H

#include <RavenDefs.h>
#include "../src/Logging/Log.h"
#include "../src/Events/AppClock.h"
#include "../src/Rendering/RenderAPIs.h"
#include "../src/Input/GamepadCode.h"
#include "../src/Input/KeyCode.h"
#include "../src/Input/MouseCode.h"
#include "../src/Input/InputDeviceRegistry.h"
#include "../src/Scene/Components/CoreComponents.h"
#include "../src/Scene/Components/RenderComponents.h"

namespace Raven3DEngineCore {
    class RavenApp;

    namespace Window {
        class IRenderWindow;
    }

    namespace Events {
        class Event;
        class EventHandler;
    }

    namespace Scripting {
        class IScriptManager;
    }

    namespace Scene {
        extern Entity_T NullEntity;
        class SceneManager;
    }

    namespace Importer {
        class IImporter;
    }

    namespace Input {
        class DeviceInfo;
    }

    namespace Rendering {
        class TextureData;
        class IRenderer;
        class GLShader;
    }

    namespace Viewports {
        class ViewportManager;
        extern ViewportManager *globalViewportManager;
    }
}


#endif //RAVEN3DENGINECORE_RAVENFORWARD_H
