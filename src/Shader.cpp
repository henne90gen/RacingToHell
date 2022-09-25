#include "Shader.h"

GLuint compileShader(GameMemory *memory, const GLenum type,
                     std::string shaderFileName) {
    File shaderFile = memory->readFile(shaderFileName);

    GLint compileStatus;
    GLuint shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, (GLchar **)&shaderFile.content,
                   (GLint *)&shaderFile.size);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE) {
        int infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            char *shaderErrorMessage = new char[infoLogLength + 1];
            glGetShaderInfoLog(shaderID, infoLogLength, NULL,
                               shaderErrorMessage);
            memory->log(shaderErrorMessage);
        }
        std::string message = "Failed to compile shader. " +
                              std::to_string(type) + "\n" +
                              std::string(shaderFile.content);
        memory->abort(message);
    }

    memory->freeFile(&shaderFile);

    return shaderID;
}

GLuint linkProgram(GameMemory *memory, const GLuint vertex_shader,
                   const GLuint fragment_shader) {
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
            glGetProgramInfoLog(programID, infoLogLength, NULL,
                                programErrorMessage);
            memory->log(programErrorMessage);
        }
        std::string message = "Failed to link shader program.\n";
        memory->abort(message);
    }

    return programID;
}

GLuint buildProgram(GameMemory *memory) {
    GLuint vertexShader =
        compileShader(memory, GL_VERTEX_SHADER, memory->shaderFileNames[0]);

    GLuint fragmentShader =
        compileShader(memory, GL_FRAGMENT_SHADER, memory->shaderFileNames[1]);

    return linkProgram(memory, vertexShader, fragmentShader);
}
