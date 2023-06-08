#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"

class Light {

    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;

    Light();
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};

#endif