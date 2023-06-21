#include "Scene.h"
#include "GL/freeglut.h"

Scene::Scene() {
    #ifdef SCENE_DEBUG
    std::cout << "Scene: Scene() called." << std::endl;
    #endif
    _picking_enabled = false;
    _picked_object = -1;
}

Scene::Scene(std::vector<Mesh>& meshes, std::vector<Program>& programs) {
    *this = Scene();
    _meshes = meshes;
    _programs = programs;
 }

void Scene::setCamera(const Camera& camera) {
    _camera = camera;
 }

 Camera& Scene::camera(){
    return _camera;
 }

void Scene::enablePicking() {
    _picking_enabled = true;
    _picked_object = -1;
}

void Scene::disablePicking() {
    _picking_enabled = false;
    _picked_object = -1;
}

bool Scene::pickingEnabled() {
    return _picking_enabled;
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

void Scene::rotateSelected(glm::vec3 axe, float angle) {
    if(_picking_enabled && _picked_object != -1) {
        //generate and set transform
    }
}

void Scene::translateSelected(glm::vec3 t) {
    if(_picking_enabled && _picked_object >= 0) {
        std::cout << "ciso";
        glm::mat4 currentModel = _meshes.at(_picked_object).model();
        _meshes.at(_picked_object).setModelTransform(glm::translate(currentModel, t));
    }
}

void Scene::draw() {

    #ifdef SCENE_DEBUG
    std::cout << "### DRAWING SCENE ###" << std::endl;
    #endif
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    //Controllo se esiste un oggetto selezionato
    for(unsigned int i = 0; i < _meshes.size(); ++i) {

        if(i == _picked_object) {

            //Utilizzo uno shading program adeguato per l'oggetto selezionato
            Shader fragmentShader(GL_FRAGMENT_SHADER);
            fragmentShader.compile("shaders/selected_frag.frag");

            Program program;

            program.addShader(fragmentShader);
            for(Shader s : _programs.at(i).shaders()) {
                if (s.type() == GL_VERTEX_SHADER) {
                    program.addShader(s);
                }
            }

            program.link();

            program.use();
            _programs.at(i).setMat4("view", _camera.viewTransform());
            _programs.at(i).setMat4("projection", _camera.projection());

            _meshes.at(i).drawSelected(program);

            program.unbind();
            
            #ifdef SCENE_DEBUG
            std::cout << "Scene: Drawing mesh #" << i << "(selected)" <<  std::endl;
            #endif
        }

        //Altrimenti non ci sono oggetti selezionati e disgeno tutto normalmente
        else {
            _programs.at(i).use();

            _programs.at(i).setMat4("view", _camera.viewTransform());
            _programs.at(i).setMat4("projection", _camera.projection());

            _meshes[i].draw(_programs[i]);
            
            _programs.at(i).unbind();
            
            #ifdef SCENE_DEBUG
            std::cout << "Scene: Drawing mesh #" << i << " with program " << i << std::endl;
            #endif
        }
    }
}

void Scene::drawPicking() {

    /*Questa viene chiamata solo quando avviene un click.
    Il click avviene, dunque renderizzo in modalità picking 
    e guardo dove ho clickato. poi grazie alla postRedisplay
    parte una nuova chiamata a draw con le info di picking in
    _picked_object*/

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader pickingVertShader(GL_VERTEX_SHADER);
    Shader pickingFragShader(GL_FRAGMENT_SHADER);
    pickingVertShader.compile("shaders/picking/picking_vert.vert");
    pickingFragShader.compile("shaders/picking/picking_frag.frag");

    Program pickingProgram;
    pickingProgram.addShader(pickingVertShader);
    pickingProgram.addShader(pickingFragShader);
    pickingProgram.link();
    
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
        std::cout << "Picking color for mesh #" << i << ": " << r << " " << g << " " << b << std::endl; 
        #endif

        /* Disegno la mesh i del colore i */
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), 4.0f/3.0f, 0.1f, 100.0f); //Questa info sarà nella camera
        pickingProgram.use();
        pickingProgram.setVec3("pickingColor", _colors.at(i));
        pickingProgram.setMat4("view", _camera.viewTransform());
        pickingProgram.setMat4("projection", projection);
        _meshes.at(i).draw(pickingProgram);
        pickingProgram.unbind();

    }
        unsigned char data[4];

        glFinish();
        glReadPixels(_last_x, glutGet(GLUT_WINDOW_HEIGHT) - _last_y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

        int r = (int)data[0];
        int g = (int)data[1];
        int b = (int)data[2];

        #ifdef SCENE_DEBUG
        std::cout << "color at " << _last_x << " " <<  _last_y << ": " << r << " " << g << " " << b << std::endl;
        #endif

        //converto il colore all'id corrispondente
        if (r == 255 && g == 255 && b == 255)
            _picked_object = -1; //Ho clickato lo sfondo

        else {
            //ogni canale colore sta in 8 bit. 0->8 8->16 16->24
            int pickedID = r + g * 256 + b * 256 * 256; 

            //ottengo la mesh su cui è avvenuto il click
            _picked_object = pickedID;
        }
        #ifdef SCENE_DEBUG
        std::cout << "Clicked on mesh #" << _picked_object << std::endl;
        #endif
}