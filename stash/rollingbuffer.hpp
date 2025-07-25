#ifndef ROLLING_BUFFER_HPP
#define ROLLING_BUFFER_HPP

#include <memory>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include <stdexcept>

class IValueHolder {
public:
    virtual ~IValueHolder() = default;
    virtual std::unique_ptr<IValueHolder> clone() const = 0;
};

template <typename T>
class ValueHolder : public IValueHolder {
public:
    T value;
    ValueHolder(const T& val);
    std::unique_ptr<IValueHolder> clone() const override;
};

class RollingBuffer {
public:
    RollingBuffer(std::type_index type, size_t size);

    template<typename T>
    void add(const T& value);

    template<typename T>
    std::vector<T> getOrderedArray() const;

    void resize(size_t newSize);
    void clear() { count = 0; index = 0; }

    size_t getCount() const;
    std::type_index getType() const;

private:
    std::type_index bufferType;
    size_t size;
    size_t index;
    size_t count;
    std::vector<std::unique_ptr<IValueHolder>> buffer;
};

// Template definitions must be in the header file
template <typename T>
ValueHolder<T>::ValueHolder(const T& val) : value(val) {}

template <typename T>
std::unique_ptr<IValueHolder> ValueHolder<T>::clone() const {
    return std::make_unique<ValueHolder<T>>(value);
}

template<typename T>
void RollingBuffer::add(const T& value) {
    if (std::type_index(typeid(T)) != bufferType)
        throw std::runtime_error("Type mismatch in RollingBuffer::add");

    buffer[index] = std::make_unique<ValueHolder<T>>(value);
    index = (index + 1) % size;
    if (count < size) ++count;
}

template<typename T>
std::vector<T> RollingBuffer::getOrderedArray() const {
    if (std::type_index(typeid(T)) != bufferType)
        throw std::runtime_error("Type mismatch in RollingBuffer::getOrderedArray");

    std::vector<T> result;
    for (size_t i = 0; i < count; ++i) {
        size_t realIndex = (index + size - count + i) % size;
        auto* holder = dynamic_cast<ValueHolder<T>*>(buffer[realIndex].get());
        result.push_back(holder->value);
    }
    return result;
}

#endif // ROLLING_BUFFER_HPP