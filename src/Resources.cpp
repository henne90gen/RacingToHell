#include "Resources.h"

#include "Memory.h"

#include <fstream>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <utility>

std::unordered_map<std::string, Resource *> resources = {};

Resource::Resource(std::string _resource_name, std::string_view _content, bool _file_too_large)
    : resource_name(std::move(_resource_name)), content(_content), file_too_large(_file_too_large) {}

std::string_view Resource::get_content(GameMemory *memory) {
    if (!file_too_large) {
        return content;
    }

    const std::string file_name = std::string(memory->base_path) + "/" + resource_name;
    auto f = std::ifstream(file_name, std::ios::binary | std::ios::ate);

    auto file_size = f.tellg();
    f.seekg(0);

    char *buffer = (char *)malloc(file_size);
    auto ptr = buffer;

    char buf[256];
    do {
        f.read(buf, 256);
        auto bytes_read = f.gcount();
        for (int i = 0; i < bytes_read; i++) {
            *ptr = buf[i];
            ptr++;
        }
    } while (f);

    content = std::string_view(buffer, file_size);
    return content;
}

std::optional<Resource *> get_resource(const std::string &resource_name) {
    auto itr = resources.find(resource_name);
    if (itr == resources.end()) {
        spdlog::error("Failed to find resource {}", resource_name);
        return {};
    }

    return itr->second;
}
