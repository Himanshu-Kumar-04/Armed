#pragma once
#include<armed.h>
#include"Armed/core/core.h"

namespace Arm {
    class SceneHierarchyPanal{
    public:
        SceneHierarchyPanal() = default;
        SceneHierarchyPanal(const Ref<Scene>& context);
        void SetContext(const Ref<Scene>& context);
        void onImGuiRender();
    private:
        void drawEntityNode(Entity& entity);
        void drawComponents(Entity& entity);

        template<typename T>
        void displayAddComponentEntry(const std::string& tag);
    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
}