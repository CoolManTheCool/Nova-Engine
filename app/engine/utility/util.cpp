#include "util.hpp"

std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) {
    os << "{ " << vec.x << ", " << vec.y << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << "{ " << vec.x << ", " << vec.y << ", " << vec.z << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec4& vec) {
    os << "{ " << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << " }";
    return os;
}

// Helper function to find the width of the largest number in each column of a matrix
template <typename T, glm::precision P, glm::length_t C, glm::length_t R>
std::array<int, C> find_max_width_per_column(const glm::mat<C, R, T, P>& mat) {
    std::array<int, C> max_widths{};
    for (glm::length_t i = 0; i < C; ++i) {
        int max_width = 0;
        for (glm::length_t j = 0; j < R; ++j) {
            std::ostringstream oss;
            oss << mat[i][j];
            int width = oss.str().length();
            if (width > max_width) {
                max_width = width;
            }
        }
        max_widths[i] = max_width;
    }
    return max_widths;
}

std::ostream& operator<<(std::ostream& os, const glm::mat2& mat) {
    auto max_widths = find_max_width_per_column(mat);
    os << "{\n";
    for (int i = 0; i < 2; ++i) {
        os << "  ";
        for (int j = 0; j < 2; ++j) {
            os << std::setw(max_widths[j]) << mat[j][i];
            if (j < 1) os << ", ";
        }
        if (i < 1) os << "\n";
    }
    os << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::mat3& mat) {
    auto max_widths = find_max_width_per_column(mat);
    os << "{\n";
    for (int i = 0; i < 3; ++i) {
        os << "  ";
        for (int j = 0; j < 3; ++j) {
            os << std::setw(max_widths[j]) << mat[j][i];
            if (j < 2) os << ", ";
        }
        if (i < 2) os << "\n";
    }
    os << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::mat4& mat) {
    auto max_widths = find_max_width_per_column(mat);
    os << "{\n";
    for (int i = 0; i < 4; ++i) {
        os << "  ";
        for (int j = 0; j < 4; ++j) {
            os << std::setw(max_widths[j]) << mat[j][i];
            if (j < 3) os << ", ";
        }
        if (i < 3) os << "\n";
    }
    os << " }";
    return os;
}

Settings_t::Settings_t() {
    width         = 512;
    height        = 512;
    title         = "Untitled Project | Nova Engine";
    version_ID    = 0;
    version_name  = "Unknown Version";
    console_lines = 25;
}

Settings_t::S_RendererSettings::S_RendererSettings() {
    ForceGPU   = false;
    ForceGPUID = 7298; // 0
}

Settings_t Settings = Settings_t();
