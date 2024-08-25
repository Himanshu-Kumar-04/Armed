#pragma once
#include "Armed/renderer/frameBuffer.h"
#include "Armed/core/core.h"

namespace Arm {
    class OpenGLFrameBuffer : public FrameBuffer {
    public:
        OpenGLFrameBuffer(const FrameBufferProperties& properties);
        virtual ~OpenGLFrameBuffer();
        
        virtual void bind() override;
        virtual void unbind() override;
        virtual uint32_t getColorAttachmentRendererID(uint32_t index = 0) const override { ARM_ASSERT(index < m_ColorAttachments.size(),"too many color attachments"); return m_ColorAttachments[index]; }
        virtual const FrameBufferProperties& getProperties() const override { return m_Properties; }

        void invalidate();
        virtual void resize(uint32_t width, uint32_t height) override;
    private:
        RendererID m_RendererID = 0;
        std::vector<RendererID> m_ColorAttachments;
        RendererID m_DepthAttachment = 0;

        FrameBufferProperties m_Properties;
        
        std::vector<FrameBufferTextureProperties> m_ColorAttachmentProperties;
        FrameBufferTextureProperties m_DepthAttachmentProprty = FrameBufferTextureFormat::None;
    };
}