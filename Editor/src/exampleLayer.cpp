#include "exampleLayer.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <Armed/renderer/OpenGL/openGLShader.h>
#include "imgui/imgui.h"
ExampleLayer::ExampleLayer()
	: Layer("example"), m_Camera(1280.0f / 720.0f, 0.1f)
{
	float squareVertices[] = {
			-0.5f ,-0.5f, 0.0f,   0.0f, 0.0f,   0.3f,0.3f,0.6f,1.0f,
			 0.5f ,-0.5f, 0.0f,   1.0f, 0.0f,   0.3f,0.3f,0.6f,1.0f,
			 0.5f , 0.5f, 0.0f,   1.0f, 1.0f,   0.3f,0.3f,0.6f,1.0f,
			-0.5f , 0.5f, 0.0f,   0.0f, 1.0f,   0.3f,0.3f,0.6f,1.0f
	};

	m_SquareVA = Arm::VertexArray::Create();
	Arm::Ref<Arm::VertexBuffer> squareVB = Arm::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->setLayout({
			{Arm::ShaderDataType::Float3 , "position"},
			{Arm::ShaderDataType::Float2 , "texCoord"},
			{Arm::ShaderDataType::Float4 , "color"}
		});
	m_SquareVA->addVertexBuffer(squareVB);
	uint32_t squareIndices[6] = { 0,1,2 ,2 ,3, 0 };
	Arm::Ref<Arm::IndexBuffer> squareIB = Arm::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->setIndexBuffer(squareIB);

	m_Shader = Arm::Shader::create("assets/shader/d_texture.glsl");
	m_Texture1 = Arm::Texture2D::Create("assets/texture/Armed.jpeg");
	m_Shader->bind();
	m_Shader->setInt("u_Texture", 0);
}

void ExampleLayer::onUpdate(Arm::Timestep ts)
{
	m_Camera.onUpdate(ts);
	Arm::RendererCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Arm::RendererCommand::clearColor();
	Arm::Renderer::beginScene(m_Camera);
	//static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	//std::dynamic_pointer_cast<Arm::OpenGLShader>(m_Shader)->setUniform3f("u_Color", m_SquareColor);
	/*for (int i = 0; i < 5; i++) {
		for(int j=0; j< 5; j++) {
			glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Arm::Renderer::submit(m_Shader, m_SquareVA, transform);
		}
	}*/
	m_Texture1->bind();
	Arm::Renderer::submit(m_Shader, m_SquareVA);
	Arm::Renderer::endScene();
}

void ExampleLayer::onEvent(Arm::Event& e)
{
	m_Camera.onEvent(e);
}

void ExampleLayer::onImGuiRender()
{
	ImGui::Begin("s");
	ImGui::Text("x: %f", m_Camera.getPosition().x);
	ImGui::Text("y: %f", m_Camera.getPosition().y);
	ImGui::Text("z: %f", m_Camera.getPosition().z);
	ImGui::End();
}