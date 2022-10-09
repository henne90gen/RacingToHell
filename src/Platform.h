#pragma once

#include <cstdint>
#include <cstring>
#include <fmt/core.h>
#include <glm/vec2.hpp>
#include <string>
#include <string_view>

#define DEFAULT_WINDOW_WIDTH 450
#define DEFAULT_WINDOW_HEIGHT 800
#define WINDOW_TITLE "Racing to Hell"
#define NUM_PLAYER_TEXTURES 6
#define NUM_TRAFFIC_TEXTURES 7
#define NUM_ITEM_TEXTURES 2
#define NUM_DIFFICULTIES 4
#define TOOLBOX_ID 0
#define CANISTER_ID 1

// Debug flags:
#define RENDER_DEBUG 0
#define COLLISION_DEBUG 0
#define SOUND_DEBUG 0
#define SOUND_ENABLE 0

struct File {
    char *content = nullptr;
    size_t size = 0;
    bool is_resource = false;
    std::string name;
};

struct GameMemory {
    bool isInitialized = false;

    float aspectRatio;
    bool doResize;

    std::string base_path;

    char *temporary;
    size_t temporaryMemorySize = 0;
    size_t temporaryMemoryOffset = 0;

    char *permanent;
    size_t permanentMemorySize = 0;
    size_t permanentMemoryOffset = 0;
};

struct Input {
    glm::vec2 mousePosition;
    // TODO convert these into a bit mask (optimization)
    bool upKeyPressed, downKeyPressed, leftKeyPressed, rightKeyPressed;
    bool upKeyClicked, downKeyClicked, leftKeyClicked, rightKeyClicked;
    bool enterKeyPressed, escapeKeyPressed;
    bool enterKeyClicked, escapeKeyClicked;
    bool shootKeyPressed, shootKeyClicked;
    bool plusKeyPressed, plusKeyClicked;
    bool minusKeyPressed, minusKeyClicked;
};

struct Platform {
    GameMemory memory = {};
    Input input = {};
    double frameTimeMs = 0.0;

    // ---------------------------------------------------
    /// crash the game because of a fatal error
    void abort(const std::string &msg);
    /// gracefully shut down game
    void exit();
    /// log a message to the console
    void log(const std::string &msg);
    /// reads a file from disk
    File read_file(const std::string &file_path, bool is_resource);
    /// releases the memory that was allocated for the given file
    void free_file(File &file);
    /// returns the last modification time in nanoseconds since the epoch
    int64_t last_modified(const std::string &file_path);
    /// writes a file to disk
    bool write_file(const File &file);
    // ---------------------------------------------------

    template <typename... T> void logf(fmt::format_string<T...> fmt, T &&...args) {
        log(fmt::vformat(fmt, fmt::make_format_args(args...)));
    }
};
