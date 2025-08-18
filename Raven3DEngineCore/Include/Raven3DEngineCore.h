//
// Created by wesley on 8/4/25.
//

#ifndef RAVENENGINECORE_H
#define RAVENENGINECORE_H


#include <pch.h>

#define BITSHIFT_TO_POS(x) (1 << x)

#define RAVEN_BYTE std::uint8_t
#define RAVEN_SHORT std::int16_t
#define RAVEN_INT std::int32_t
#define RAVEN_LONG std::int64_t
#define RAVEN_FLOAT std::float_t
#define RAVEN_DOUBLE std::float64_t
#define RAVEN_U_SHORT std::uint16_t
#define RAVEN_U_INT std::uint32_t
#define RAVEN_U_LONG std::uint64_t

#define RAVEN_U_INT_MAX 0xFFFFFFFF

#define RAVEN_ENTITY_TYPE RAVEN_U_INT
#define RAVEN_ENTITY_NULL RAVEN_U_INT_MAX

#ifndef RAVEN_RESOURCE_PATH
#define RAVEN_RESOURCE_PATH "../Resources/"
#endif

#include <Scene/Components/CoreComponents.h>
#include <Logging/Log.h>
#include <Input/KeyCode.h>
#include <Input/MouseCode.h>
#include <Input/GamepadCode.h>
#include <Input/InputDeviceRegistry.h>
#include <Events/Event.h>
#include <Events/MouseEvent.h>
#include <Events/WindowEvent.h>
#include <Events/KeyEvent.h>
#include <Events/GamepadEvent.h>
#include <Events/EventHandler.h>
#include <Rendering/Shader.h>
#include <Rendering/Renderer.h>
#include <Events/AppEvent.h>
#include <Scene/SceneManager.h>
#include <Importer/Importer.h>
#include <Scene/Components/CoreComponentGetters.h>
#include <Scripting/ScriptManager.h>
#include <Window/Window.h>
#include <RavenApp.h>


#endif //RAVENENGINECORE_H
