#include "Transform.hpp"
#include <glm/ext/matrix_transform.hpp>

Transform::Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
    : translation{ translation }, rotation{ rotation }, scale{ scale } {
        this->modelMatrix = glm::mat4(1);
        this->modelMatrix = glm::translate(this->modelMatrix, translation);
        this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.x), glm::vec3(1, 0, 0));
        this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.y), glm::vec3(0, 1, 0));
        this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.z), glm::vec3(0, 0, 1));
        this->modelMatrix = glm::scale(this->modelMatrix, this->scale);
}