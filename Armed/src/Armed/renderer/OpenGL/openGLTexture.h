#pragma once
#include "Armed/renderer/texture.h"
#include <glad.h>

namespace Arm {
    class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(const int32_t width, const int32_t height);
        OpenGLTexture2D(const std::string& path);

        virtual ~OpenGLTexture2D();

        virtual uint32_t getWidth() const override { return m_Width; }
        virtual uint32_t getHeight() const override { return m_Height; }
        virtual uint32_t getID() const override { return m_RendererID; }
        
        virtual void setData(void* data, uint32_t size) override;
        virtual bool operator==(const Texture& other) const override {
            return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
        }
        virtual void bind(uint32_t slot = 0) const override;
    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };
}