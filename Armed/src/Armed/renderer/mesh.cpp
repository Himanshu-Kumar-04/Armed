#include "ArmPCH.h"
#include "mesh.h"

namespace Arm {
    Mesh::Mesh(const std::string& file)
    {
    }

    Mesh::Mesh(std::vector<VertexData>& vertices, std::vector<uint32_t>& indices, std::vector<Ref<Texture2D>>& textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        m_VertexBuffer = VertexBuffer::Create(vertices);
        m_VertexBuffer->setLayout({
                {ShaderDataType::Float3 , "position"},
                {ShaderDataType::Float4 , "color"},
                {ShaderDataType::Float2 , "texCoord"},
                {ShaderDataType::Float , "texIndex"},
                {ShaderDataType::Float , "tilingFactor"}
            });

        Ref<IndexBuffer> ib = IndexBuffer::Create(indices.data(), indices.size());
        m_VertexBuffer->setIndexBuffer(ib);
    }

    Mesh::Mesh(std::vector<VertexData>& vertices, std::vector<uint32_t>& indices)
    {
        this->vertices = vertices;
        this->indices = indices;

        m_VertexBuffer = VertexBuffer::Create((uint32_t)vertices.size() * sizeof(VertexData));
        m_VertexBuffer->setLayout({
                {ShaderDataType::Float3 , "position"},
                {ShaderDataType::Float4 , "color"},
                {ShaderDataType::Float2 , "texCoord"},
                {ShaderDataType::Float , "texIndex"},
                {ShaderDataType::Float , "tilingFactor"}
            });

        Ref<IndexBuffer> ib = IndexBuffer::Create(indices.data(), indices.size());
        m_VertexBuffer->setIndexBuffer(ib);
    }

    std::vector<VertexData> Mesh::updateTransform(const glm::mat4& transform)
    {
        std::vector<VertexData> newVertices = vertices;
        for (auto& vertex : newVertices) {
            vertex.position = transform * glm::vec4{ vertex.position, 1.0f };
        }
        return newVertices;
    }
}