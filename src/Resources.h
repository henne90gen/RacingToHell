#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "Platform.h"

struct Resource {
    std::string resource_name;
    int64_t last_modified = 0;
    File file = {};

    explicit Resource(std::string _resource_name);

    std::string_view get_content(Platform &platform);
    [[nodiscard]] bool has_changed(Platform &platform) const;
    [[nodiscard]] std::string get_file_name(Platform &platform) const;
};

void init_resources();
std::optional<Resource *> get_resource(const std::string &resource_name);
