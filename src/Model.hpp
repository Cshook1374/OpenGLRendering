#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <GL/glew.h>

#include <vector>

class Model {
    public:
        Model(std::vector<float> positions, std::vector<float> textures, std::vector<uint32_t> indices);
    private:
        void bindIndexBuffer(std::vector<uint32_t> indices);
        void storeDataIntoAttributeList(uint32_t attribIndex, uint32_t coordinateSize, std::vector<float> data);
    public:
        uint32_t vaoID;
        uint64_t vertexCount;
};

#endif