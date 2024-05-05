#include<armed.h>
class ExampleLayer : public Arm::Layer {
public:
	ExampleLayer()
		:m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), Layer("")
	{
		float squareVertices[3 * 4] = {
			-0.5f ,-0.5f, 0.0f,
			 0.5f ,-0.5f, 0.0f,
			 0.5f , 0.5f, 0.0f,
			-0.5f , 0.5f, 0.0f
		};

		m_SquareVA.reset(Arm::VertexArray::Create());
		std::shared_ptr<Arm::VertexBuffer> squareVB;
		squareVB.reset(Arm::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->setLayout({
				{Arm::ShaderDataType::Float3 , "position"}
			});
		m_SquareVA->addVertexBuffer(squareVB);
		uint32_t squareIndices[6] = { 0,1,2 ,2 ,3, 0 };
		std::shared_ptr<Arm::IndexBuffer> squareIB;
		squareIB.reset(Arm::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->setIndexBuffer(squareIB);

		m_Shader.reset(Arm::Shader::Create());
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
			m_CameraRotation += m_CameraRotationSpeed*ts;
		else if (Arm::Input::isKeyPressed(Arm::Key::E))
			m_CameraRotation -= m_CameraRotationSpeed*ts;

		Arm::RendererCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Arm::RendererCommand::clearColor();

		m_Camera.setPosition(m_CameraPosition);
		m_Camera.setRotation(m_CameraRotation);

		Arm::Renderer::beginScene(m_Camera);

		m_Shader->bind();
		Arm::Renderer::submit(m_Shader, m_SquareVA);

		Arm::Renderer::endScene();
	}
	void onEvent(Arm::Event& event) override
	{
	}
private:
	std::shared_ptr<Arm::VertexArray> m_SquareVA;

	std::shared_ptr<Arm::Shader> m_Shader;

	Arm::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition = {0.0f,0.0f,0.0f};
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotationSpeed = 180.0f;
	float m_CameraRotation= 0.0f;
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