//
// Created by wesley on 8/10/25.
//
#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Scripting;



LuaScriptManager::LuaScriptManager(SceneManager *sceneManager) : IScriptManager(sceneManager) {
    _luaState = luaL_newstate();
    luaL_openselectedlibs(_luaState, LUA_GLIBK
                                        | LUA_STRLIBK
                                        | LUA_COLIBK
                                        | LUA_TABLIBK
                                        | LUA_MATHLIBK
                                        | LUA_DBLIBK
                                        | LUA_UTF8LIBK , 0 );
}


void LuaScriptManager::RunOnEvent(const Events::Event &event, RAVEN_FLOAT deltaTime) {
    
}
