#pragma once
#include"Armed/core/core.h"
#include<string>

namespace Arm {
    class Texture {
    public:
        virtual ~Texture() = default;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;

        virtual void setData(void* data,uint32_t size) = 0;
        
        virtual bool operator==(const Texture& other) const = 0;

        virtual uint32_t getID() const = 0;
        virtual void bind(uint32_t slot = 0) const = 0;
    };

    class Texture2D : public Texture {
    public:
        static Ref<Texture2D> Create(uint32_t width, uint32_t height);
        static Ref<Texture2D> Create(const std::string& path);

    };
}