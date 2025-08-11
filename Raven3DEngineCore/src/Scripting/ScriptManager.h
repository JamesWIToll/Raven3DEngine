//
// Created by wesley on 8/10/25.
//

#ifndef RAVEN3DENGINECORE_SCRIPTMANAGER_H
#define RAVEN3DENGINECORE_SCRIPTMANAGER_H

namespace Raven3DEngineCore::Scripting {

    enum class ScriptType {
        LUA,
    };

    class IScriptManager {
    protected:
        SceneManager* _sceneManager = nullptr;
        explicit IScriptManager(SceneManager* sceneManager) : _sceneManager(sceneManager) {}
    public:
        virtual ~IScriptManager() = default;

        virtual ScriptType getScriptType() = 0;

        virtual void RunOnEvent(const Events::Event& event, float deltaTime) = 0;
    };


    class LuaScriptManager final : public IScriptManager {
        lua_State* _luaState = nullptr;
    public:
        ~LuaScriptManager() override {
            lua_close(_luaState);
            _luaState = nullptr;
        }

        explicit LuaScriptManager(SceneManager* sceneManager);

        ScriptType getScriptType() override { return ScriptType::LUA; }

        void RunOnEvent(const Events::Event& event, float deltaTime) override;
    };

}

#endif //RAVEN3DENGINECORE_SCRIPTMANAGER_H