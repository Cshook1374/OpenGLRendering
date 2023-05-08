#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include "Model.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>

class Renderer {
    public:
        void prepare(glm::vec4 backgroundColor);
        void render(const Model& model);
};

#endif