#include "ArmPCH.h"
#include "sceneLibrary.h"

namespace Arm {
    Ref<Scene> SceneLibrary::createNewScene(const std::string& name)
    {
        if (m_Scenes.count(name)) {
            size_t suffix = 1;
            std::string newName;
            do
            {
                std::ostringstream uniqueName("");
                uniqueName << name.substr(0, name.find_last_of('(')) << "(" << suffix << ")";
                suffix++;
                newName = uniqueName.str();
            } while (m_Scenes.count(newName));

            m_Scenes[newName] = CreateRef<Scene>(newName);
            if (m_Scenes.size() == 1)
                m_ActiveScene = m_Scenes[newName];
            return m_Scenes[newName];
        }
        else {
            m_Scenes[name] = CreateRef<Scene>(name);
            if (m_Scenes.size() == 1)
                m_ActiveScene = m_Scenes[name];
            return m_Scenes[name];
        }

    }

    void SceneLibrary::removeScene(Ref<Scene> scene)
    {
        m_Scenes.erase(scene->getSceneName());
    }
}