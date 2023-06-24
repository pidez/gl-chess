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

Program* mainProgram;
Program* pieceProgram;


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

	mainProgram = new Program();
	pieceProgram = new Program();

	Shader sphereVertexShader(GL_VERTEX_SHADER);
	Shader sphereFragmentShader(GL_FRAGMENT_SHADER);
	Shader pieceVertexShader(GL_VERTEX_SHADER);
	Shader pieceFragmentShader(GL_FRAGMENT_SHADER);

	sphereVertexShader.compile("shaders/square.vert");
	sphereFragmentShader.compile("shaders/square.frag");
	pieceVertexShader.compile("shaders/vertex_shader.vert");
	pieceFragmentShader.compile("shaders/fragment_shader.frag");


	mainProgram->addShader(sphereVertexShader);
	mainProgram->addShader(sphereFragmentShader);
	pieceProgram->addShader(pieceVertexShader);
	pieceProgram->addShader(pieceFragmentShader);

	mainProgram->link();
	pieceProgram->link();

	std::vector<Program> programs;
	programs.push_back(*mainProgram);
	programs.push_back(*pieceProgram);

	/* Inizializzazione delle mesh */

	Mesh light, dark, board;
	light.loadMesh("models/board/light_square.obj", aiProcess_FlipUVs);
	dark.loadMesh("models/board/dark_square.obj", aiProcess_FlipUVs);
	board.loadMesh("models/board/board.obj", aiProcess_FlipUVs);

	dark.setProgramIndex(0);
	light.setProgramIndex(0);
	board.setProgramIndex(0);

	board.setModelTransform(glm::rotate(glm::mat4(1.0), (float)glm::radians(90.0), glm::vec3(1, 0, 0)));



	/* Inizializzazione della scena */

	std::vector<Mesh> meshes;

	for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			if ((i + j) % 2 == 0) {
				light.setModelTransform(glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3(i * -0.058, 0, j * -0.058)),
				 (float)glm::radians(90.0), glm::vec3(1, 0, 0)));
				meshes.push_back(light);
			}
			else {
				dark.setModelTransform(glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3(i * -0.058, 0, j * -0.058)),
				 (float)glm::radians(90.0), glm::vec3(1, 0, 0)));
				meshes.push_back(dark);
			}
		}
	}


	//Dopo gli squares c'e il corpo della scacchiera
	meshes.push_back(board);

	//Dopo la scacchiera i pezzi
	Mesh whiteRook;
	whiteRook.loadMesh("models/rook/rook.obj", aiProcess_Triangulate);
	whiteRook.setProgramIndex(1);

	meshes.push_back(whiteRook);




	scene = new Scene(meshes, programs);

	mainCamera.rotateX(-45);

	scene->setCamera(mainCamera);

		/* Imposto gli shader per il rendering della torre
	PER PASSARE I VALORI AGLI SHADER E' NECESSARIO AVER CHIAMATO glUseProgram(id) */
	
	mainProgram->use();

	/* Informazioni sulla luce */
	mainProgram->setLightColor(glm::vec3(1, 1, 1));
	mainProgram->setLightIntensity(1);
	mainProgram->setLightPosition(glm::vec3(3, 3, 1));

	mainProgram->unbind();

	pieceProgram->use();
	pieceProgram->setLightColor(glm::vec3(1, 1, 1));
	pieceProgram->setLightIntensity(1);
	pieceProgram->setLightPosition(glm::vec3(3, 3, 1));
	pieceProgram->unbind();


	scene->enablePicking();

    glutMainLoop();
}

void init(int* argc, char** argv) {

	//Initialize window
	glutInit(argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 4);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200, 900);
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
		scene->rotateScene(glm::vec3(0, 0, 1), -1.57f);
	}

	if(key == 's') {
		scene->rotateScene(glm::vec3(1, 0, 0), 1.57f);
	}

	if(key == 'w') {
		scene->rotateScene(glm::vec3(1, 0, 0), -1.57f);
	}

	if(key == 'a') {
		scene->rotateScene(glm::vec3(0, 0, 1), 1.57f);
	}

	if(key == 27) {
		exit(0);
	}
	glutPostRedisplay();
}

void motionFunc(int x, int y) {
	scene->followCursor(x, y);
	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y) {
	if(button == GLUT_LEFT && state == GLUT_DOWN) {
		scene->mousePicking(x, y);
		glutPostRedisplay();
	}

	if(state == GLUT_UP) {
		scene->unselect();
		glutPostRedisplay();
	}
}