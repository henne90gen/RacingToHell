#include "OpenGL.h"

GLuint loadTexture(
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
setTexturePixels(GLuint texture_object_id, VideoBuffer *videoBuffer) {
    glBindTexture(GL_TEXTURE_2D, texture_object_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, videoBuffer->width, videoBuffer->height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, videoBuffer->content);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint compileShader(const GLenum type, const GLchar *source, const GLint length) {
    GLuint shader_object_id = glCreateShader(type);
    GLint compile_status;

    glShaderSource(shader_object_id, 1, (const GLchar **) &source, &length);
    glCompileShader(shader_object_id);
    glGetShaderiv(shader_object_id, GL_COMPILE_STATUS, &compile_status);

    return shader_object_id;
}

GLuint linkProgram(const GLuint vertex_shader, const GLuint fragment_shader) {
    GLuint program_object_id = glCreateProgram();
    GLint link_status;

    glAttachShader(program_object_id, vertex_shader);
    glAttachShader(program_object_id, fragment_shader);
    glLinkProgram(program_object_id);
    glGetProgramiv(program_object_id, GL_LINK_STATUS, &link_status);

    return program_object_id;
}

GLuint buildProgram() {

    // FIXME don't hard code the shaders
    const GLchar *vertex_shader_source = "attribute vec4 a_Position;attribute vec2 a_TextureCoordinates;varying vec2 v_TextureCoordinates;void main() {v_TextureCoordinates = a_TextureCoordinates;gl_Position = a_Position;}";
    const GLint vertex_shader_source_length = 179;
    const GLchar *fragment_shader_source = "precision mediump float;uniform sampler2D u_TextureUnit;varying vec2 v_TextureCoordinates;void main() {gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);}";
    const GLint fragment_shader_source_length = 166;

    return buildProgram(vertex_shader_source, vertex_shader_source_length, fragment_shader_source,
                        fragment_shader_source_length);
}

GLuint buildProgram(const GLchar *vertex_shader_source, const GLint vertex_shader_source_length,
                    const GLchar *fragment_shader_source,
                    const GLint fragment_shader_source_length) {

    GLuint vertex_shader = compileShader(
            GL_VERTEX_SHADER, vertex_shader_source, vertex_shader_source_length);
    GLuint fragment_shader = compileShader(
            GL_FRAGMENT_SHADER, fragment_shader_source, fragment_shader_source_length);
    return linkProgram(vertex_shader, fragment_shader);
}


GLuint buildProgramFromAssets(const char *vertex_shader_path, const char *fragment_shader_path) {
    const File vertex_shader_source = readFile(vertex_shader_path);
    const File fragment_shader_source = readFile(fragment_shader_path);
    return buildProgram(
            vertex_shader_source.content, vertex_shader_source.size,
            fragment_shader_source.content, fragment_shader_source.size);
}

GLuint createVertexBufferObject(const GLsizeiptr size, const GLvoid *data, const GLenum usage) {
    GLuint vbo_object;
    glGenBuffers(1, &vbo_object);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_object);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo_object;
}

GLuint buildTriangleProgram() {
    // FIXME don't hard code the shaders
    // FIXME do alpha blending in the fragment shader
    const GLchar *vertex_shader_source = "attribute vec4 vPosition;void main(){gl_Position = vPosition;}";
    const GLint vertex_shader_source_length = 63;
    const GLchar *fragment_shader_source = "precision mediump float;void main(){gl_FragColor = vec4(1.0, 0.0, 0.0, 0.5);}";
    const GLint fragment_shader_source_length = 77;

    return buildProgram(vertex_shader_source, vertex_shader_source_length,
                        fragment_shader_source, fragment_shader_source_length);
}

void setupOpenGL(VideoBuffer *videoBuffer) {
    texture = loadTexture(videoBuffer->width, videoBuffer->height, GL_RGBA, videoBuffer->content);
    buffer = createVertexBufferObject(sizeof(rect), rect, GL_STATIC_DRAW);

    program = buildProgram();

    a_position_location = glGetAttribLocation(program, "a_Position");
    a_texture_coordinates_location = glGetAttribLocation(program, "a_TextureCoordinates");
    u_texture_unit_location = glGetUniformLocation(program, "u_TextureUnit");
}

void swapBuffers(VideoBuffer *videoBuffer) {
    setTexturePixels(texture, videoBuffer);

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
