#pragma once
#include<vector>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include"texture.h"
#include"buffer.h"

namespace Arm {

    struct VertexData {
        glm::vec3 position;
        glm::vec4 color = glm::vec4(1.0f);
        glm::vec2 texCoord;
        float texIndex = 0.0f;
        float tilingFactor = 1.0f;
    };


    class Mesh {
    public:
        enum class Shape {
            Cube,
            Cylinder,
            Cone,
            Sphere
        };
    public:
        std::vector<VertexData> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::vector<Ref<Texture2D>> m_Textures;

        Mesh();
        Mesh(const std::string& file);
        Mesh(std::vector<VertexData>& vertices, std::vector<uint32_t>& indices, std::vector<Ref<Texture2D>>& textures);
        Mesh(std::vector<VertexData>& vertices, std::vector<uint32_t>& indices);
        void draw(const glm::mat4& transform);
    private:
        Ref<VertexBuffer> m_VertexBuffer;
    };
}