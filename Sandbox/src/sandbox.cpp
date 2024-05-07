#include<armed.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include <Armed/renderer/OpenGL/openGLShader.h>


class ExampleLayer : public Arm::Layer {
public:
	ExampleLayer()
		:m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), Layer(""), m_CameraPosition(0.0f)
	{
		float squareVertices[4 * 5] = {
			-0.5f ,-0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f ,-0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f , 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f , 0.5f, 0.0f, 0.0f, 1.0f
		};

		m_SquareVA.reset(Arm::VertexArray::Create());
		Arm::Ref<Arm::VertexBuffer> squareVB;
		squareVB.reset(Arm::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->setLayout({
				{Arm::ShaderDataType::Float3 , "position"},
				{Arm::ShaderDataType::Float2 , "texCoord"}
			});
		m_SquareVA->addVertexBuffer(squareVB);
		uint32_t squareIndices[6] = { 0,1,2 ,2 ,3, 0 };
		Arm::Ref<Arm::IndexBuffer> squareIB;
		squareIB.reset(Arm::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->setIndexBuffer(squareIB);

		m_Shader.reset(Arm::Shader::Create("assets/shader/texture.glsl"));

		m_Texture1 = Arm::Texture2D::Create("assets/texture/Landscape.jpg");
		m_Texture2 = Arm::Texture2D::Create("assets/texture/Armed.jpeg");

		std::dynamic_pointer_cast<Arm::OpenGLShader>(m_Shader)->bind();
		std::dynamic_pointer_cast<Arm::OpenGLShader>(m_Shader)->setUniform1i("u_Texture", 0);
	}

	void onUpdate(Arm::Timestep ts) override
	{
		if (Arm::Input::isKeyPressed(Arm::Key::A))
			m_CameraPosition.x -= m_CameraMoveSpeed*ts;
		else if (Arm::Input::isKeyPressed(Arm::Key::D))
			m_CameraPosition.x += m_CameraMoveSpeed*ts;

		if (Arm::Input::isKeyPressed(Arm::Key::W))
			m_CameraPosition.y += m_CameraMoveSpeed*ts;
		else if (Arm::Input::isKeyPressed(Arm::Key::S))
			m_CameraPosition.y -= m_CameraMoveSpeed*ts;

		if (Arm::Input::isKeyPressed(Arm::Key::Q))
			m_CameraRotation -= m_CameraRotationSpeed*ts;
		else if (Arm::Input::isKeyPressed(Arm::Key::E))
			m_CameraRotation += m_CameraRotationSpeed*ts;

		Arm::RendererCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Arm::RendererCommand::clearColor();

		m_Camera.setPosition(m_CameraPosition);
		m_Camera.setRotation(m_CameraRotation);

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
		Arm::Renderer::submit(m_Shader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_Texture2->bind();
		Arm::Renderer::submit(m_Shader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));

		Arm::Renderer::endScene();
	}
	void onEvent(Arm::Event& event) override
	{
	}
	/*void onImGuiRender() override{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}*/
private:
	Arm::Ref<Arm::VertexArray> m_SquareVA;
	Arm::Ref<Arm::Shader> m_Shader;
	Arm::Ref<Arm::Texture2D> m_Texture1;
	Arm::Ref<Arm::Texture2D> m_Texture2;
	Arm::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;
	
	float m_CameraRotationSpeed = 180.0f;
	float m_CameraRotation= 0.0f;
	
	//glm::vec3 m_SquareColor = {0.2f,0.3f,0.5f};
};

class Sandbox : public Arm::Application {
public:
	Sandbox() {
		pushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};

Arm::Application* Arm::createApplication() {
	return new Sandbox();
}