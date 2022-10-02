#include "android_RacingToHell.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#define MB 1024 * 1024

AAssetManager *asset_manager;
Platform platform = {};
Input gameInput = {};
int realWindowWidth, realWindowHeight;

extern "C"
JNIEXPORT void JNICALL
Java_game_racingtohell_NativeWrapper_on_1surface_1changed(JNIEnv *env UNUSED, jclass clazz UNUSED,
                                                          jint width, jint height) {
    static bool memoryInitialized = false;
    if (!memoryInitialized) {
        memoryInitialized = true;

        platform.log("Initializing Game");
        platform.memory.temporaryMemorySize = 10 * MB;
        platform.memory.permanentMemorySize = 100 * MB;
        platform.memory.temporary = (char *) malloc(platform.memory.temporaryMemorySize);
        platform.memory.permanent = (char *) malloc(platform.memory.permanentMemorySize);
        platform.memory.doResize = true;
        platform.memory.aspectRatio = 16.0f / 9.0f;
        platform.memory.base_path = "./";
    }

    realWindowWidth = width;
    realWindowHeight = height;
}

extern "C"
JNIEXPORT void JNICALL
Java_game_racingtohell_NativeWrapper_on_1touch_1event(JNIEnv *env UNUSED, jclass clazz UNUSED,
                                                      jfloat x, jfloat y, jboolean pressed) {
    // Scaling x and y to game-coordinates
    x = x * DEFAULT_WINDOW_WIDTH / realWindowWidth;
    y = y * DEFAULT_WINDOW_HEIGHT / realWindowHeight;

    gameInput.leftKeyPressed = 0;
    gameInput.rightKeyPressed = 0;
    gameInput.upKeyPressed = 0;
    gameInput.downKeyPressed = 0;
    gameInput.shootKeyClicked = 0;
    gameInput.shootKeyPressed = 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_game_racingtohell_NativeWrapper_on_1draw_1frame(JNIEnv *env UNUSED, jclass clazz UNUSED) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GameState *gameState = getGameState(platform);
    gameState->rotationAngle = PI / 2;

    platform.input = &gameInput;
    update_and_render(platform);
}

extern "C"
JNIEXPORT void JNICALL
Java_game_racingtohell_NativeWrapper_init_1asset_1manager(JNIEnv *env UNUSED, jclass clazz UNUSED,
                                                          jobject java_asset_manager) {
    asset_manager = AAssetManager_fromJava(env, java_asset_manager);
}

void Platform::abort(const std::string &msg) {
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s", msg.c_str());
    ::exit(1);
}

void Platform::log(const std::string &msg) {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", msg.c_str());
}

File Platform::read_file(const std::string &file_path, bool is_resource) {
    std::string assetName = file_path;

    log("Reading " + assetName);

    AAsset *asset = AAssetManager_open(asset_manager, assetName.c_str(), AASSET_MODE_STREAMING);

    if (!asset) {
        abort("Couldn't open file " + file_path);
    }

    auto length = (size_t) AAsset_getLength(asset);
    auto content = (char *) std::malloc(length);
    std::memcpy(content, AAsset_getBuffer(asset), length);
    AAsset_close(asset);

    File file = {};
    file.size = length;
    file.name = file_path;
    file.content = content;

    return file;
}

void Platform::free_file(File &file) {
    if (file.content) {
        std::free(file.content);
    }

    file.size = 0;
}

void Platform::exit() {
    log("Exiting.");
    ::exit(0);
}
