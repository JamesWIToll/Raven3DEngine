//
// Created by wesley on 8/10/25.
//

#ifndef RAVEN3DENGINECORE_SCRIPTMANAGER_H
#define RAVEN3DENGINECORE_SCRIPTMANAGER_H

namespace Raven3DEngineCore::Scripting {

    enum class ScriptType {
        LUA,
    };

    class IScriptManager : public Events::EventNotifier {
    protected:
        Scene::SceneManager* _sceneManager = nullptr;
        explicit IScriptManager(Scene::SceneManager* sceneManager) : _sceneManager(sceneManager) {}
    public:
        virtual ~IScriptManager() = default;

        virtual void Initialize() = 0;
        virtual ScriptType GetScriptType() = 0;

        virtual void RunOnEvent(const Events::Event& event) = 0;
    };

    class LuaScriptManager final : public IScriptManager {
        lua_State* _luaState = nullptr;
    public:
        ~LuaScriptManager() override {
            lua_close(_luaState);
            _luaState = nullptr;
        }

        void Initialize() override;

        explicit LuaScriptManager(Scene::SceneManager* sceneManager);

        ScriptType GetScriptType() override { return ScriptType::LUA; }

        void RunOnEvent(const Events::Event& event) override;
    };

}

#endif //RAVEN3DENGINECORE_SCRIPTMANAGER_H