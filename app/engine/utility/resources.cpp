#include "resources.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

#include <iostream>
#include <iterator>
#include <filesystem>
namespace fs = std::filesystem;

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "util.hpp"

namespace std {
template <>
struct hash<Nova::Mesh::Vertex> {
    size_t operator()(const Nova::Mesh::Vertex& vertex) const {
        size_t seed = 0;
        Nova::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
        return seed;
    }
};

}; // Namespace std

namespace Nova {

std::vector<std::string> searchDirectory(const std::string& directory);
uint64_t constexpr mix(char m, uint64_t s) {
    return ((s<<7) + ~(s>>3)) + ~m;
}
 
uint64_t constexpr _hash(const char * m) {
    return (*m) ? mix(*m, _hash(m+1)) : 0;
}

std::string getExecutableDirectory() {
    std::string executablePath;

    // Get the full path to the executable
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    executablePath = std::string(buffer);
#else
    char buffer[PATH_MAX];
    ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (length != -1) {
        buffer[length] = '\0';
        executablePath = std::string(buffer);
    }
#endif
    // Use std::filesystem to get the parent path
    std::filesystem::path pathObj = std::filesystem::path(executablePath).parent_path();
    // Convert to string and remove the trailing slash/backslash
    std::string directory = pathObj.string();
    // Remove trailing slash or backslash if present
    if (!directory.empty() && (directory.back() == '/' || directory.back() == '\\')) {
        directory.pop_back();
    }

    return directory;
}

Resources::Resources() {
    std::cout << "================================\n";
    std::cout << "Nova Engine                     \n";
    std::cout << "================================\n";

    executablePath = getExecutableDirectory();
    std::cout << "Executable path: " << executablePath << std::endl;
    std::cout << "\nResources:\n";
    for (const auto& path : searchDirectory(executablePath)) {
        std::string ext;
        std::string name;
        size_t dotPos = path.find_last_of(".") + 1;
        ext = (dotPos != 0) ? path.substr(dotPos) : "";
        size_t slashPos = path.find_last_of("/") + 1;
        name = (dotPos != std::string::npos) ? path.substr(0, dotPos-1) : "";
        name = (dotPos != std::string::npos) ? name.substr(slashPos) : "";
        if (!(name == "" || ext == "")) {
            //(ext == "frag" || ext == "vert") ? std::cout : std::cout << "Name: " << name << " Ext: " << ext << "\n";
            switch(_hash(ext.c_str())) {
            case _hash("spv"):
                shaderPaths.insert(std::make_pair(name, path));
                break;
            case _hash("obj"):
                //meshs.insert(std::make_pair(name, Nova::Mesh::createBuilderFromFile(path)));
                meshQueue.emplace_back(std::pair<std::string, std::string>(name, path));
                break;
            }
        }
    }
    std::cout << "\nShaders:";
    for (const auto& entry : shaderPaths) {
        std::cout << "\n - " << entry.first;
    }
    std::cout << "\nModels:";
    for (const auto& entry : meshQueue) {
        std::cout << "\n - " << entry.first;
    }
    std::cout << std::endl;
}

void Resources::cleanup() {
    meshs.clear();
    shaderPaths.clear();
    meshQueue.clear();
}

void Resources::loadMeshs(Device& device) {
    // Load meshes from the meshs map
    for (const auto& entry : meshQueue) {
        std::string name = entry.first;
        std::string path = entry.second;

        meshs.insert(std::make_pair(name, loadMesh(path, device)));
    }
}

std::vector<std::string> searchDirectory(const std::string& directory) {
    std::vector<std::string> files;

    // Check if the given directory exists and is a directory
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        return files; // Return empty vector if directory is invalid
    }

    // Iterate through the directory
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_directory(entry.path())) {
            // If the entry is a directory, recurse into it
            std::vector<std::string> subDirFiles = searchDirectory(entry.path().string());
            // Append the files found in the subdirectory to the main list
            files.insert(files.end(), subDirFiles.begin(), subDirFiles.end());
        } else if (fs::is_regular_file(entry.path())) {
            // If the entry is a file, add it to the list
            files.push_back(entry.path().string());
        }
    }

    return files;
}

std::shared_ptr<Mesh> loadMesh(const std::string &filepath, Device &device) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::vector<Mesh::Vertex> vertices;
    std::vector<uint32_t> indices;

	std::unordered_map<Mesh::Vertex, uint32_t> uniqueVertices{};

	for(const auto &shape : shapes) {
		for (const auto &index : shape.mesh.indices) {
			Mesh::Vertex vertex{};

			if (index.vertex_index >= 0) {
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2],
				};

				auto colorIndex = static_cast<std::vector<float>::size_type>(3 * index.vertex_index + 2);
				if (colorIndex < attrib.colors.size()) {
					vertex.color = {
					attrib.colors[colorIndex - 2],
					attrib.colors[colorIndex - 1],
					attrib.colors[colorIndex - 0],
					};
				} else {
					vertex.color = {1.f, 1.f, 1.f};
				}
			}
			if (index.normal_index >= 0) {
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};
			}
			if (index.texcoord_index >= 0) {
				vertex.uv = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1],
				};
			}

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}

    return std::shared_ptr<Mesh>(new Mesh(vertices, indices, device));
}

/**
 * @brief Get a model by name.
 * 
 * @param name The name of the model.
 * @return std::shared_ptr<Nova::Mesh> A shared pointer to the model.
 * @throws std::invalid_argument if the model is not found.
 * 
 * @todo Implement the model loading logic.
 */
std::shared_ptr<Nova::Mesh> Resources::getMesh(const std::string name) { 
    auto p = meshs.find(name);
    if(p != meshs.end()) {
        return p->second;
    } else {
        throw std::invalid_argument("Mesh not found: " + name);
    }
   return nullptr;
}

std::vector<char> Resources::getShader(const std::string name) {
    auto path = shaderPaths.find(name);
    if(path == shaderPaths.end()) {
        throw std::invalid_argument("Shader not found: " + name);
    }
    std::ifstream file{path->second, std::ios::ate | std::ios::binary};
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + path->second);
	}
    size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	return buffer;
}

} // namespace Nova