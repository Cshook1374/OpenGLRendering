#include "Model.hpp"

Model::Model(std::vector<float> positions, std::vector<float> textures, std::vector<uint32_t> indices)
    :vertexCount{ indices.size() } {
        glGenVertexArrays(1, &this->vaoID);
        glBindVertexArray(this->vaoID);
        bindIndexBuffer(indices);
        storeDataIntoAttributeList(0, 3, positions);
        storeDataIntoAttributeList(1, 2, textures);
        glBindVertexArray(0);
}

void Model::bindIndexBuffer(std::vector<uint32_t> indices) {
    uint32_t eboID;
    glGenBuffers(1, &eboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.front()), &indices.front(), GL_STATIC_DRAW);
}

void Model::storeDataIntoAttributeList(uint32_t attribIndex, uint32_t coordinateSize, std::vector<float> data) {
    uint32_t vboID;
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data.front()), &data.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(attribIndex, coordinateSize, GL_FLOAT, false, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}