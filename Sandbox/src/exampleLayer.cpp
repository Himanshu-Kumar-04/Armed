#include "exampleLayer.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
ExampleLayer::ExampleLayer()
	: Layer("example"), m_Camera(0.78f, 1280.0f / 720.0f, 0.1f, 100.0f)
{
	float vertices[] = {
	-0.5f, 0.5f, 5.0f,	0.0f,0.0f,	0.0f, 1.0f, 1.0f, 1.0f,
	 0.5f, 0.5f, 5.0f,	1.0f,0.0f,	0.3f, 0.0f, 1.0f, 1.0f,
	 0.5f, 1.5f, 5.0f,	1.0f,1.0f,	1.0f, 0.4f, 0.5f, 1.0f,
	-0.5f, 1.5f, 5.0f,	0.0f,1.0f,	1.0f, 1.0f, 6.0f, 1.0f,
	};

	// Indices for vertices order
	uint32_t indices[] =
	{
		0,1,2,2,3,0
	};

	// Generates Vertex Array Object and binds it
	m_SquareVA = Arm::VertexArray::Create();
	// Generates Vertex Buffer Object and links it to vertices
	Arm::Ref<Arm::VertexBuffer> vb =  Arm::VertexBuffer::Create(vertices, sizeof(vertices));
	vb->setLayout({
		{Arm::ShaderDataType::Float3, "position"},
		{Arm::ShaderDataType::Float2, "texCoord"},
		{Arm::ShaderDataType::Float4, "tintColor"},
		});
	m_SquareVA->addVertexBuffer(vb);
	// Generates Element Buffer Object and links it to indices
	Arm::Ref<Arm::IndexBuffer> ib =  Arm::IndexBuffer::Create(indices, sizeof(indices));
	m_SquareVA->setIndexBuffer(ib);
	
	//m_Texture1 = Arm::Texture2D::Create("assets/texture/grass_tile.png");

	m_Shader = Arm::Shader::create("assets/shader/d_texture.glsl");
	m_Shader->bind();

	//m_Texture1->bind(0);
	//m_Shader->setInt("u_Texture", 0);
}

void ExampleLayer::onUpdate(Arm::Timestep ts)
{
	m_Camera.onUpdate(ts);

	Arm::RendererCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Arm::RendererCommand::clearColor();

	static float rotation = 0.0f;
	rotation += ts * 9.0f;
	
	Arm::Renderer::beginScene(m_Camera);
	//m_Texture1->bind(0);
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
