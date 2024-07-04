#include "ArmPCH.h"
#include "mesh.h"

Arm::Mesh::Mesh(const std::string& file)
{
}

Arm::Mesh::Mesh(std::vector<VertexData>& vertices, std::vector<uint32_t>& indices, std::vector<Ref<Texture2D>>& textures)
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

Arm::Mesh::Mesh(std::vector<VertexData>& vertices, std::vector<uint32_t>& indices)
{
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

    Ref<IndexBuffer> ib = IndexBuffer::Create(indices.data(), indices.size());
    m_VertexBuffer->setIndexBuffer(ib);
}

void Arm::Mesh::updateTransform(const glm::mat4& transform)
{
    for (auto& vertex : vertices) {
        vertex.position = transform * glm::vec4{vertex.position, 1.0f};
    }
}