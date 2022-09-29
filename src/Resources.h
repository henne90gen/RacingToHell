#pragma once

#include <optional>
#include <string>
#include <string_view>

struct GameMemory;

struct Resource {
    std::string resource_name;
    int64_t last_modified;

    explicit Resource(std::string _resource_name, int64_t last_modified);

    std::string_view get_content(GameMemory *memory);
    [[nodiscard]] bool has_changed(GameMemory *memory) const;
    [[nodiscard]] std::string get_file_name(GameMemory *memory) const;
};

void init_resources();
std::optional<Resource *> get_resource(const std::string &resource_name);
