#include "ArmPCH.h"
#include "mesh.h"
#include "renderCommand.h"

namespace Arm {
    Mesh::Mesh()
    {
        std::vector<VertexData> vertices {
            VertexData(glm::vec3{-0.5f,-0.5f, 0.5f}, glm::vec4(1.0f), glm::vec2{0.0f, 0.0f}),
            VertexData(glm::vec3{ 0.5f,-0.5f, 0.5f}, glm::vec4(1.0f), glm::vec2{1.0f, 0.0f}),
            VertexData(glm::vec3{ 0.5f, 0.5f, 0.5f}, glm::vec4(1.0f), glm::vec2{1.0f, 1.0f}),
            VertexData(glm::vec3{-0.5f, 0.5f, 0.5f}, glm::vec4(1.0f), glm::vec2{0.0f, 1.0f}),
            VertexData(glm::vec3{ 0.5f,-0.5f,-0.5f}, glm::vec4(1.0f), glm::vec2{1.0f, 0.0f}),
            VertexData(glm::vec3{ 0.5f, 0.5f,-0.5f}, glm::vec4(1.0f), glm::vec2{1.0f, 1.0f}),
            VertexData(glm::vec3{-0.5f,-0.5f,-0.5f}, glm::vec4(1.0f), glm::vec2{0.0f, 0.0f}),
            VertexData(glm::vec3{-0.5f, 0.5f,-0.5f}, glm::vec4(1.0f), glm::vec2{0.0f, 1.0f})
        };
        
        std::vector<uint32_t> indices { 
            0,1,2,2,3,0,
            1,4,5,5,2,1,
            4,6,7,7,5,4,
            6,0,3,3,7,6,
            3,2,5,5,7,3,
            1,0,6,6,4,1
        };

        this->vertices = vertices;
        this->indices = indices;

        m_VertexBuffer = VertexBuffer::Create(vertices);
        m_VertexBuffer->setLayout({
                {ShaderDataType::Float3 , "position"},
                {ShaderDataType::Float4 , "color"},
                {ShaderDataType::Float2 , "texCoord"},
                {ShaderDataType::Float , "texIndex"},
                {ShaderDataType::Float , "tilingFactor"}
            });

        Ref<IndexBuffer> ib = IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size()));
        m_VertexBuffer->setIndexBuffer(ib);
    }
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

        Ref<IndexBuffer> ib = IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size()));
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

        Ref<IndexBuffer> ib = IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size()));
        m_VertexBuffer->setIndexBuffer(ib);
    }

    void Mesh::draw(const glm::mat4& transform)
    {
        std::vector<VertexData> newVertices = vertices;
        for (auto& vertex : newVertices) {
            vertex.position = transform * glm::vec4{ vertex.position, 1.0f };
        }
        for (uint8_t i = 1; i < textures.size(); i++)
            textures[i]->bind(i);
        m_VertexBuffer->setData(newVertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(VertexData)));
        RenderCommand::drawIndexed(m_VertexBuffer, static_cast<uint32_t>(indices.size()));
    }
}