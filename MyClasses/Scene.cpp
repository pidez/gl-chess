#include "Scene.h"

#include "GL/freeglut.h"

#include <fstream>

Scene::Scene() {
    #ifdef SCENE_DEBUG
    std::cout << "Scene: Scene() called." << std::endl;
    #endif
    _picking_enabled = false;
    _picked_object = 0; //torre selezionata per test
}

Scene::Scene(std::vector<Mesh>& meshes, std::vector<Program>& programs) {
    *this = Scene();
    _meshes = meshes;
    _programs = programs;
 }

void Scene::setCamera(const Camera& camera) {
    _camera = camera;
 }


 Camera& Scene::camera() {
    return _camera;
 }

void Scene::enablePicking() {
    _picking_enabled = true;
}

void Scene::disablePicking() {
    _picking_enabled = false;
}

void Scene::mousePicking(int x, int y) {

    if(_picking_enabled) {

        #ifdef SCENE_DEBUG
        std::cout <<  "reading color at " << x << " " << y << std::endl;
        #endif

        _last_x = x;
        _last_y = y;

        drawPicking();
    }
}

void Scene::draw() {

    #ifdef SCENE_DEBUG
    std::cout << "### DRAWING SCENE ###" << std::endl;
    #endif

    if(_picking_enabled) {
        drawPicking();
    }
    else {
        drawNoPicking();
    }
}

void Scene::drawNoPicking() {
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(unsigned int i = 0; i < _meshes.size(); ++i) {
        _programs.at(i).use();

        _programs.at(i).setMat4("view", _camera.viewTransform());
        _programs.at(i).setMat4("projection", _camera.projection());

        _meshes[i].draw(_programs[i]);
        
        _programs.at(i).unbind();
        
        #ifdef DEBUG
        std::cout << "Scene: Drawing mesh #" << i+1 << std::endl;
        #endif
    }
}

void Scene::drawPicking() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader pickingVertShader(GL_VERTEX_SHADER);
    Shader pickingFragShader(GL_FRAGMENT_SHADER);
    pickingVertShader.compile("shaders/picking/picking_vert.vert");
    pickingFragShader.compile("shaders/picking/picking_frag.frag");

    Program pickingProgram;
    pickingProgram.addShader(pickingVertShader);
    pickingProgram.addShader(pickingFragShader);
    pickingProgram.link();
    
    glClearColor(1, 1, 1, 1);

    /* Primo giro di rendering: colore univoco per ogni oggetto */
    for (unsigned int i = 0; i < _meshes.size(); ++i) {
        /* Ottengo il colore univoco di ogni mesh tramite un'operazione bitwise fra l'indice della mesh
        nell'array. con FF identifico 256 indici, quindi con 3 canali ottengo 256^3 triplette (colori) univoche.
        infine shifto tutto a destra per stare nel range 0-255. */
        int r = (i & 0x0000FF) >> 0; //(i+1)*100 solo per vedere effettivamente qualcosa.
        int g = (i & 0x00FF00) >> 8;
        int b = (i & 0xFF0000) >> 16;
        
        _colors.push_back(glm::vec3(r/255.0, g/255.0, b/255.0));

        #ifdef SCENE_DEBUG
        std::cout << "Picking color for mesh #" << i+1 << ": " << r << " " << g << " " << b << std::endl; 
        #endif

        /* Disegno la mesh i del colore i */
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), 4.0f/3.0f, 0.1f, 100.0f); //Questa info sarà nella camera
        pickingProgram.use();
        pickingProgram.setVec3("pickingColor", _colors.at(i));
        pickingProgram.setMat4("view", _camera.viewTransform());
        pickingProgram.setMat4("projection", projection);
        _meshes.at(i).draw(pickingProgram);
        pickingProgram.unbind();

        char data[4];

        glFinish();
        glReadPixels(_last_x, glutGet(GLUT_WINDOW_HEIGHT) - _last_y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

        #ifdef SCENE_DEBUG
        std::cout << "color at " << _last_x << " " <<  _last_y << ": " << (int)data[0] << " " << (int)data[1] << " " << (int)data[2] << std::endl;
        #endif


        //converto il colore all'id corrispondente
        int pickedID = (int)data[0] + data[1] * 256 + data[2] * 256 * 256; //ogni canale colore sta in 8 bit. 0->8 8->16 16->24

        //ottengo la mesh su cui è avvenuto il click
        _picked_object = pickedID;

        #ifdef SCENE_DEBUG
        std::cout << "Clicked on mesh #" << pickedID << std::endl;
        #endif
    }

    drawNoPicking();
}