#pragma once
#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <android/asset_manager_jni.h>
#include <stdlib.h>
#include <cstdlib>
#include <RacingToHell.h>
#include "OpenGL.h"

#define UNUSED  __attribute__((unused))
#define LOG_TAG "RacingToHell"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

static AAssetManager *asset_manager;
static GameMemory memory;
static VideoBuffer videoBuffer;
static Input gameInput = {};
static bool initialized = false;
static int realWindowWidth, realWindowHeight;
