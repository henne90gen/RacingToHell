#include "Shader.h"

#include "Resources.h"

#include <fmt/core.h>

GLuint compileShader(Platform &platform, const GLenum type, Resource *shader) {
    GLint compileStatus;
    GLuint shaderID = glCreateShader(type);

    const auto shader_content = shader->get_content(platform);
    const auto shader_size = shader_content.size();

    auto shader_data = const_cast<char *>(shader_content.data());
    glShaderSource(shaderID, 1, &shader_data, (GLint *)&shader_size);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE) {
        int infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            char *shaderErrorMessage = new char[infoLogLength + 1];
            glGetShaderInfoLog(shaderID, infoLogLength, nullptr, shaderErrorMessage);
            platform.log(shaderErrorMessage);
        }
        platform.abort(fmt::format("Failed to compile shader: {}\n{}", type, shader_content));
    }

    return shaderID;
}

GLuint linkProgram(Platform &platform, const GLuint vertex_shader, const GLuint fragment_shader) {
    GLuint programID = glCreateProgram();
    GLint linkStatus;

    glAttachShader(programID, vertex_shader);
    glAttachShader(programID, fragment_shader);
    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE) {
        int infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            char *programErrorMessage = new char[infoLogLength + 1];
            glGetProgramInfoLog(programID, infoLogLength, nullptr, programErrorMessage);
            platform.log(programErrorMessage);
        }
        platform.abort("Failed to link shader program.");
    }

    return programID;
}

GLuint buildProgram(Platform &platform, Resource *vertex_shader, Resource *fragment_shader) {
    platform.log("Compiling shaders...");
    GLuint vertex_shader_id = compileShader(platform, GL_VERTEX_SHADER, vertex_shader);
    GLuint fragment_shader_id = compileShader(platform, GL_FRAGMENT_SHADER, fragment_shader);
    return linkProgram(platform, vertex_shader_id, fragment_shader_id);
}
