#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "Mesh.h"
#include "Material.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {

public:

    Model(const char* path, int aiArgs);

    void draw(Program& program);

private:
 
    std::vector<Mesh> _meshes;
    std::vector<Texture> texture_loaded;
    std::string directory; ///< directory corrente, utile per trovare i materials

    /*Funzione che carica un modello
     * aiArgs è un parametro bitwise. (aiProcess_Triangulate | aiProcess_FlipUvs | ...) 
     */
    void loadModel(std::string path, int aiArgs);
    void processNode(aiNode* node, const aiScene* scene);


    /*Funzione che usa i dati ottenuti dal file per generare oggetti di tipo Mesh*/
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    /*Funzione che si occupa di caricare i materials definiti nel file del modello*/
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    
    /*Funzione ausiliaria che carica una texture da un file*/
    unsigned int textureFromFile(const char* path, const std::string& directory);
};

#endif