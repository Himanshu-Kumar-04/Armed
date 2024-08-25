#pragma once
#include <vector>

namespace Arm {
    enum class FrameBufferTextureFormat {
        None = 0,

        //Color
        RGBA8,

        //Depth/Stencil
        DEPTH24STENCIL8,
        
        //Default
        Depth = DEPTH24STENCIL8
    };

    struct FrameBufferTextureProperties {
        FrameBufferTextureProperties() = default;
        FrameBufferTextureProperties(FrameBufferTextureFormat format)
            : textureFormat(format) {}
        FrameBufferTextureFormat textureFormat = FrameBufferTextureFormat::None;
    };

    struct FrameBufferAttachment {
        FrameBufferAttachment() = default;
        FrameBufferAttachment(std::initializer_list<FrameBufferTextureProperties> _attachments)
            : attachments(_attachments) {}

        std::vector<FrameBufferTextureProperties> attachments;
    };

    struct FrameBufferProperties {
        uint32_t width  = 400;
        uint32_t height = 400;
        FrameBufferAttachment attachments;
        uint32_t samples = 1;
        bool swapChainTarget = false;
        uint32_t screenLimit = 8192;
    };

    class FrameBuffer {
    public:
        virtual const FrameBufferProperties& getProperties() const = 0;
        virtual uint32_t getColorAttachmentRendererID(uint32_t index = 0) const = 0;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual void resize(uint32_t width, uint32_t height) = 0;
        static Ref<FrameBuffer> create(const FrameBufferProperties& properties);
    private:
        FrameBufferProperties m_Properties;
    };
}