#include "android_RacingToHell.h"

#include "RacingToHell.cpp"

extern "C"
JNIEXPORT void JNICALL
Java_game_racingtohell_NativeWrapper_on_1surface_1changed(JNIEnv *env UNUSED, jclass clazz UNUSED,
                                                          jint width, jint height) {
    if (!memoryInitialized) {
        rth_log("Initializing Game");
        memory = {};
        memory.log = rth_log;
        memory.abort = abort;
        memory.exitGame = exitGame;
        memory.readFile = readFile;
        memory.freeFile = freeFile;
        memory.temporaryMemorySize = 10 * 1024 * 1024;
        memory.permanentMemorySize = 100 * 1024 * 1024;
        memory.temporary = (char *) malloc(memory.temporaryMemorySize);
        memory.permanent = (char *) malloc(memory.permanentMemorySize);

        char vertexShaderFileName[] = "./res/shaders/vertex.glsl";
        for (unsigned i = 0; i < sizeof(vertexShaderFileName) / sizeof(char); i++) {
            memory.shaderFileNames[0][i] = vertexShaderFileName[i];
        }
        char fragmentShaderFileName[] = "./res/shaders/fragment.glsl";
        for (unsigned i = 0; i < sizeof(fragmentShaderFileName) / sizeof(char);
             i++) {
            memory.shaderFileNames[1][i] = fragmentShaderFileName[i];
        }

        memory.doResize = true;
        memory.aspectRatio = 16.0f / 9.0f;

        memoryInitialized = true;
    }

    // cause shaders to recompile
    memory.shaderModTimes[0][0] = 0;
    memory.shaderModTimes[0][1] = 1;
    memory.shaderModTimes[1][0] = 0;
    memory.shaderModTimes[1][1] = 1;

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

    GameState *gameState = getGameState(&memory);
    gameState->rotationAngle = PI / 2;

    updateAndRender(&gameInput, &memory);

    memory.shaderModTimes[0][0] = 0;
    memory.shaderModTimes[0][1] = 0;
    memory.shaderModTimes[1][0] = 0;
    memory.shaderModTimes[1][1] = 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_game_racingtohell_NativeWrapper_init_1asset_1manager(JNIEnv *env UNUSED, jclass clazz UNUSED,
                                                          jobject java_asset_manager) {
    asset_manager = AAssetManager_fromJava(env, java_asset_manager);
}

ABORT(abort) {
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s", message.c_str());
    exit(1);
}

LOG(rth_log) {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", message.c_str());
}

READ_FILE(readFile) {
    std::string assetName = "";
    for (int i = 6; i < fileName.length(); i++) {
        assetName += fileName[i];
    }

    rth_log("Reading " + assetName);

    AAsset *asset = AAssetManager_open(asset_manager, assetName.c_str(), AASSET_MODE_STREAMING);

    if (!asset) {
        abort("Couldn't open file " + fileName);
    }

    const char *content;
    size_t length = (size_t) AAsset_getLength(asset);
    content = (char *) AAsset_getBuffer(asset);

    File file = {};
    file.size = length;
    file.name = fileName;
    file.content = (char *) content;
    file.fileHandle = asset;

    return file;
}

FREE_FILE(freeFile) {
    if (file->content) {
        AAsset_close((AAsset *) file->fileHandle);
    }

    file->size = 0;
}

EXIT_GAME(exitGame) {
    rth_log("Exiting.");
    exit(0);
}