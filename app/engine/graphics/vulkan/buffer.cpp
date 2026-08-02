#include "buffer.hpp"

#include <cassert>
#include <cstring>
#include <type_traits>

namespace Nova {

VkDeviceSize Buffer::getAlignment(
    VkDeviceSize instanceSize,
    VkDeviceSize minOffsetAlignment
) {
    if (minOffsetAlignment > 0) {
        return (instanceSize + minOffsetAlignment - 1) &
               ~(minOffsetAlignment - 1);
    }

    return instanceSize;
}

VkBufferUsageFlags Buffer::toVulkanUsage(Usage usage)
{
    VkBufferUsageFlags flags = 0;

    if ((usage & Usage::TransferSrc) != Usage::None)
        flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    if ((usage & Usage::TransferDst) != Usage::None)
        flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    if ((usage & Usage::Vertex) != Usage::None)
        flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    if ((usage & Usage::Index) != Usage::None)
        flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

    if ((usage & Usage::Uniform) != Usage::None)
        flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    if ((usage & Usage::Storage) != Usage::None)
        flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

    if ((usage & Usage::Indirect) != Usage::None)
        flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;

    return flags;
}

VkMemoryPropertyFlags Buffer::toVulkanMemoryProperties(
    MemoryProperty properties
) {
    VkMemoryPropertyFlags flags = 0;

    if ((properties & MemoryProperty::DeviceLocal) != MemoryProperty::None)
        flags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    if ((properties & MemoryProperty::HostVisible) != MemoryProperty::None)
        flags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    if ((properties & MemoryProperty::HostCoherent) != MemoryProperty::None)
        flags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    if ((properties & MemoryProperty::HostCached) != MemoryProperty::None)
        flags |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;

    return flags;
}


Buffer::Buffer(
    Device&        device,
    VkDeviceSize   instanceSize,
    uint32_t       instanceCount,
    Usage          usage,
    MemoryProperty memoryProperties,
    VkDeviceSize   minOffsetAlignment
)
    : device{device},
      instanceSize{instanceSize},
      instanceCount{instanceCount},
      usage{usage},
      memoryProperties{memoryProperties}
{
    alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
    bufferSize    = alignmentSize * instanceCount;

    device.createBuffer(
        bufferSize,
        toVulkanUsage(usage),
        toVulkanMemoryProperties(memoryProperties),
        buffer,
        memory
    );
}


Buffer::~Buffer()
{
    unmap();

    if (buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(device.device(), buffer, nullptr);
    }

    if (memory != VK_NULL_HANDLE) {
        vkFreeMemory(device.device(), memory, nullptr);
    }
}


VkResult Buffer::map(
    VkDeviceSize size,
    VkDeviceSize offset
) {
    assert(buffer && memory && "Called map on buffer before create");

    return vkMapMemory(
        device.device(),
        memory,
        offset,
        size,
        0,
        &mapped
    );
}


void Buffer::unmap()
{
    if (mapped) {
        vkUnmapMemory(device.device(), memory);
        mapped = nullptr;
    }
}


void Buffer::writeToBuffer(
    const void* data,
    VkDeviceSize size,
    VkDeviceSize offset
) {
    assert(mapped && "Cannot copy to unmapped buffer");

    if (size == VK_WHOLE_SIZE)
        size = bufferSize - offset;

    memcpy(
        static_cast<char*>(mapped) + offset,
        data,
        size
    );
}


void Buffer::readFromBuffer(
    void* data,
    VkDeviceSize size,
    VkDeviceSize offset
) const {
    if (size == VK_WHOLE_SIZE)
        size = bufferSize - offset;

    memcpy(
        data,
        static_cast<char*>(mapped) + offset,
        size
    );
}


VkResult Buffer::flush(
    VkDeviceSize size,
    VkDeviceSize offset
) {
    VkMappedMemoryRange mappedRange{};
    mappedRange.sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = memory;
    mappedRange.offset = offset;
    mappedRange.size   = size;

    return vkFlushMappedMemoryRanges(
        device.device(),
        1,
        &mappedRange
    );
}


VkResult Buffer::invalidate(
    VkDeviceSize size,
    VkDeviceSize offset
) {
    VkMappedMemoryRange mappedRange{};
    mappedRange.sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = memory;
    mappedRange.offset = offset;
    mappedRange.size   = size;

    return vkInvalidateMappedMemoryRanges(
        device.device(),
        1,
        &mappedRange
    );
}


VkDescriptorBufferInfo Buffer::descriptorInfo(
    VkDeviceSize size,
    VkDeviceSize offset
) const {
    if (size == VK_WHOLE_SIZE)
        size = bufferSize - offset;

    return {
        buffer,
        offset,
        size
    };
}


void Buffer::writeToIndex(
    const void* data,
    uint32_t index
) {
    writeToBuffer(
        data,
        instanceSize,
        index * alignmentSize
    );
}


VkResult Buffer::flushIndex(uint32_t index)
{
    return flush(
        alignmentSize,
        index * alignmentSize
    );
}


VkDescriptorBufferInfo Buffer::descriptorInfoForIndex(
    uint32_t index
) const {
    return descriptorInfo(
        alignmentSize,
        index * alignmentSize
    );
}


VkResult Buffer::invalidateIndex(uint32_t index)
{
    return invalidate(
        alignmentSize,
        index * alignmentSize
    );
}

} // namespace Nova