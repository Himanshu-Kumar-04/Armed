#include "ArmPCH.h"
#include "openGLFrameBuffer.h"
#include "glad.h"

namespace Arm {

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferProperties& properties)
        :m_Properties(properties)
    {
        invalidate();
    }
    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthAttachment);
    }
    void OpenGLFrameBuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Properties.width, m_Properties.height);
    }
    void OpenGLFrameBuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void OpenGLFrameBuffer::invalidate()
    {
        if (m_RendererID) {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
        }
        glDisable(GL_DEPTH_TEST);

        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Properties.width, m_Properties.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Properties.width, m_Properties.height);
        //glTexImage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Properties.width, m_Properties.height, 0,
        //    GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
        
        ARM_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "incomplete frame buffer");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void OpenGLFrameBuffer::resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0 || width > m_Properties.screenLimit || height > m_Properties.screenLimit) {
            ARM_WARNING("Attempted to resize frame buffer to (%d, %d)", width, height);
            return;
        }
        m_Properties.width = width;
        m_Properties.height = height;

        invalidate();
    }
}
