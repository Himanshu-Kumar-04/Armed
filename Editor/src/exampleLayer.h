#pragma once
#include<armed.h>
#include<glm/glm.hpp>

class ExampleLayer : public Arm::Layer {
public:
	ExampleLayer();
	~ExampleLayer() = default;
	void onUpdate(Arm::Timestep ts) override;
	void onEvent(Arm::Event& e) override;
	void onImGuiRender() override;
private:
	Arm::OrthographicCamera m_Camera;
	Arm::Ref<Arm::VertexArray> m_SquareVA;
	Arm::Ref<Arm::Shader> m_Shader;
	Arm::Ref<Arm::Texture2D> m_Texture1;
	Arm::Ref<Arm::Texture2D> m_Texture2;
};