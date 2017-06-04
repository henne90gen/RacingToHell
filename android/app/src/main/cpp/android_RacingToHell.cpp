#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <android/asset_manager_jni.h>
#include "RacingToHell.h"

#define UNUSED  __attribute__((unused))
#define BUFFER_OFFSET(i) ((void*)(i))
#define LOG_TAG "RacingToHell"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

static GLuint texture;
static GLuint buffer;
static GLuint program;

static GLint a_position_location;
static GLint a_texture_coordinates_location;
static GLint u_texture_unit_location;

// position X, Y, texture S, T
static const float rect[] = {-1.0f, -1.0f, 0.0f, 0.0f,
                             -1.0f, 1.0f, 0.0f, 1.0f,
                             1.0f, -1.0f, 1.0f, 0.0f,
                             1.0f, 1.0f, 1.0f, 1.0f};

//static uint32_t *data;
static AAssetManager *asset_manager;
static GameMemory memory;
static VideoBuffer videoBuffer;

extern "C"
JNIEXPORT void JNICALL
Java_game_racingtohell_NativeWrapper_on_1surface_1created(JNIEnv *env, jclass clazz) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    LOGI("Started app and cleared screen");
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
}

GLuint load_texture(
        const GLsizei width, const GLsizei height,
        const GLenum type, const GLvoid *pixels) {
    GLuint texture_object_id;
    glGenTextures(1, &texture_object_id);

    glBindTexture(GL_TEXTURE_2D, texture_object_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture_object_id;
}

void
set_texture_pixels(GLuint texture_object_id, const GLsizei width, const GLsizei height,
                   const GLenum type,
                   const GLvoid *pixels) {
    glBindTexture(GL_TEXTURE_2D, texture_object_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

}

GLuint compile_shader(const GLenum type, const GLchar *source, const GLint length) {
    GLuint shader_object_id = glCreateShader(type);
    GLint compile_status;

    glShaderSource(shader_object_id, 1, (const GLchar **) &source, &length);
    glCompileShader(shader_object_id);
    glGetShaderiv(shader_object_id, GL_COMPILE_STATUS, &compile_status);

    return shader_object_id;
}

GLuint link_program(const GLuint vertex_shader, const GLuint fragment_shader) {
    GLuint program_object_id = glCreateProgram();
    GLint link_status;

    glAttachShader(program_object_id, vertex_shader);
    glAttachShader(program_object_id, fragment_shader);
    glLinkProgram(program_object_id);
    glGetProgramiv(program_object_id, GL_LINK_STATUS, &link_status);

    return program_object_id;
}

GLuint build_program(const GLchar *vertex_shader_source, const GLint vertex_shader_source_length,
                     const GLchar *fragment_shader_source,
                     const GLint fragment_shader_source_length) {
    GLuint vertex_shader = compile_shader(
            GL_VERTEX_SHADER, vertex_shader_source, vertex_shader_source_length);
    GLuint fragment_shader = compile_shader(
            GL_FRAGMENT_SHADER, fragment_shader_source, fragment_shader_source_length);
    return link_program(vertex_shader, fragment_shader);
}

GLuint build_program_from_assets(const char *vertex_shader_path, const char *fragment_shader_path) {
    const File vertex_shader_source = readFile(vertex_shader_path);
    const File fragment_shader_source = readFile(fragment_shader_path);
    const GLuint program_object_id = build_program(
            vertex_shader_source.content, vertex_shader_source.size,
            fragment_shader_source.content, fragment_shader_source.size);

    return program_object_id;
}

GLuint create_vbo(const GLsizeiptr size, const GLvoid *data, const GLenum usage) {
    GLuint vbo_object;
    glGenBuffers(1, &vbo_object);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_object);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo_object;
}

extern "C"
JNIEXPORT void JNICALL
Java_game_racingtohell_NativeWrapper_on_1surface_1changed(JNIEnv *env UNUSED, jclass clazz UNUSED,
                                                          jint width, jint height) {
    LOGI("Surface changed");

    texture = load_texture(videoBuffer.width, videoBuffer.height, GL_RGBA, videoBuffer.content);
    buffer = create_vbo(sizeof(rect), rect, GL_STATIC_DRAW);

    const char *vss = "attribute vec4 a_Position;attribute vec2 a_TextureCoordinates;varying vec2 v_TextureCoordinates;void main() {v_TextureCoordinates = a_TextureCoordinates;gl_Position = a_Position;}";
    const char *fss = "precision mediump float;uniform sampler2D u_TextureUnit;varying vec2 v_TextureCoordinates;void main() {gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);}";
    program = build_program(vss, 179, fss, 166);

    a_position_location = glGetAttribLocation(program, "a_Position");
    a_texture_coordinates_location = glGetAttribLocation(program, "a_TextureCoordinates");
    u_texture_unit_location = glGetUniformLocation(program, "u_TextureUnit");
}

extern "C"
JNIEXPORT void JNICALL
Java_game_racingtohell_NativeWrapper_on_1draw_1frame(JNIEnv *env UNUSED, jclass clazz UNUSED) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Input input = {};
    updateAndRender(&videoBuffer, &input, &memory);

    set_texture_pixels(texture, videoBuffer.width, videoBuffer.height, GL_RGBA,
                       videoBuffer.content);

    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(u_texture_unit_location, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(a_position_location, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT),
                          BUFFER_OFFSET(0));
    glVertexAttribPointer(a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE,
                          4 * sizeof(GL_FLOAT), BUFFER_OFFSET(2 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(a_position_location);
    glEnableVertexAttribArray(a_texture_coordinates_location);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
