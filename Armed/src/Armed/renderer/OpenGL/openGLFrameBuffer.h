#pragma once
#include "Armed/renderer/frameBuffer.h"

namespace Arm {
    class OpenGLFrameBuffer : public FrameBuffer {
    public:
        OpenGLFrameBuffer(const FrameBufferProperties& properties);
        virtual ~OpenGLFrameBuffer();
        
        virtual void bind() override;
        virtual void unbind() override;
        virtual uint32_t getColorAttachmentRendererID() const override { return m_ColorAttachment; }
        virtual const FrameBufferProperties& getProperties() const override { return m_Properties; }

        void invalidate();
        virtual void resize(uint32_t width, uint32_t height) override;
    private:
        uint32_t m_RendererID = 0;
        uint32_t m_ColorAttachment = 0;
        uint32_t m_DepthAttachment = 0;
        FrameBufferProperties m_Properties;
    };
}