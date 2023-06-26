#include "Mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Mesh::Mesh() {
    _modelTransform = glm::mat4(1.0);
    _programIndex = 0;
}


void Mesh::draw(Program& program) {
    
    /* Passaggio delle info sul material al fragment shader */

    program.setMaterial(_material);
    program.setMat4("model", _modelTransform);

    /* Rendering effettivo delle primitive */

    glActiveTexture(GL_TEXTURE0);
    program.setInt("texture_diffuse", 0);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Mesh::drawSelected(Program& program) {
    program.setMat4("model", _modelTransform);

    /* Rendering effettivo delle primitive */
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

bool Mesh::loadMesh(const std::string& path, unsigned int aiArgs) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.c_str(), aiArgs);

    if(scene) {
        return setupMesh(scene, path);
    }
    else {
        std::cout << "Errore nel caricamento della mesh " << path << ": " << importer.GetErrorString() << std::endl;
        return false;
    }
}

bool Mesh::setupMesh(const aiScene* scene, const std::string& path) {

    /* Lettura dei dati su posizione, normale e coordinate dei vertici della mesh */

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const aiMesh* mesh = scene->mMeshes[0];

    #ifdef DEBUG
    std::cout << "Importo modello..." << std::endl;
    #endif

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        
        const aiVector3D* pPos      = &(mesh->mVertices[i]);
        const aiVector3D* pNormal = mesh->HasNormals() ? &(mesh->mNormals[i]) : &Zero3D;
        const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;

        #ifdef POS_NORM_DEBUG
        std::cout << "Position: " << pPos->x << " " << pPos->y << " " << pPos->z << std::endl;
        std::cout << "Normal: " << pNormal->x << " " << pNormal->y << " " << pNormal->z << std::endl;
        std:: cout << "Texture: " << pTexCoord->x << " " << pTexCoord->y << std::endl;
        #endif
    
        Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
                    glm::vec3(pNormal->x, pNormal->y, pNormal->z),
                    glm::vec2(pTexCoord->x, pTexCoord->y));

        _vertices.push_back(v);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces;  ++i) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; ++j) {
            _indices.push_back(face.mIndices[j]);
        }
    } 

    #ifdef DEBUG
    std::cout << "Modello importato." << std::endl;
    #endif

    /* Lettura dei dati sul material della mesh */

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    aiString name;
    aiColor3D diffuseColor;
    aiColor3D ambientColor;
    aiColor3D specularColor;
    float shininess;

    material->Get(AI_MATKEY_NAME, name);
    material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
    material->Get(AI_MATKEY_SHININESS, shininess);

    glm::vec3 diffuse(diffuseColor.r, diffuseColor.g, diffuseColor.b);
    glm::vec3 ambient(ambientColor.r, ambientColor.g, ambientColor.b);
    glm::vec3 specular(specularColor.r, specularColor.g, specularColor.b);

    #ifdef DEBUG
    std::cout << "Loaded material: " << name.C_Str() << std::endl;
    std::cout << "Ka: " << ambientColor.r << " " << ambientColor.g << " " << ambientColor.b << std::endl;
    std::cout << "Kd: " << diffuseColor.r << " " << diffuseColor.g << " " << diffuseColor.b << std::endl;    
    std::cout << "Ks: " << specularColor.r << " " << specularColor.g << " " << specularColor.b << std::endl;
    std::cout << "Shininess: " << shininess << std::endl;
    #endif

    Material mMaterial(ambient, diffuse, specular, shininess);
    _material = mMaterial;

    /*Caricamento della texture */
    /*Per questo progetto sto supponendo di avere sempre una sola texture di tipo diffusivo*/

    aiString str;
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0){
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

        #ifdef DEBUG
        std::cout << "Texture name: " << str.C_Str() << std::endl;
        #endif

        int width, height, channels;

        /*find working directory*/

        std::string dir = path.substr(0, path.find_last_of("/\\") + 1);
        std::string target = dir.append(str.C_Str());

        std::cout << "texture path: " << target << std::endl;

        unsigned char* image = stbi_load(target.c_str(), &width, &height, &channels, 4);

        if(image == nullptr) 
            std::cout << "Scene: Caricamento della texture fallito." << std::endl;


        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D); //Altrimenti non funziona piu un cazzo

        stbi_image_free(image);
    }

    /* Impostazione dello stato openGL */

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

    //Texture Cooridinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, _texcoords)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return true;
}

void Mesh::setModelTransform(const glm::mat4& model) {
    _modelTransform = model;
}

const glm::mat4& Mesh::model() const {
    return _modelTransform;
}

void Mesh::setProgramIndex(unsigned int index) {
    if(index > 0)
        _programIndex = index;
}

int Mesh::getProgramIndex() {
    return _programIndex;
}
