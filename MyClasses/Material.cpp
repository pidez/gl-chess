#include "Material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
    _ambient(ambient), _diffuse(diffuse), _specular(specular), _shininess(shininess) {}

Material::Material() : _ambient(0, 0, 0), _diffuse(0, 0, 0), _specular(0, 0, 0) {}

const glm::vec3& Material::ambient() const {return _ambient;}

const glm::vec3& Material::diffuse() const {return _diffuse;}

const glm::vec3& Material::specular() const {return _specular;}

const float Material::shininess() const {return _shininess;}

glm::vec3& Material::ambient() {return _ambient;}

glm::vec3& Material::diffuse() {return _diffuse;}

glm::vec3& Material::specular() {return _specular;}

float Material::shininess() {return _shininess;}