#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Model::Model(const char* path, int aiArgs) {
    loadModel(path, aiArgs);
}

void Model::draw(Program& program) {
    for(Mesh mesh : _meshes) {
        mesh.draw(program);
    }
}

void Model::loadModel(std::string path, int aiArgs) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiArgs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Assimp: Errore nell'importazione del modello." << std::endl;
    } else {
        std::cout << "Assimp: Modello importato correttamente." << std::endl;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        glm::vec3 v;

        v.x = mesh->mVertices[i].x;
        v.y = mesh->mVertices[i].y;
        v.z = mesh->mVertices[i].z;

        vertex._position = v;

        if(mesh->HasNormals()) {
            v.x = mesh->mNormals[i].x;
            v.y = mesh->mNormals[i].y;
            v.z = mesh->mNormals[i].z;
            
            vertex._normal = v;
        }

        else {
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = 0.0f;

            vertex._normal = v;
        }

        if(mesh->mTextureCoords[0]) {
            glm::vec2 v2;

            v2.x = mesh->mTextureCoords[0][i].x;
            v2.y = mesh->mTextureCoords[0][i].y;

            vertex._texcoords = v2;
        } else {
            vertex._texcoords = glm::vec2(0.0f, 0.0f);
        }
        //std::cout << vertex;
        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    //diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    //specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    //Eventualmente normal maps, height maps

    //ottengo il materiale della mesh corrente

    aiColor3D diffuseColor;
    aiColor3D ambientColor;
    aiColor3D specularColor;

    material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);

    std::cout << "ambient color: " << ambientColor.r << " " << ambientColor.g << " " << ambientColor.b << std::endl;
    

    
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
        aiString str;
        material->GetTexture(type, i, &str);

        bool skip = false;
        for(unsigned int j = 0; j < texture_loaded.size(); ++j) {
            if(std::strcmp(texture_loaded[j]._path.data(), str.C_Str()) == 0) {
                textures.push_back(texture_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip) {
            Texture texture;
            texture._id = textureFromFile(str.C_Str(), this->directory);
            texture._type = typeName;
            texture._path = str.C_Str();
            textures.push_back(texture);
            texture_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::textureFromFile(const char* path, const std::string& directory) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}