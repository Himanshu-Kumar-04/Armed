#pragma once
#include<armed.h>

namespace Arm {
    class EditorLayer : public Layer {
    public:
        EditorLayer();
        ~EditorLayer() = default;
        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onUpdate(Timestep ts) override;
        virtual void onImGuiRender() override;
        virtual void onEvent(Event& e) override;
    private:
        OrthographicCamera m_Camera;
        Ref<VertexArray> m_SquareVA;
        Ref<Shader> m_Shader;
        Ref<FrameBuffer> m_FrameBuffer;
        Ref<Texture2D> m_SpritesSheet;
    };
}