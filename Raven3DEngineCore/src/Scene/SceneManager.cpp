//
// Created by wesley on 8/10/25.
//

#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Scene;

Entity_T Raven3DEngineCore::Scene::NullEntity = static_cast<Entity_T>(RAVEN_ENTITY_NULL);

SceneManager::SceneManager(RAVEN_U_INT vpID): _viewportId(vpID) {
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
    _registry.emplace<TransformData3D>(_root, TransformData3D{
        .localTransform {1.0f },
        .worldTransform {1.0f }
    });
}

SceneManager::~SceneManager(){
    _registry.clear();
}

void SceneManager::Initialize() {
    _eventHandler->RegisterEventListener(Events::EventType::AppUpdate, [this] (const Events::Event &) { Update(); });
    _eventHandler->RegisterEventListener(Events::EventType::AppPreRender, [this] (const Events::Event &event) {
        const auto preRenderEvent = dynamic_cast<const Events::AppPreRenderEvent&>(event);
        const auto vp = Viewports::globalViewportManager->GetViewport(_viewportId);
        if (vp != nullptr && vp->renderer != nullptr) {
            ProcessRenderables(Viewports::globalViewportManager->GetViewport(_viewportId)->renderer);
        }
    });
    _eventHandler->RegisterEventListener(Events::EventType::ViewportTearDown, [this] (const Events::Event &e) {
        if (const auto vpEvent = dynamic_cast<const Events::VPTearDownEvent&>(e); vpEvent.GetViewportID() == _viewportId) {
            for (const auto renderView = _registry.view<Rendering::RenderData3D>(); const auto [entity, data] : renderView.each()) {
                if (const auto vp = Viewports::globalViewportManager->GetViewport(_viewportId); vp != nullptr && vp->renderer != nullptr) {
                    vp->renderer->ReleaseRenderData(&data);
                }
            }
        }
    });
}

void SceneManager::Update() {
    const auto transformView = _registry.view<RelationshipData, TransformData3D>();

    std::function<void(Entity_T)> UpdateChildTransform = [&](const Entity_T entity) {
        if (!transformView.contains(entity)) {
            RAVEN_LOG_ERROR("Entity {} does not have both a transform and relationship component", static_cast<RAVEN_ENTITY_TYPE>(entity));
            return;
        }
        auto [relData, transform] = transformView.get(entity);
        for (const auto children = relData.children; const auto child : children) {
            auto [childRelData, childTransform] = transformView.get(child);
            childTransform.UpdateWorldTransform(transform.worldTransform);
            UpdateChildTransform(child);
        }

    };

    UpdateChildTransform(_root);
}

void SceneManager::ProcessRenderables(Rendering::IRenderer *renderer) {
    for (const auto renderView = _registry.view<Rendering::RenderData3D, TransformData3D>(); const auto [entity, renderData, transform] : renderView.each()) {
        renderer->QueueForRender(&renderData, &transform);
    }

    for (const auto lightView = _registry.view<Rendering::LightData3D, TransformData3D>(); const auto [entity, lightData, transform] : lightView.each()) {
        if (lightData.type == Rendering::LightType::Point) {
            lightData.posDir = glm::vec3(transform.worldTransform[3]);
        }
        renderer->AddLight(&lightData);
    }

    bool foundCurrent = false;
    for (const auto camView = _registry.view<Rendering::CameraData3D, TransformData3D>(); auto [entity, camData, transform] : camView.each()) {
        camData.UpdateVectors(transform.worldTransform);
        if (camData.current && !foundCurrent) {
            foundCurrent = true;
            renderer->SetActiveCam(&camData, &transform);
        } else if (camData.current) {
            camData.current = false;
        }
    }
}

Entity_T SceneManager::CreateEntity(const EntityMetaData& data, const Entity_T parent) {
    auto parent_entity = parent;
    if (parent == NullEntity) {
        parent_entity = _root;
    }
    const Entity_T entity = _registry.create();
    EntityMetaData metaData {data};
    metaData.id = entity;
    _registry.emplace<EntityMetaData>(entity, metaData);
    _registry.emplace<RelationshipData>(entity, RelationshipData{
        .root = _root,
        .parent = parent_entity,
        .children = {}
    });
    this->GetComponent<RelationshipData>(parent_entity)->children.emplace_back(entity);
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
    if (const auto renderData = _registry.try_get<Rendering::RenderData3D>(entity); renderData != nullptr) {
        Viewports::globalViewportManager->GetViewport(_viewportId)->renderer->ReleaseRenderData(renderData);
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

    auto relData = _registry.try_get<RelationshipData>(entity);
    auto currParentRelData = _registry.try_get<RelationshipData>(relData->parent);
    auto newParentRelData = _registry.try_get<RelationshipData>(parent);

    for (auto i=0; i < currParentRelData->children.size(); i++) {
        if (currParentRelData->children[i] == entity) {
            currParentRelData->children.erase(currParentRelData->children.begin() + i);
            break;
        }
    }

    relData->parent = parent;
    newParentRelData->children.push_back(entity);
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

void SceneManager::PrintSceneGraph() const {
    auto view = _registry.view<RelationshipData, EntityMetaData, TransformData3D>();

    for (auto [entity, relData, metaData, transformData] : view.each()) {
        std::string children;
        for (const auto child : relData.children) {
            children += std::to_string(static_cast<RAVEN_ENTITY_TYPE>(child)) + ", ";
        }
        RAVEN_LOG_DEBUG("Entity: {} - Name: {} - Local Translation: {}, {}, {} - children: {}",
            static_cast<RAVEN_INT>(entity), metaData.name,
            transformData.localTransform[3][0], transformData.localTransform[3][1], transformData.localTransform[3][2],
            children);
    }

}
