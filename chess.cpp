#include <iostream>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "MyClasses/stb_image.h"

#include "MyClasses/Mesh.h"
#include "MyClasses/Shader.h"
#include "MyClasses/Program.h"
#include "MyClasses/Camera.h"
#include "MyClasses/Scene.h"

GLuint window;
Camera mainCamera;

Program* rookProgram;
Program* sphereProgram;

Mesh* rook;
Mesh* sphere;

Scene *scene;

void init(int* argc, char** argv);

//glut callbacks
void keyboardFunction(unsigned char key, int x, int y);
void displayFunc();
void motionFunc(int x, int y);
void mouseFunc(int button, int state, int x, int y);

int main(int argc, char** argv) {
	init(&argc, argv);

	/* Inizializzazione dei programmi di shading */

	rookProgram = new Program();
	sphereProgram = new Program();

	Shader rookVertexShader(GL_VERTEX_SHADER);
	Shader rookFragmentShader(GL_FRAGMENT_SHADER);
	Shader sphereVertexShader(GL_VERTEX_SHADER);
	Shader sphereFragmentShader(GL_FRAGMENT_SHADER);

	rookVertexShader.compile("shaders/vertex_shader.vert");
	rookFragmentShader.compile("shaders/fragment_shader.frag");
	sphereVertexShader.compile("shaders/sphere_vert.vert");
	sphereFragmentShader.compile("shaders/sphere_frag.frag");

	rookProgram->addShader(rookVertexShader);
	rookProgram->addShader(rookFragmentShader);
	sphereProgram->addShader(sphereVertexShader);
	sphereProgram->addShader(sphereFragmentShader);

	sphereProgram->link();
	rookProgram->link();

	/* Inizializzazione delle mesh */
	rook = new Mesh();
	sphere = new Mesh();

	sphere->loadMesh("models/Sphere.obj", aiProcess_Triangulate);
	rook->loadMesh("models/rook/rook.obj", aiProcess_Triangulate);
	rook->setModelTransform(glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(10, 10, 10)), glm::vec3(0.2, -0.2, 0)));
	sphere->setModelTransform(glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5)));
	
	/* Inizializzazione della scens */

	std::vector<Mesh> meshes;
	meshes.push_back(*rook);
	meshes.push_back(*sphere);

	std::vector<Program> programs;
	programs.push_back(*rookProgram);
	programs.push_back(*sphereProgram);

	scene = new Scene(meshes, programs);
	scene->setCamera(mainCamera);

		/* Imposto gli shader per il rendering della torre
	PER PASSARE I VALORI AGLI SHADER E' NECESSARIO AVER CHIAMATO glUseProgram(id) */
	rookProgram->use();
	
	/* Trasformazioni */

	/* Posizione della camera */
	
	/* Informazioni sulla luce */
	rookProgram->setLightColor(glm::vec3(1, 1, 1));
	rookProgram->setLightIntensity(1);
	rookProgram->setLightPosition(glm::vec3(0, 0, 1));

	rookProgram->unbind();
	
	/* Imposto gli shader per il rendering della sfera */
	sphereProgram->use();
	
	/* Trasformazioni */
	
	/* Posizione della camera */

	/* Informazioni sulla luce */
	sphereProgram->setLightColor(glm::vec3(1, 1, 1));
	sphereProgram->setLightIntensity(1);
	sphereProgram->setLightPosition(glm::vec3(0, 0, 1));

	sphereProgram->unbind();

	scene->enablePicking();

    glutMainLoop();
}

void init(int* argc, char** argv) {

	//Initialize window
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(200, 100);
	window = glutCreateWindow("2DGraphing");

	//Initialize glew
	GLenum glew_init = glewInit();
	if(glew_init != GLEW_OK) {
		std::cout << "Error: " << glewGetErrorString(glew_init) << std::endl;
		exit(-1);
	}

	//callbacks registration
	glutKeyboardFunc(keyboardFunction);
	glutDisplayFunc(displayFunc);
	glutMotionFunc(motionFunc);
	glutMouseFunc(mouseFunc);

	//

	//stbi_set_flip_vertically_on_load(true);
	glEnable(GL_CULL_FACE);
  	glCullFace(GL_BACK);
  	glFrontFace(GL_CCW);
  	glEnable(GL_DEPTH_TEST);
  
  	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void displayFunc() {


	scene->draw();

	glutSwapBuffers();
}

void keyboardFunction(unsigned char key, int x, int y) {

	if(key == 'd') {
		scene->translateSelected(glm::vec3(0.1f, 0.0f, 0.0f));
	}

	if(key == 's') {
		scene->translateSelected(glm::vec3(0.0f, -0.1f, 0.0f));
	}

	if(key == 'w') {
		scene->translateSelected(glm::vec3(0.0f, 0.1f, 0.0f));
	}

	if(key == 'a') {
		scene->translateSelected(glm::vec3(-0.1f, 0.0f, 0.0f));
	}

	if(key == 27) {
		exit(0);
	}
	glutPostRedisplay();
}

void motionFunc(int x, int y) {
	scene->camera().mouseInput(x, y);
	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y) {
	if(button == GLUT_LEFT && state == GLUT_DOWN) {
		scene->mousePicking(x, y);
		glutPostRedisplay();
	}
}