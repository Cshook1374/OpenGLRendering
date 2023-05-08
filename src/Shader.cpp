#include "Shader.hpp"

Shader::Shader(Transform transform, Camera camera, std::string vertexFile, std::string fragmentFile) 
    : transform{ transform }, camera{ camera } {
    this->vertexShaderID = compileShader(vertexFile, GL_VERTEX_SHADER);
    this->fragmentShaderID = compileShader(fragmentFile, GL_FRAGMENT_SHADER);
    this->shaderProgramID = glCreateProgram();
    glAttachShader(this->shaderProgramID, this->vertexShaderID);
    glAttachShader(this->shaderProgramID, this->fragmentShaderID);
    bindAllAttributes();
    glLinkProgram(this->shaderProgramID);
    int32_t success;
    char infoLog[1024];
    glGetProgramiv(this->shaderProgramID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        glGetProgramInfoLog(this->shaderProgramID, 1024, nullptr, infoLog);
        std::cout << "FAILED TO LINK SHADER PROGRAM!\n" << infoLog << "\n";
    }
    getAllUniformLocations();
}

uint32_t Shader::compileShader(std::string shaderFilePath, GLenum shaderType) {
    uint32_t shaderID = glCreateShader(shaderType);
    std::ifstream file(shaderFilePath);
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string shaderCode = ss.str();
    const char* shaderSource = shaderCode.c_str();
    file.close();
    glShaderSource(shaderID, 1, &shaderSource, nullptr);
    glCompileShader(shaderID);
    int32_t success;
    char infoLog[1024];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
        if (shaderType == GL_VERTEX_SHADER) {
            std::cout << "FAILED TO COMPILE VERTEX SHADER!\n" << infoLog << "\n";
        }
        if (shaderType == GL_FRAGMENT_SHADER) {
            std::cout << "FAILED TO COMPILE FRAGMENT SHADER!\n" << infoLog << "\n";
        }
    }
    return shaderID;
}

void Shader::getAllUniformLocations() {
    this->location_modelMatrix = glGetUniformLocation(this->shaderProgramID, "modelMatrix");
    this->location_viewMatrix = glGetUniformLocation(this->shaderProgramID, "viewMatrix");
}

void Shader::bindAllAttributes() {
    glBindAttribLocation(this->shaderProgramID, 0, "position");
    glBindAttribLocation(this->shaderProgramID, 1, "textureCoords");
}

void Shader::loadMatrix(uint32_t location, glm::mat4 matrix) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::start() {
    glUseProgram(this->shaderProgramID);
    loadMatrix(this->location_modelMatrix, this->transform.modelMatrix);
    loadMatrix(this->location_viewMatrix, this->camera.viewMatrix);
}

void Shader::stop() {
    glUseProgram(0);
}