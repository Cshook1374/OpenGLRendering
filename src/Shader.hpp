#ifndef SHADER_HPP_
#define SHADER_HPP_

#include "Transform.hpp"
#include "Camera.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint>

class Shader {
    public:
        Shader(Transform transform, Camera camera, std::string vertexFile, std::string fragmentFile);
        void start();
        void stop();
    private:
        uint32_t compileShader(std::string shaderFilePath, GLenum shaderType);
        void getAllUniformLocations();
        void bindAllAttributes();
    private:
        void loadMatrix(uint32_t location, glm::mat4 matrix);
    public:
        uint32_t shaderProgramID;
    private:
        uint32_t vertexShaderID;
        uint32_t fragmentShaderID;
    private:
        uint32_t location_modelMatrix;
        uint32_t location_viewMatrix;
    private:
        Transform transform;
        Camera camera;
};

#endif