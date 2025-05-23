#include "rollingbuffer.hpp"

RollingBuffer::RollingBuffer(std::type_index type, size_t size)
    : bufferType(type), size(size), index(0), count(0) {
    buffer.resize(size);
}

size_t RollingBuffer::getCount() const {
    return count;
}

std::type_index RollingBuffer::getType() const {
    return bufferType;
}

void RollingBuffer::resize(size_t newSize) {
    if (newSize == size) return;

    std::vector<std::unique_ptr<IValueHolder>> newBuffer(newSize);
    size_t elementsToMove = std::min(count, newSize);

    // Copy elements in order to the new buffer
    for (size_t i = 0; i < elementsToMove; ++i) {
        size_t oldIndex = (index + size - count + i) % size;
        newBuffer[i] = std::move(buffer[oldIndex]);
    }

    buffer = std::move(newBuffer);
    size = newSize;
    index = elementsToMove % newSize;
    count = elementsToMove;
}