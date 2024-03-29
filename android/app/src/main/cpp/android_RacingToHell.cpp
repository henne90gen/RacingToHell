#include "android_RacingToHell.h"

#include <EGL/egl.h>
#include <Resources.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#define MB 1024 * 1024

AAssetManager *asset_manager;
Platform platform = {};
int realWindowWidth, realWindowHeight;
auto previousTime = std::chrono::high_resolution_clock::now();

void platform_log(const std::string &msg) { __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", msg.c_str()); }

void platform_exit() {
    platform_log("Exiting.");
    ::exit(0);
}

void platform_abort(const std::string &msg) {
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s", msg.c_str());
    ::exit(1);
}

File platform_read_file(const std::string &file_path, bool is_resource) {
    if (!is_resource) {
        // TODO implement reading of non-resource files
        platform_log("Reading of non-resource files is not supported on Android");
        return {};
    }

    std::string assetName = file_path.substr(4, file_path.size() - 1);

    platform_log("Reading " + assetName);

    AAsset *asset = AAssetManager_open(asset_manager, assetName.c_str(), AASSET_MODE_STREAMING);

    if (!asset) {
        platform_abort("Couldn't open file " + assetName);
    }

    auto length = (size_t)AAsset_getLength(asset);
    auto content = (char *)std::malloc(length);
    std::memcpy(content, AAsset_getBuffer(asset), length);
    AAsset_close(asset);

    File file = {};
    file.size = length;
    file.name = file_path;
    file.content = content;

    return file;
}

void platform_free_file(File &file) {
    if (file.content) {
        std::free(file.content);
    }

    file.size = 0;
}

int64_t platform_last_modified(const std::string &file_path) {
    // TODO maybe check for existence and only if it exists do the check
    return 0;
}

extern "C" JNIEXPORT void JNICALL Java_game_racingtohell_NativeWrapper_on_1surface_1changed(JNIEnv *env UNUSED,
                                                                                            jclass clazz UNUSED,
                                                                                            jint width, jint height) {
    static bool memoryInitialized = false;
    if (!memoryInitialized) {
        memoryInitialized = true;
        platform.exit = platform_exit;
        platform.abort = platform_abort;
        platform.log = platform_log;
        platform.last_modified = platform_last_modified;
        platform.read_file = platform_read_file;
        platform.free_file = platform_free_file;
        platform.log("Initializing Game");

        init_resources(platform);

        auto procAddress = reinterpret_cast<GLADloadproc>(eglGetProcAddress);
        auto status = gladLoadGLES2Loader(procAddress);
        if (status == 0) {
            platform.abort("Failed to initialize GLAD");
        }

        std::string version = (char *)glGetString(GL_VERSION);
        platform.log("OpenGL Version: " + version);

        int vers[2] = {};
        glGetIntegerv(GL_MAJOR_VERSION, &vers[0]);
        glGetIntegerv(GL_MINOR_VERSION, &vers[1]);
        platform.log("OpenGL ES: " + std::to_string(vers[0]) + "." + std::to_string(vers[1]));
        //        if (vers[0] > 3 || (vers[0] == 3 && vers[1] >= 1)) {
        //            platform.log("OpenGL Version: " + version);
        //        }

        platform.memory.temporaryMemorySize = 10 * MB;
        platform.memory.permanentMemorySize = 100 * MB;
        platform.memory.temporary = (char *)malloc(platform.memory.temporaryMemorySize);
        platform.memory.permanent = (char *)malloc(platform.memory.permanentMemorySize);
        platform.memory.doResize = true;
        platform.memory.aspectRatio = 9.0f / 16.0f;
        platform.memory.base_path = "./";

        platform.log("Initializing Game - Done");
    }

    realWindowWidth = width;
    realWindowHeight = height;
}

extern "C" JNIEXPORT void JNICALL Java_game_racingtohell_NativeWrapper_on_1touch_1event(JNIEnv *env UNUSED,
                                                                                        jclass clazz UNUSED, jfloat x,
                                                                                        jfloat y, jboolean pressed) {
    // Scaling x and y to game-coordinates
    x = x * DEFAULT_WINDOW_WIDTH / realWindowWidth;
    y = y * DEFAULT_WINDOW_HEIGHT / realWindowHeight;

    platform.input.leftKeyPressed = 0;
    platform.input.rightKeyPressed = 0;
    platform.input.upKeyPressed = 0;
    platform.input.downKeyPressed = 0;
    platform.input.shootKeyClicked = 0;
    platform.input.shootKeyPressed = 0;
}

extern "C" JNIEXPORT void JNICALL Java_game_racingtohell_NativeWrapper_on_1draw_1frame(JNIEnv *env UNUSED,
                                                                                       jclass clazz UNUSED) {
    auto now = std::chrono::high_resolution_clock::now();
    auto timeDiff = now - previousTime;
    platform.frameTimeMs = double(timeDiff.count()) / 1000000.0;

    update_and_render(platform);

    previousTime = now;
}

extern "C" JNIEXPORT void JNICALL Java_game_racingtohell_NativeWrapper_init_1asset_1manager(
    JNIEnv *env UNUSED, jclass clazz UNUSED, jobject java_asset_manager) {
    asset_manager = AAssetManager_fromJava(env, java_asset_manager);
}
