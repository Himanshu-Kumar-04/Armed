#pragma once

namespace Arm {
    struct FrameBufferProperties {
        uint32_t width, height;
        uint32_t samples = 1;
        bool swapChainTarget = false;
    };

    class FrameBuffer {
    public:
        virtual const FrameBufferProperties& getProperties() const = 0;
        virtual uint32_t getColorAttachmentRendererID() const = 0;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        static Ref<FrameBuffer> create(const FrameBufferProperties& properties);
    private:
        FrameBufferProperties m_Properties;
    };
}