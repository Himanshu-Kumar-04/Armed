#pragma once
#include<iostream>

namespace Arm {
    class UUID {
    public:
        UUID();
        UUID(int64_t _UUID)
            :m_UUID(_UUID) {}
        UUID(const UUID&) = default;

        operator uint64_t() const { return m_UUID; }
    private:
        uint64_t m_UUID;
    };
}

namespace std {
    template <typename T> struct hash;

    template<>
    struct hash<Arm::UUID>
    {
        std::size_t operator()(const Arm::UUID& uuid) const
        {
            return (uint64_t)uuid;
        }
    };

}