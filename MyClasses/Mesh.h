
/* Contiene le informazioni su una mesh, ovvero i dati grezzi, il materiale e la trasformazione di modello
    In questo modo è facile risalire alle informazioni rilevanti quando si vuole effettuare il rendering della mesh */

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glm/glm.hpp"


#include "Program.h"
#include "Material.h"

struct Vertex {
    glm::vec3 _position;
    glm::vec3 _normal;

    Vertex() {}
    Vertex(glm::vec3 position, glm::vec3 normal) : _position(position), _normal(normal) {}

    friend std::ostream& operator<<(std::ostream& os, Vertex v) {
        os << "[Position: (" << v._position.x << ", " << v._position.y << ", " << v._position.z << "), " << 
        "Normal: (" << v._normal.x << ", " << v._normal.y << ", " << v._normal.z << ")]" << std::endl;

        return os; 
    }
};

class Mesh {


public:

    Mesh();

    //Carica una singola mesh da file.
    bool loadMesh(const std::string& path, unsigned int aiArgs);

    void draw(Program& program);

    //Funzione da chiamare per disegnare in modo personalizzato la mesh 
    //Quando è selezionata
    void drawSelected(Program& program);

    void setModelTransform(const glm::mat4& model);
    const glm::mat4& model() const;

private:

    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;

    glm::mat4 _modelTransform;

    unsigned int _vao;
    unsigned int _vbo;
    unsigned int _ebo;

    Material _material;



    //Riempie i vettori _vertices, _indices con i dati della singola mesh letta da file.
    //imposta vao, vbo, ebo con i dati su posizione e normale
    bool setupMesh(const aiScene* scene, const std::string& path);


};

#endif