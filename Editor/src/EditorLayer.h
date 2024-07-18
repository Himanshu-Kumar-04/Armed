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
        Ref<FrameBuffer> m_FrameBuffer;
        Ref<Scene> m_Scene;
        std::vector<Ref<Scene>> m_Scenes;

        //panals.......
        SceneHierarchyPanal m_SceneHierarchyPanal;

        std::string m_AssetFilePath = "assetPack.armd";
        AssetPack m_AssetPack;

        Entity m_CameraEntity;
        float t = 0.0f;
        glm::vec2 m_ViewportSize;
    };
}