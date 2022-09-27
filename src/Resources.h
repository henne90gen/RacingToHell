#pragma once

#include <optional>
#include <string>
#include <string_view>

struct Resource {
    std::string resource_name;
    std::string_view content;
    bool file_too_large;
    uint64_t last_modified = 0;

    Resource(std::string _resource_name, std::string_view _content, bool _file_too_large);

    std::string_view get_content(const std::string &base_path);
};

std::optional<Resource *> get_resource(const std::string &resource_name);
