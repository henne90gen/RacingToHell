#include "Resources.h"

#include "Memory.h"

#include <filesystem>
#include <fstream>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <utility>

std::unordered_map<std::string, Resource *> resources = {};

Resource::Resource(std::string _resource_name, int64_t _last_modified)
    : resource_name(std::move(_resource_name)), last_modified(_last_modified) {}

std::string_view Resource::get_content(GameMemory *memory) {
    const std::string file_name = get_file_name(memory);
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

    return std::string_view(buffer, file_size);
}

bool Resource::has_changed(GameMemory *memory) const {
    auto last_write_time = std::filesystem::last_write_time(get_file_name(memory));
    auto timestamp =
        std::chrono::time_point_cast<std::chrono::milliseconds>(last_write_time).time_since_epoch().count();
    return timestamp > last_modified;
}

std::string Resource::get_file_name(GameMemory *memory) const {
    return std::string(memory->base_path) + "/" + resource_name;
}

std::optional<Resource *> get_resource(const std::string &resource_name) {
    auto itr = resources.find(resource_name);
    if (itr == resources.end()) {
        spdlog::error("Failed to find resource {}", resource_name);
        return {};
    }

    return itr->second;
}
