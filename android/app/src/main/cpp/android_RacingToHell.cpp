#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <android/asset_manager_jni.h>
#include <stdlib.h>
#include "RacingToHell.h"
#include "OpenGL.h"

#define UNUSED  __attribute__((unused))
#define LOG_TAG "RacingToHell"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

static AAssetManager *asset_manager;
static GameMemory memory;
static VideoBuffer videoBuffer;
static bool initialized = false;

extern "C"
JNIEXPORT void JNICALL
Java_game_racingtohell_NativeWrapper_on_1surface_1changed(JNIEnv *env UNUSED, jclass clazz UNUSED,
                                                          jint width, jint height) {
    LOGI("Surface changed");
    if (!initialized) {
        memory = {};
        memory.temporaryMemorySize = 10 * 1024 * 1024;
        memory.permanentMemorySize = 100 * 1024 * 1024;
        memory.temporary = (char *) malloc(memory.temporaryMemorySize);
        memory.permanent = (char *) malloc(memory.permanentMemorySize);

        videoBuffer.width = WINDOW_WIDTH;
        videoBuffer.height = WINDOW_HEIGHT;
        videoBuffer.bytesPerPixel = 4;
        videoBuffer.content = malloc(
                videoBuffer.bytesPerPixel * videoBuffer.width * videoBuffer.height);

        initialized = true;
    }

    setupOpenGL(&videoBuffer);
}

extern "C"
JNIEXPORT void JNICALL
Java_game_racingtohell_NativeWrapper_on_1draw_1frame(JNIEnv *env UNUSED, jclass clazz UNUSED) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Input input = {};
    updateAndRender(&videoBuffer, &input, &memory);

    swapBuffers(&videoBuffer);
}

extern "C"
JNIEXPORT void JNICALL
Java_game_racingtohell_NativeWrapper_init_1asset_1manager(JNIEnv *env UNUSED, jclass clazz UNUSED,
                                                          jobject java_asset_manager) {
    asset_manager = AAssetManager_fromJava(env, java_asset_manager);
}

void abort(std::string message) {
    LOGI("%s", message.c_str());
    exit(1);
}

File readFile(std::string fileName) {

    std::string assetName = "";
    for (int i = 6; i < fileName.length(); i++) {
        assetName += fileName[i];
    }

    LOGI("Reading %s\n", assetName.c_str());
    AAsset *asset = AAssetManager_open(asset_manager, assetName.c_str(), AASSET_MODE_STREAMING);

    if (!asset) {
        abort("Couldn't open file " + fileName);
    }

    File file = {};
    file.size = (size_t) AAsset_getLength(asset);
    file.name = fileName;
    file.content = (char *) AAsset_getBuffer(asset);
    file.fileHandle = asset;

    return file;

}

void freeFile(File *file) {
    if (file->content) {
        AAsset_close((AAsset *) file->fileHandle);
    }

    file->size = 0;
}
