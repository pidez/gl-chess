#include "Scene.h"

Scene::Scene() {
    _picking_enabled = false;
    _picked_object = -1;
}

Scene::Scene(std::vector<Mesh>& meshes, std::vector<Program>& programs) : _meshes(meshes), _programs(programs) {
    Scene();
}

void Scene::enablePicking() {
    _picking_enabled = true;
}

void Scene::disablePicking() {
    _picking_enabled = false;
}

void Scene::draw() {
    for(unsigned int i = 0; i < _meshes.size(); ++i) {
        _programs.at(i).use();
        _meshes[i].draw(_programs[i]);
        _programs.at(i).unbind();
        
        #ifdef DEBUG
        std::cout << "Scene: Drawing mesh #" << i+1 << std::endl;
        #endif
    }
}