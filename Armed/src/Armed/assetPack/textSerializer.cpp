#include "ArmPCH.h"
#include "textSerializer.h"
#include "Armed/scene/sceneSerializer.h"

#include <fstream>

Arm::TextSerializer::TextSerializer(const std::filesystem::path& filePath)
    :m_FilePath(filePath)
{
}

Arm::TextSerializer::~TextSerializer()
{
}

static bool isComponentPresent(const std::string& component, std::vector<std::string> list) {

    return (std::find(list.begin(), list.end(), component) != list.end()) ? true : false;
}

void Arm::TextSerializer::serializeAssets(const Ref<Scene> scene)
{
    SceneSerializer serializer(scene);
    AssetPack scenePack = serializer.serialize();

    std::ofstream stream(m_FilePath);
    stream << scenePack.validationToken << '\n';
    stream << "version " << (int)scenePack.version << '\n';
    
    stream << "Scenes - [";
    for (auto scene : scenePack.sceneMap)
        stream << scene.first << ", ";
    stream << "]\n";

    for (auto& scene : scenePack.sceneMap) {
        stream << scene.first << " - [";
        for ( auto entityID : scene.second)
            stream << entityID << ", ";
        stream << "]\n";
    }


    ////////////////////////////////////////////////////////
    ////Entity
    ////////////////////////////////////////////////////////

    

    stream << "\n\n";
    for (auto& pair : scenePack.entityMap) {
        AssetPack::ComponentBlock componentBlock;
        componentBlock = pair.second;
        
        if (isComponentPresent("Tag", componentBlock.componentsPresent)) {
            stream
                << componentBlock.tag << "  " << pair.first << '\n';
        }

        stream << "Components - [";
        for (std::string& c : componentBlock.componentsPresent)
            stream << c << ',';
        stream << "]\n";

        if (isComponentPresent("Transform", componentBlock.componentsPresent)) {
            stream
                << "Position - ["
                << componentBlock.position.x << ", "
                << componentBlock.position.y << ", "
                << componentBlock.position.z << "]\n" 
                << "Rotation - ["
                << componentBlock.rotation.x << ", "
                << componentBlock.rotation.y << ", "
                << componentBlock.rotation.z << "]\n"
                << "Scale    - ["
                << componentBlock.scale.x << ", "
                << componentBlock.scale.y << ", "
                << componentBlock.scale.z << "]\n";
        }

        if (isComponentPresent("Camera", componentBlock.componentsPresent)) {
            stream << "IsPrimary          - " << ((componentBlock.isPrimary) ? "true" : "false") << '\n';
            stream << "HasFixedAspectRaio - " << ((componentBlock.hasFixedAspectRatio) ? "true" : "false") << '\n';
            stream << "ProjectionType     - " << ((componentBlock.hasFixedAspectRatio) ? "true" : "false") << '\n';

            stream
                << "OrthographicSize        - " << componentBlock.orthographicSize << '\n'
                << "OrthographicNear        - " << componentBlock.orthographicNear << '\n'
                << "OrthographicFar         - " << componentBlock.orthographicFar << '\n'
                << "PerspectiveVerticalFOV  - " << componentBlock.perspectiveVerticalFOV << '\n'
                << "PerspectiveNear         - " << componentBlock.perspectiveNear << '\n'
                << "PerspectiveFar          - " << componentBlock.perspectiveFar << '\n';

        }

        if (isComponentPresent("Sprite", componentBlock.componentsPresent)) {
            stream
                << "Color - ["
                << componentBlock.color.r << ','
                << componentBlock.color.g << ','
                << componentBlock.color.b << ','
                << componentBlock.color.a << "]\n";
        }

        if (isComponentPresent("Mesh", componentBlock.componentsPresent)) {
        }

        stream << "\n\n\n";
    }
}
