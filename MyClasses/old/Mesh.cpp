
#include <iostream>

#include "Mesh.h"

Mesh::Mesh() {

}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) : 
    _vertices(vertices), _indices(indices), _textures(textures) {
        setupMesh();
    }

void Mesh::setupMesh() {

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);

    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    //Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, _normal)));
    glEnableVertexAttribArray(1);

    //Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, _texcoords)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::draw(Program& program) {

    unsigned int nDiffuse = 1;
    unsigned int nSpecular = 1;

    for(int i = 0; i < _textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = _textures[i]._type;
        if(name == "texture_diffuse") {
            number = std::to_string(nDiffuse);
            nDiffuse++;
        }
        else if(name == "texture_specular") {
            number = std::to_string(nSpecular);
            nSpecular++;
        }

        //Si utilizza una convenzione per i nomi delle variabili uniform
        // texture_diffuseN, texture_specularN
        // ovviamente tale condizione deve essere rispettata anche nella stesura dello shader.
        program.setInt(name + number, i);
        glBindTexture(GL_TEXTURE_2D, _textures[i]._id);
    }

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    glBindVertexArray(0);
}

