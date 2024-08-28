#include "ArmPCH.h"
#include "openGLFrameBuffer.h"
#include "glad.h"

namespace Arm {

    static bool isDepthFormat(FrameBufferTextureProperties props) {
        switch (props.textureFormat)
        {
        case Arm::FrameBufferTextureFormat::None:               return false;
        case Arm::FrameBufferTextureFormat::DEPTH24STENCIL8:    return true;
        default:                                                return false;
        }
    }

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferProperties& properties)
        :m_Properties(properties)
    {
        for (FrameBufferTextureProperties props : m_Properties.attachments.attachments) {
            if (!isDepthFormat(props))
                m_ColorAttachmentProperties.emplace_back(props);
            else
                m_DepthAttachmentProprty = props;
        }

        invalidate();
    }
    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures((int32_t)m_ColorAttachments.size(), m_ColorAttachments.data());
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
            glDeleteTextures((int32_t)m_ColorAttachments.size(), m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);

            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }

        glCreateFramebuffers(1, &m_RendererID);
        
        //Attachments
        if (m_ColorAttachmentProperties.size()) {
            m_ColorAttachments.resize(m_ColorAttachmentProperties.size());

            for (uint16_t i = 0; i < m_ColorAttachments.size(); i++) {
                switch (m_ColorAttachmentProperties[i].textureFormat)
                {
                case FrameBufferTextureFormat::RGBA8:
                    if (m_Properties.samples > 1) {
                        glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, static_cast<GLsizei>(m_ColorAttachments.size()), m_ColorAttachments.data());
                        glTextureStorage2DMultisample(m_ColorAttachments[i], m_Properties.samples, GL_RGBA8, m_Properties.width, m_Properties.height, GL_FALSE);
                    }
                    else {
                        glCreateTextures(GL_TEXTURE_2D, static_cast<GLsizei>(m_ColorAttachments.size()), m_ColorAttachments.data());
                        glTextureStorage2D(m_ColorAttachments[i], 1, GL_RGBA8, m_Properties.width, m_Properties.height);

                        glTextureParameteri(m_ColorAttachments[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTextureParameteri(m_ColorAttachments[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        glTextureParameteri(m_ColorAttachments[i], GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                        glTextureParameteri(m_ColorAttachments[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTextureParameteri(m_ColorAttachments[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    }
                    glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0 + i, m_ColorAttachments[i], 0);
                    break;
                default:
                    break;
                }
            }
        }

        if (m_DepthAttachmentProprty.textureFormat != FrameBufferTextureFormat::None) {
            switch (m_DepthAttachmentProprty.textureFormat)
            {
            case FrameBufferTextureFormat::DEPTH24STENCIL8:
                if (m_Properties.samples > 1) {
                    glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_DepthAttachment);
                    glTextureStorage2DMultisample(m_DepthAttachment, m_Properties.samples, GL_DEPTH24_STENCIL8, m_Properties.width, m_Properties.height, GL_FALSE);
                }
                else {
                    glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
                    glTextureStorage2D(m_DepthAttachment, 1, GL_DEPTH24_STENCIL8, m_Properties.width, m_Properties.height);

                    glTextureParameteri(m_DepthAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTextureParameteri(m_DepthAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTextureParameteri(m_DepthAttachment, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                    glTextureParameteri(m_DepthAttachment, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTextureParameteri(m_DepthAttachment, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                }
                glNamedFramebufferTexture(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, m_DepthAttachment, 0);
                break;
            default:
                break;
            }
        }

        if (m_ColorAttachments.size() > 1) {
            ARM_ASSERT(m_ColorAttachments.size() <= 4, "too many color attachments");
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

            glNamedFramebufferDrawBuffers(m_RendererID, static_cast<GLsizei>(m_ColorAttachments.size()), buffers);
        }
        else if (m_ColorAttachments.empty())
            glNamedFramebufferDrawBuffer(m_RendererID,GL_NONE);

        ARM_ASSERT((glCheckNamedFramebufferStatus(m_RendererID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "incomplete frame buffer");
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
