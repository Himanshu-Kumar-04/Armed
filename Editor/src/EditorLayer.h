#pragma once
#include<armed.h>
#include "panel/sceneHierarchyPanal.h"

namespace Arm {
    class EditorLayer : public Layer {
    public:
        EditorLayer();
        ~EditorLayer() = default;
        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onUpdate(Timestep ts) override;
        virtual void onImGuiRender() override;
        virtual void onEvent(Event& e) override;
    private:
        Ref<Shader> m_Shader;
        Ref<FrameBuffer> m_FrameBuffer;
        Ref<Texture2D> m_SpritesSheet;
        Ref<Scene> m_Scene;

        //panals.......
        SceneHierarchyPanal m_SceneHierarchyPanal;

        Entity m_CameraEntity;
        float t = 0.0f;
        glm::vec2 m_ViewportSize;
    };
}