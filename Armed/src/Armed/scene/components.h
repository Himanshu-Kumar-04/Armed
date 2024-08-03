#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Armed/core/UUID.h"
#include "sceneCamera.h"
#include "Armed/renderer/mesh.h"

namespace Arm {
    
    struct IDComponent
    {
        UUID ID;
        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
        IDComponent(const UUID& _UUID)
            :ID(_UUID) {}
    };

    struct TagComponent {
        std::string tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& _tag)
            : tag(_tag) {}
    };

    struct TransformComponent {
        glm::vec3 translation = { 0.0f,0.0f,0.0f };
        glm::vec3 rotation = { 0.0f,0.0f,0.0f };
        glm::vec3 scale = { 1.0f,1.0f,1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& _translation)
            :translation(_translation) {}

        glm::mat4 getTransform() const {
            return glm::translate(glm::mat4(1.0f), translation)
                
                * glm::rotate(glm::mat4(1.0f), rotation.x, { 1,0,0 })
                * glm::rotate(glm::mat4(1.0f), rotation.y, { 0,1,0 })
                * glm::rotate(glm::mat4(1.0f), rotation.z, { 0,0,1 })
                
                * glm::scale(glm::mat4(1.0f), scale);
        }
    };

    struct MeshComponent {
        Mesh mesh;
        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;
        MeshComponent(const Mesh& _mesh)
            :mesh(_mesh) {}

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