#define STB_IMAGE_IMPLEMENTATION
#include "Texture.hpp"

Texture::Texture(std::string textureFileName, uint32_t textureIndex) {
    if (textureIndex > 31) {
        std::cout << "TEXTURE " << textureFileName << "INDEX EXCEEDS VALID VALUES (0 - 31)!\n";
    }
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFileName.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "FAILED TO LOAD TEXTURE RESOURCE: " << textureFileName << "\n";
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void Texture::bindTexture() {
    glBindTexture(GL_TEXTURE_2D, this->textureID);
}

void Texture::unbindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}