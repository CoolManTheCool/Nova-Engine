#pragma once
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace Nova {

class Object;

template<typename T>
class ObjectRef {
    static_assert(std::is_base_of<Object, T>::value, "T must derive from Object");

public:
    ObjectRef() = default;
    explicit ObjectRef(const std::shared_ptr<T>& obj) : ref(obj) {}

    bool isValid() const { return !ref.expired(); }

    // Returns a shared_ptr<T> for temporary ownership
    std::shared_ptr<T> lock() const { return std::dynamic_pointer_cast<T>(ref.lock()); }

    // Script-safe access operators
    T* operator->() {
        auto s = lock();
#if defined(DEBUG) || !defined(NDEBUG)
        if (!s) throw std::runtime_error("Tried to access destroyed Object");
#else
        if (!s) return nullptr;
#endif
        return s.get();
    }

    const T* operator->() const {
        auto s = lock();
#if defined(DEBUG) || !defined(NDEBUG)
        if (!s) throw std::runtime_error("Tried to access destroyed Object");
#else
        if (!s) return nullptr;
#endif
        return s.get();
    }

private:
    std::weak_ptr<T> ref;
};

} // namespace Nova