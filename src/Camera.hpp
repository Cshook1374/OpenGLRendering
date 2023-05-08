#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
    public:
        Camera(float FOV, float nearClip, float farClip);
        void calculateMatrices();
    public:
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
    public:
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 worldUp;
    private:
        float FOV;
        float nearClip;
        float farClip;
        glm::vec3 cameraDirection;
        glm::vec3 cameraRight;
        glm::vec3 cameraUp;
};

#endif