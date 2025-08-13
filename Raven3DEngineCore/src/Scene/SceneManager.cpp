//
// Created by wesley on 8/10/25.
//

#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Scene;

// Declared in CoreComponents.h
// ReSharper disable once CppUseAuto
Entity_T Raven3DEngineCore::Scene::NullEntity = static_cast<Entity_T>(RAVEN_ENTITY_NULL);

SceneManager::SceneManager() {
    _registry = entt::basic_registry<Entity_T>();
    _root = _registry.create();
    _registry.emplace<RelationshipData>(_root, RelationshipData{
        .root = _root,
        .parent = NullEntity,
        .children = {}
    });
    _registry.emplace<EntityMetaData>(_root, EntityMetaData{
        .id = _root,
        .name = "rootEntity"
    });
    _registry.emplace<TransformData>(_root, TransformData{
        .localTransform {
            glm::vec3(0.0f),
            glm::quat(),
            glm::vec3(1.0f)
        },
        .worldTransform {
            glm::vec3(0.0f),
            glm::quat(),
            glm::vec3(1.0f)
        }
    });
}

SceneManager::~SceneManager(){
    _registry.clear();
}

void SceneManager::Initialize() {
    _eventHandler->RegisterEventListener(Events::EventType::AppUpdate, [this] (const Events::Event &) { Update(); });
}

void SceneManager::Update() {
    const auto transformView = _registry.view<RelationshipData, TransformData>();

    std::function<void(Entity_T)> UpdateChildTransform = [&](const Entity_T entity) {
        if (!transformView.contains(entity)) {
            RAVEN_LOG_ERROR("Entity {} does not have both a transform and relationship component", static_cast<RAVEN_ENTITY_TYPE>(entity));
            return;
        }
        auto [relData, transform] = transformView.get(entity);
        for (const auto children = relData.children; const auto child : children) {
            auto [childRelData, childTransform] = transformView.get(child);
            childTransform.TransformByMat(transform.GetMatrix());
            UpdateChildTransform(child);
        }

    };

    UpdateChildTransform(_root);
}

void SceneManager::ProcessRenderables(Rendering::IRenderer *renderer) {
    for (const auto renderView = _registry.view<Rendering::RenderData, TransformData>(); const auto [entity, renderData, transform] : renderView.each()) {
        renderer->QueueForRender(&renderData, &transform);
    }
}

Entity_T SceneManager::CreateEntity(const EntityMetaData& data, const Entity_T parent) {
    const Entity_T entity = _registry.create();
    EntityMetaData metaData {data};
    metaData.id = entity;
    _registry.emplace<EntityMetaData>(entity, metaData);
    _registry.emplace<RelationshipData>(entity, RelationshipData{
        .root = _root,
        .parent = parent == NullEntity ? _root : parent,
        .children = {}
    });
    return entity;
}

bool SceneManager::DestroyEntity(const Entity_T entity) {
    if (entity == _root) {
        RAVEN_LOG_ERROR("Cannot destroy root entity of a scene");
        return false;
    }
    const auto relData = _registry.get<RelationshipData>(entity);
    for (const auto child : relData.children) {
        auto childRelData = _registry.get<RelationshipData>(child);
        childRelData.parent = relData.parent;
    }
    auto parentRelData = _registry.get<RelationshipData>(relData.parent);
    for (auto i = 0; i < parentRelData.children.size(); i++) {
        if (parentRelData.children[i] == entity) {
            parentRelData.children.erase(parentRelData.children.begin() + i);
        }
    }
    _registry.destroy(entity);
    return true;
}

Entity_T SceneManager::GetParentEntity(const Entity_T entity) {
    return _registry.get<RelationshipData>(entity).parent;
}

bool SceneManager::ReparentEntity(const Entity_T entity, const Entity_T parent) {
    if (entity == _root) {
        RAVEN_LOG_ERROR("Cannot reparent root entity of a scene");
        return false;
    }

    auto relData = _registry.get<RelationshipData>(entity);
    auto currParentRelData = _registry.get<RelationshipData>(parent);
    auto newParentRelData = _registry.get<RelationshipData>(parent);

    for (auto i=0; i < currParentRelData.children.size(); i++) {
        if (currParentRelData.children[i] == entity) {
            currParentRelData.children.erase(currParentRelData.children.begin() + i);
            break;
        }
    }

    relData.parent = parent;
    newParentRelData.children.push_back(entity);
    return true;
}

bool SceneManager::AddChildEntity(const Entity_T entity, const Entity_T child) {
    auto relData = _registry.get<RelationshipData>(entity);

    for (const auto _child: relData.children) {
        if (_child == child) {
            RAVEN_LOG_ERROR("Entity {} already has child {}", static_cast<RAVEN_ENTITY_TYPE>(entity), static_cast<RAVEN_ENTITY_TYPE>(child));
            return false;
        }
    }

    auto childRelData = _registry.get<RelationshipData>(child);
    childRelData.parent = entity;
    relData.children.push_back(child);
    return true;
}

template<typename T_Comp1, typename... T_Comps>
bool SceneManager::ConnectComponents(const Entity_T entity, T_Comp1 firstComponent, T_Comps... restComponents) {
    _registry.emplace<T_Comp1, T_Comps...>(entity, firstComponent, restComponents...);
    return true;
}

template<typename T_Comp1, typename... T_Comps>
bool SceneManager::DisconnectComponents(const Entity_T entity) {
    _registry.remove<T_Comp1, T_Comps...>(entity);
    return true;
}
