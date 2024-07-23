#pragma once
#include "scene.h"
#include "map"

namespace Arm {
    class SceneLibrary {
    public:
        SceneLibrary() = default;

        Ref<Scene> createNewScene(const std::string& name = "Untitled");

        std::map<std::string, Ref<Scene>>& get() { return m_Scenes; }

        Ref<Scene> getScene(const std::string& name) { return m_Scenes[name]; }
        Ref<Scene> getActiveScene() { return m_ActiveScene; }
        void setActiveScene(Ref<Scene> scene) { m_ActiveScene = scene; }
        void clearScenes() { m_Scenes.clear(); }
        void removeScene(Ref<Scene> scene);

    private:
        Ref<Scene> m_ActiveScene;
        std::map<std::string, Ref<Scene>> m_Scenes;
    };
}