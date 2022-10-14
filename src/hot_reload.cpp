#if WIN32
#include <Windows.h>
#undef min
#else
#include <dlfcn.h>
#endif

#include <filesystem>
#include <string>

#include "RacingToHell.h"

struct GameCode {
    std::string sourceDllPath;
    std::string tmpDllPath;
    int64_t lastModified = std::numeric_limits<int64_t>::min();

#if WIN32
    HMODULE gameCodeDLL = nullptr;
#else
    void *libraryHandle;
#endif

    update_and_render_func *update_and_render = nullptr;
    init_resources_func *init_resources = nullptr;
};
GameCode gameCode = {};

void platform_abort(const std::string &msg);
void platform_log(const std::string &msg);
int64_t platform_last_modified(const std::string &file_path);

#ifdef HOT_RELOAD
#if WIN32
std::string GetLastErrorAsString() {
    // Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return {}; // No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;

    // Ask Win32 to give us the string version of that message ID.
    // The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet
    // know how long the message string will be).
    size_t size =
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    // Copy the error message into a std::string.
    std::string message(messageBuffer, size);

    // Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}

void reload_game_code(GameCode &code) {
    if (code.sourceDllPath.empty()) {
        char filename[1000];
        GetModuleFileNameA(nullptr, filename, sizeof(filename));
        auto exePath = std::filesystem::path(std::string(filename)).parent_path().generic_string();
        code.sourceDllPath = exePath + "/game.dll";
        code.tmpDllPath = exePath + "/game-tmp.dll";
    }

    if (!std::filesystem::exists(code.sourceDllPath)) {
        return;
    }

    auto lastModified = platform_last_modified(code.sourceDllPath);
    if (code.gameCodeDLL != nullptr && lastModified == code.lastModified) {
        return;
    }

    if (code.gameCodeDLL) {
        FreeLibrary(code.gameCodeDLL);
        code.gameCodeDLL = nullptr;
        code.update_and_render = nullptr;
        code.init_resources = nullptr;
    }

    platform_log("Loading game code");

    code.lastModified = lastModified;

    CopyFile(code.sourceDllPath.c_str(), code.tmpDllPath.c_str(), FALSE);
    code.gameCodeDLL = LoadLibraryA(code.tmpDllPath.c_str());
    if (!code.gameCodeDLL) {
        platform_abort(GetLastErrorAsString());
    }

    code.update_and_render = (update_and_render_func *)GetProcAddress(code.gameCodeDLL, "update_and_render");
    if (!code.update_and_render) {
        platform_abort(GetLastErrorAsString());
    }

    code.init_resources = (init_resources_func *)GetProcAddress(code.gameCodeDLL, "init_resources");
    if (!code.init_resources) {
        platform_abort(GetLastErrorAsString());
    }
}
#else
void reload_game_code(GameCode &code) {
    code.sourceDllPath = "./libgame.so";

    if (!std::filesystem::exists(code.sourceDllPath)) {
        return;
    }

    auto lastModified = platform_last_modified(code.sourceDllPath);
    if (code.libraryHandle != nullptr && lastModified == code.lastModified) {
        return;
    }

    if (code.libraryHandle != nullptr) {
        dlclose(code.libraryHandle);
        code.libraryHandle = nullptr;
        code.update_and_render = nullptr;
        code.init_resources = nullptr;
    }

    platform_log("Loading game code");

    code.lastModified = lastModified;
    code.libraryHandle = dlopen(code.sourceDllPath.c_str(), RTLD_LOCAL | RTLD_NOW);
    if (!code.libraryHandle) {
        platform_abort("Couldn't load library: " + std::string(dlerror()));
        return;
    }

    code.update_and_render = (update_and_render_func *)dlsym(code.libraryHandle, "update_and_render");
    if (!code.update_and_render) {
        platform_abort("Couldn't load 'update_and_render' function: " + std::string(dlerror()));
        return;
    }

    code.init_resources = (init_resources_func *)dlsym(code.libraryHandle, "init_resources");
    if (!code.init_resources) {
        platform_abort("Couldn't load 'init_resources' function: " + std::string(dlerror()));
        return;
    }

    auto dlsym_error = dlerror();
    if (dlsym_error) {
        platform_abort("Function loading failed" + std::string(dlsym_error));
        return;
    }
}
#endif
#endif
