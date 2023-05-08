#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <stb/stb_image.h>

#include <GL/glew.h>

#include <iostream>
#include <string>
#include <cstdint>

class Texture {
    public:
        Texture(std::string textureFileName, uint32_t textureIndex);
        void bindTexture();
        void unbindTexture();
    private:
        uint32_t textureID;
};

#endif