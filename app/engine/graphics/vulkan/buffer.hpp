#pragma once

#include "device.hpp"

#include <cstdint>
#include <type_traits>

namespace Nova {

class Buffer {
public:
    enum class Usage : uint32_t {
        None        = 0,
        TransferSrc = 1u << 0,
        TransferDst = 1u << 1,
        Uniform     = 1u << 2,
        Storage     = 1u << 3,
        Index       = 1u << 4,
        Vertex      = 1u << 5,
        Indirect    = 1u << 6
    };

    enum class MemoryProperty : uint32_t {
        None         = 0,
        DeviceLocal  = 1u << 0,
        HostVisible  = 1u << 1,
        HostCoherent = 1u << 2,
        HostCached   = 1u << 3
    };

    Buffer(
        Device&        device,
        VkDeviceSize   instanceSize,
        uint32_t       instanceCount,
        Usage          usage,
        MemoryProperty memoryProperties,
        VkDeviceSize   minOffsetAlignment = 1
    );

    Buffer(
        Device&        device,
        uint64_t       size,
        Usage          usage,
        MemoryProperty memoryProperties
    );

    ~Buffer();

    Buffer(const Buffer&)            = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&&)            = delete;
    Buffer& operator=(Buffer&&) = delete;

    VkResult map(
        VkDeviceSize size   = VK_WHOLE_SIZE,
        VkDeviceSize offset = 0
    );

    void unmap();

    void writeToBuffer(
        const void*  data,
        VkDeviceSize size   = VK_WHOLE_SIZE,
        VkDeviceSize offset = 0
    );

    void readFromBuffer(
        void*        data,
        VkDeviceSize size   = VK_WHOLE_SIZE,
        VkDeviceSize offset = 0
    ) const;

    VkResult flush(
        VkDeviceSize size   = VK_WHOLE_SIZE,
        VkDeviceSize offset = 0
    );

    VkResult invalidate(
        VkDeviceSize size   = VK_WHOLE_SIZE,
        VkDeviceSize offset = 0
    );

    VkDescriptorBufferInfo descriptorInfo(
        VkDeviceSize size   = VK_WHOLE_SIZE,
        VkDeviceSize offset = 0
    ) const;

    void writeToIndex(const void* data, uint32_t index);

    VkResult flushIndex(uint32_t index);

    VkResult invalidateIndex(uint32_t index);

    VkDescriptorBufferInfo descriptorInfoForIndex(uint32_t index) const;

    VkBuffer getBuffer() const {
        return buffer;
    }

    void* getMappedMemory() const {
        return mapped;
    }

    uint32_t getInstanceCount() const {
        return instanceCount;
    }

    VkDeviceSize getInstanceSize() const {
        return instanceSize;
    }

    VkDeviceSize getAlignmentSize() const {
        return alignmentSize;
    }

    Usage getUsage() const {
        return usage;
    }

    MemoryProperty getMemoryProperties() const {
        return memoryProperties;
    }

    VkDeviceSize getBufferSize() const {
        return bufferSize;
    }

private:
    static VkDeviceSize getAlignment(
        VkDeviceSize instanceSize,
        VkDeviceSize minOffsetAlignment
    );

    static VkBufferUsageFlags toVulkanUsage(Usage usage);

    static VkMemoryPropertyFlags toVulkanMemoryProperties(
        MemoryProperty properties
    );

    Device& device;

    void*          mapped = nullptr;
    VkBuffer       buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;

    VkDeviceSize bufferSize    = 0;
    VkDeviceSize instanceSize  = 0;
    uint32_t     instanceCount = 0;
    VkDeviceSize alignmentSize = 0;

    Usage          usage            = Usage::None;
    MemoryProperty memoryProperties = MemoryProperty::None;
};

// Bit operators

constexpr Buffer::Usage operator|(
    Buffer::Usage lhs,
    Buffer::Usage rhs
) {
    using Value = std::underlying_type_t<Buffer::Usage>;

    return static_cast<Buffer::Usage>(
        static_cast<Value>(lhs) |
        static_cast<Value>(rhs)
    );
}

constexpr Buffer::Usage operator&(
    Buffer::Usage lhs,
    Buffer::Usage rhs
) {
    using Value = std::underlying_type_t<Buffer::Usage>;

    return static_cast<Buffer::Usage>(
        static_cast<Value>(lhs) &
        static_cast<Value>(rhs)
    );
}

constexpr Buffer::Usage& operator|=(
    Buffer::Usage& lhs,
    Buffer::Usage  rhs
) {
    lhs = lhs | rhs;
    return lhs;
}

constexpr Buffer::MemoryProperty operator|(
    Buffer::MemoryProperty lhs,
    Buffer::MemoryProperty rhs
) {
    using Value = std::underlying_type_t<Buffer::MemoryProperty>;

    return static_cast<Buffer::MemoryProperty>(
        static_cast<Value>(lhs) |
        static_cast<Value>(rhs)
    );
}

constexpr Buffer::MemoryProperty operator&(
    Buffer::MemoryProperty lhs,
    Buffer::MemoryProperty rhs
) {
    using Value = std::underlying_type_t<Buffer::MemoryProperty>;

    return static_cast<Buffer::MemoryProperty>(
        static_cast<Value>(lhs) &
        static_cast<Value>(rhs)
    );
}

constexpr Buffer::MemoryProperty& operator|=(
    Buffer::MemoryProperty& lhs,
    Buffer::MemoryProperty  rhs
) {
    lhs = lhs | rhs;
    return lhs;
}

} // namespace Nova