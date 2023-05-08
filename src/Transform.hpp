#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
    public:
        Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
    public:
        glm::mat4 modelMatrix;
    private:
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
};

#endif