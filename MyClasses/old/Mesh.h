#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <ostream>

#include "glm/glm.hpp"

#include "Program.h"

    struct Vertex {
        glm::vec3 _position;
        glm::vec3 _normal;
        glm::vec2 _texcoords;

        Vertex() {}
        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texcoords) : _position(position), _normal(normal), _texcoords(texcoords)
        {}

        friend std::ostream& operator<<(std::ostream& os, Vertex v) {
            os << "[Position: (" << v._position.x << ", " << v._position.y << ", " << v._position.z << "), " << 
            "Normal: (" << v._normal.x << ", " << v._normal.y << ", " << v._normal.z << "), " << 
            "Texture Coordinates: (" << v._texcoords.x << ", " << v._texcoords.y << ")]" << std::endl;

            return os; 
        }
    };

    struct Texture {
        unsigned int _id;
        std::string _type;
        std::string _path;
    };

class Mesh {
public:


    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void draw(Program& program);

private:
    
    std::vector<Vertex>         _vertices;
    std::vector<unsigned int>   _indices;
    std::vector<Texture>        _textures;

    unsigned int _vao;
    unsigned int _vbo;
    unsigned int _ebo;

    void setupMesh();



};

#endif