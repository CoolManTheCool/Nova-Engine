#pragma once

namespace Nova {

constexpr uint64_t fnv1a_64(const char* str) {
    uint64_t hash = 1469598103934665603ULL; // FNV offset basis
    while (*str) {
        hash ^= static_cast<uint64_t>(*str++);
        hash *= 1099511628211ULL; // FNV prime
    }
    return hash;
}

constexpr uint64_t hash(const char* str) {
    return fnv1a_64(str);
}

} // namespace Nova