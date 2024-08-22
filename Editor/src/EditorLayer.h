#pragma once
#include<armed.h>
#include "panel/sceneExplorer.h"

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
        enum class SceneState {
            Edit = 0, Simulate = 1, Runtime = 2
        };
    private:
        AssetPack m_AssetPack;
        Ref<FrameBuffer> m_FrameBuffer;
        Ref<SceneLibrary> m_Scenes = CreateRef<SceneLibrary>();
        EditorCamera m_EditorCamera;
        Ref<Scene> m_ActiveScene;
        Entity m_SelectedEntity;
        SceneState m_SceneState = SceneState::Edit;
        //panals.......
        SceneExplorer m_SceneExplorer;


        glm::vec2 m_ViewportSize = { 400.0f,400.0f };
        int m_GuizmoType = -1;
    };
}