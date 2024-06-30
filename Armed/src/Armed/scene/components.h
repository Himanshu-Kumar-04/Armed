#pragma once
#include <glm/glm.hpp>
#include "sceneCamera.h"

namespace Arm {
    struct TagComponent {
        std::string tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& _tag)
            : tag(_tag) {}
    };

    struct TransformComponent {
        glm::mat4 transform = glm::mat4(1.0f);

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& _transform)
            :transform(_transform) {}

        operator glm::mat4& () { return transform; }
        operator const glm::mat4& () const { return transform; }
    };

    struct SpriteRendererComponent {
        glm::vec4 color = glm::vec4(1.0f);

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& _color)
            :color(_color) {}
    };

    struct CameraComponent {
        SceneCamera camera;
        bool isPrimary = true;
        bool hasFixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };
    
    class ScriptableEntity;

    struct NativeScriptComponent {
        ScriptableEntity* instance = nullptr;

        ScriptableEntity* (*instiateScript)();
        void(*destroyScript)(NativeScriptComponent*);
        
        template<typename T>
        void bind() {
            instiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
            destroyScript = [](NativeScriptComponent* nsc) {delete (T*)nsc->instance; nsc->instance = nullptr; };
        }
    };
}