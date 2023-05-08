#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint>

class Shader {
    public:
        Shader(std::string vertexFile, std::string fragmentFile);
        void start();
        void stop();
    private:
        uint32_t compileShader(std::string shaderFilePath, GLenum shaderType);
        void bindAllAttributes();
    public:
        uint32_t shaderProgramID;
    private:
        uint32_t vertexShaderID;
        uint32_t fragmentShaderID;
};

#endif