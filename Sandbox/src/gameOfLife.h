#pragma once
#include<armed.h>

class GameOfLife : public Arm::Layer {
public:
    GameOfLife();
    ~GameOfLife() = default;
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

    static const uint16_t width = 300;
    static const uint16_t height = 300;

    uint8_t cells[width][height] = {};
    uint8_t state[width][height] = {};
};