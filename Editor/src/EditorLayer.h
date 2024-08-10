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
        bool onKeyPressed(KeyPressedEvent& e);
        void newFile();
        void openFile();
        void saveFile();
    private:
        Ref<FrameBuffer> m_FrameBuffer;
        Ref<Scene> m_ActiveScene;
        Ref<SceneLibrary> m_Scenes = CreateRef<SceneLibrary>();
        Entity m_EditorCamera;
        AssetPack m_AssetPack;

        //panals.......
        SceneHierarchyPanal m_SceneHierarchyPanal;


        glm::vec2 m_ViewportSize = { 400.0f,400.0f };
        int m_GuizmoType = -1;
    };
}