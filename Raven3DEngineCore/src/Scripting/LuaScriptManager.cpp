//
// Created by wesley on 8/10/25.
//
#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Scripting;


void LuaScriptManager::Initialize() {
    _eventHandler->RegisterEventListener(Events::EventType::AppUpdate,              [this] (const Events::Event & event) { RunOnEvent(event); }, _eventListenerId);
    _eventHandler->RegisterEventListener(Events::EventType::GamePadAxisMoved,       [this] (const Events::Event & event) { RunOnEvent(event); }, _eventListenerId);
    _eventHandler->RegisterEventListener(Events::EventType::GamePadButtonPressed,   [this] (const Events::Event & event) { RunOnEvent(event); }, _eventListenerId);
    _eventHandler->RegisterEventListener(Events::EventType::GamePadButtonReleased,  [this] (const Events::Event & event) { RunOnEvent(event); }, _eventListenerId);
    _eventHandler->RegisterEventListener(Events::EventType::KeyPressed,             [this] (const Events::Event & event) { RunOnEvent(event); }, _eventListenerId);
    _eventHandler->RegisterEventListener(Events::EventType::KeyReleased,            [this] (const Events::Event & event) { RunOnEvent(event); }, _eventListenerId);
    _eventHandler->RegisterEventListener(Events::EventType::MouseButtonPressed,     [this] (const Events::Event & event) { RunOnEvent(event); }, _eventListenerId);
    _eventHandler->RegisterEventListener(Events::EventType::MouseButtonReleased,    [this] (const Events::Event & event) { RunOnEvent(event); }, _eventListenerId);
    _eventHandler->RegisterEventListener(Events::EventType::MouseMoved,             [this] (const Events::Event & event) { RunOnEvent(event); }, _eventListenerId);
    _eventHandler->RegisterEventListener(Events::EventType::MouseScrolled,          [this] (const Events::Event & event) { RunOnEvent(event); }, _eventListenerId);
}

LuaScriptManager::LuaScriptManager(Scene::SceneManager *sceneManager) : IScriptManager(sceneManager) {
    _luaState = luaL_newstate();
    luaL_openselectedlibs(_luaState, LUA_GLIBK
                                        | LUA_STRLIBK
                                        | LUA_COLIBK
                                        | LUA_TABLIBK
                                        | LUA_MATHLIBK
                                        | LUA_DBLIBK
                                        | LUA_UTF8LIBK , 0 );
}


void LuaScriptManager::RunOnEvent(const Events::Event &event) {
    
}
