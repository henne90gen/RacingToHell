#include "Resources.h"

#include "Memory.h"

#include <filesystem>
#include <fstream>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <utility>

std::unordered_map<std::string, Resource *> resources = {};

Resource::Resource(std::string _resource_name) : resource_name(std::move(_resource_name)) {}

std::string_view Resource::get_content(Platform &platform) {
    last_modified = platform.last_modified(get_file_name(platform));
    file = platform.read_file(resource_name, true);
    return {file.content, file.size};
}

bool Resource::has_changed(Platform &platform) const {
    auto timestamp = platform.last_modified(get_file_name(platform));
    return timestamp > last_modified;
}

std::string Resource::get_file_name(Platform &platform) const {
    return std::string(platform.memory.base_path) + "/" + resource_name;
}

std::optional<Resource *> get_resource(Platform &platform, const std::string &resource_name) {
    auto itr = resources.find(resource_name);
    if (itr == resources.end()) {
        platform.log("Failed to find resource " + resource_name);
        return {};
    }

    return itr->second;
}
