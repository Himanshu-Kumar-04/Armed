#pragma once
#include<armed.h>

class Sandbox2D : public Arm::Layer {
public:
    Sandbox2D();
    ~Sandbox2D() = default;
    virtual void onAttach() override;
    virtual void onDetach() override;
    virtual void onUpdate(Arm::Timestep ts) override;
    virtual void onImGuiRender() override;
    virtual void onEvent(Arm::Event& e) override;
private:
    Arm::OrthographicCamera m_Camera;
    Arm::Ref<Arm::VertexArray> m_SquareVA;
    Arm::Ref<Arm::Shader> m_Shader;
    Arm::Ref<Arm::FrameBuffer> m_FrameBuffer;
    Arm::Ref<Arm::Texture2D> m_SpritesSheet;
};