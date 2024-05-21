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
    private:
        uint32_t m_RendererID;
        uint32_t m_ColorAttachment;
        uint32_t m_DepthAttachment;
        FrameBufferProperties m_Properties;
    };
}