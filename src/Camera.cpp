#include "Camera.hpp"
#include <glm/ext/quaternion_geometric.hpp>

Camera::Camera(float FOV, float nearClip, float farClip) 
    : FOV{ FOV }, nearClip{ nearClip }, farClip{ farClip }, cameraPosition{ glm::vec3(0, 0, 0) }, cameraTarget{ glm::vec3(0, 0, -1) }, worldUp{ glm::vec3(0, 1, 0) } {
        //TODO: Constructor Implementation
}

void Camera::calculateMatrices() {
    this->cameraDirection = glm::normalize(this->cameraPosition - this->cameraTarget);
    this->cameraRight = glm::normalize(glm::cross(this->cameraDirection, this->worldUp));
    this->cameraUp = glm::cross(this->cameraRight, this->cameraDirection);
    this->viewMatrix = glm::mat4(1);
    this->viewMatrix = glm::lookAt(this->cameraPosition, this->cameraDirection, this->cameraUp);
}