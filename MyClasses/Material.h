#ifndef MATERIAL_H
#define MATERIAL_H

#include "glm/glm.hpp"

class Material {

public:

    Material();
    Material(glm::vec3 ambient, glm::vec3 _diffuse, glm::vec3 _specular, float shininess);

private:

    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;

    float _shininess;

public:

    const glm::vec3& ambient() const;
    const glm::vec3& diffuse() const;
    const glm::vec3& specular() const;
    const float shininess() const;

    glm::vec3& ambient();
    glm::vec3& diffuse();
    glm::vec3& specular();
    float shininess();


};

#endif