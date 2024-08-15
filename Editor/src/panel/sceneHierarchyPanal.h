#pragma once
#include<armed.h>
#include"Armed/core/core.h"


namespace Arm {
    class SceneHierarchyPanal{
    public:
        SceneHierarchyPanal() = default;
        SceneHierarchyPanal(const Ref<Scene>& context);
        void setContext(const Ref<Scene>& context);
        Ref<Scene> getContext() { return m_Context; }
        Entity getSelectedEntity() const { return m_SelectionContext; }
        void onImGuiRender(Ref<SceneLibrary> scenes, AssetPack& assetPack,  Entity& selectedEntity);
    private:
        void drawSceneExplorerMenu(Ref<SceneLibrary> scenes, AssetPack& assetPack);
        void drawEntityNode(Entity& entity);
        void drawComponents(Entity& entity);

        template<typename T>
        void displayAddComponentEntry(const std::string& tag);
    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
}