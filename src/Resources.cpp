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

std::string_view Resource::get_content(Platform &platform) {
    file = platform.read_file(resource_name, true);
    return {file.content, file.size};
}

bool Resource::has_changed(Platform &platform) const {
    auto last_write_time = std::filesystem::last_write_time(get_file_name(platform));
    auto timestamp =
        std::chrono::time_point_cast<std::chrono::milliseconds>(last_write_time).time_since_epoch().count();
    return timestamp > last_modified;
}

std::string Resource::get_file_name(Platform &platform) const {
    return std::string(platform.memory.base_path) + "/" + resource_name;
}

std::optional<Resource *> get_resource(const std::string &resource_name) {
    auto itr = resources.find(resource_name);
    if (itr == resources.end()) {
        spdlog::error("Failed to find resource {}", resource_name);
        return {};
    }

    return itr->second;
}
