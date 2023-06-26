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

void initModels();

//glut callbacks
void keyboardFunction(unsigned char key, int x, int y);
void displayFunc();
void motionFunc(int x, int y);
void mouseFunc(int button, int state, int x, int y);

int main(int argc, char** argv) {
	init(&argc, argv);

	initModels();

	mainCamera.rotateX(-35);

	scene->setCamera(mainCamera);
	
	mainProgram->use();

	/* Informazioni sulla luce */
	mainProgram->setLightColor(glm::vec3(1, 1, 1));
	mainProgram->setLightIntensity(1);
	mainProgram->setLightPosition(glm::vec3(3, 3, 1));

	mainProgram->unbind();

	pieceProgram->use();
	pieceProgram->setLightColor(glm::vec3(1, 1, 1));
	pieceProgram->setLightIntensity(1);
	pieceProgram->setLightPosition(glm::vec3(1,1,1));
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
		scene->camera().rotateY(-1.0);
	}

	if(key == 's') {
		scene->camera().keyboardInput(Camera_Movement::BACK, 0.05);
	}

	if(key == 'w') {
		scene->camera().keyboardInput(Camera_Movement::FORWARD, 0.05);
	}

	if(key == 'a') {
		scene->camera().rotateY(1.0);
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

void initModels() {
	
	mainProgram = new Program();
	pieceProgram = new Program();

	Shader boardVertexShader(GL_VERTEX_SHADER);
	Shader boardFragmentShader(GL_FRAGMENT_SHADER);
	Shader pieceVertexShader(GL_VERTEX_SHADER);
	Shader pieceFragmentShader(GL_FRAGMENT_SHADER);

	boardVertexShader.compile("shaders/square.vert");
	boardFragmentShader.compile("shaders/square.frag");
	pieceVertexShader.compile("shaders/vertex_shader.vert");
	pieceFragmentShader.compile("shaders/fragment_shader.frag");


	mainProgram->addShader(boardVertexShader);
	mainProgram->addShader(boardFragmentShader);
	pieceProgram->addShader(pieceVertexShader);
	pieceProgram->addShader(pieceFragmentShader);

	mainProgram->link();
	pieceProgram->link();

	std::vector<Program> programs;
	programs.push_back(*mainProgram);
	programs.push_back(*pieceProgram);

	/* Inizializzazione delle mesh */

	Mesh light, dark, board;
	light.loadMesh("models/modelli_corretti/light_square.obj", aiProcess_FlipUVs);
	dark.loadMesh("models/modelli_corretti/dark_square.obj", aiProcess_FlipUVs);
	board.loadMesh("models/modelli_corretti/board.obj", aiProcess_FlipUVs);

	dark.setProgramIndex(0);
	light.setProgramIndex(0);
	board.setProgramIndex(0);






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


	//setup pawns

	Mesh whitePawn, blackPawn;

	whitePawn.loadMesh("models/modelli_corretti/white_pawn.obj", aiProcess_Triangulate);
	whitePawn.setProgramIndex(1);

	blackPawn.loadMesh("models/modelli_corretti/black_pawn.obj", aiProcess_Triangulate);
	blackPawn.setProgramIndex(1);

	for(int i = 0; i < 8; ++i) {
		whitePawn.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(i * -0.058, 0, -0.058)));
		meshes.push_back(whitePawn);

		blackPawn.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(i * -0.058, 0, -6 * 0.058)));
		meshes.push_back(blackPawn);
	}

	//setup rooks

	Mesh whiteRook, blackRook;

	whiteRook.loadMesh("models/modelli_corretti/white_rook.obj", 0);
	whiteRook.setProgramIndex(1);
	blackRook.loadMesh("models/modelli_corretti/black_rook.obj", 0);
	blackRook.setProgramIndex(1);

	whiteRook.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0)));
	meshes.push_back(whiteRook);

	whiteRook.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(7 * -0.058, 0, 0)));
	meshes.push_back(whiteRook);

	blackRook.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -7 * 0.058)));
	meshes.push_back(blackRook);

	blackRook.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(7 * -0.058, 0, -7 * 0.058)));
	meshes.push_back(blackRook);

	//setup bishop

	Mesh whiteBishop, blackBishop;

	whiteBishop.loadMesh("models/modelli_corretti/white_bishop.obj", 0);
	whiteBishop.setProgramIndex(1);
	blackBishop.loadMesh("models/modelli_corretti/black_bishop.obj", 0);
	blackBishop.setProgramIndex(1);

	whiteBishop.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(-2 * 0.058, 0, 0)));
	meshes.push_back(whiteBishop);

	whiteBishop.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(5 * -0.058, 0, 0)));
	meshes.push_back(whiteBishop);

	blackBishop.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(-2 * 0.058, 0, -7 * 0.058)));
	meshes.push_back(blackBishop);

	blackBishop.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(5 * -0.058, 0, -7 * 0.058)));
	meshes.push_back(blackBishop);


	
	//steup queen
	Mesh whiteQueen, blackQueen;

	whiteQueen.loadMesh("models/modelli_corretti/white_queen.obj", 0);
	whiteQueen.setProgramIndex(1);
	blackQueen.loadMesh("models/modelli_corretti/black_queen.obj", 0);
	blackQueen.setProgramIndex(1);

	whiteQueen.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(-4* 0.058 ,0, 0)));
	meshes.push_back(whiteQueen);

	blackQueen.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(-4* 0.058 ,0, -7 * 0.058)));
	meshes.push_back(blackQueen);

	//setup knight

	Mesh whiteKnight, blackKnight;

	whiteKnight.loadMesh("models/modelli_corretti/white_knight.obj", 0);
	whiteKnight.setProgramIndex(1);
	blackKnight.loadMesh("models/modelli_corretti/black_knight.obj", 0);
	blackKnight.setProgramIndex(1);

	whiteKnight.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(-0.058, 0, 0)));
	meshes.push_back(whiteKnight);

	whiteKnight.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(-6 * 0.058, 0, 0)));
	meshes.push_back(whiteKnight);

	blackKnight.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(-0.058, 0, -7 * 0.058)));
	meshes.push_back(blackKnight);

	blackKnight.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(-6 * 0.058, 0, -7 * 0.058)));
	meshes.push_back(blackKnight);

	//setup king

	Mesh whiteKing, blackKing;

	whiteKing.loadMesh("models/modelli_corretti/white_king.obj", 0);
	whiteKing.setProgramIndex(1);
	blackKing.loadMesh("models/modelli_corretti/black_king.obj", 0);
	blackKing.setProgramIndex(1);

	whiteKing.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(3 * -0.058, 0, 0)));
	meshes.push_back(whiteKing);

	blackKing.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(3 * -0.058, 0, -7 * 0.058)));
	meshes.push_back(blackKing);

	//setup clock

	Mesh body, screen, b1, b2, b3, b4;

	body.loadMesh("models/modelli_corretti/clock_body.obj", aiProcess_Triangulate);
	body.setProgramIndex(1);
	//body.setModelTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.1)));
	meshes.push_back(body);

	screen.loadMesh("models/modelli_corretti/clock_screen.obj", aiProcess_Triangulate);
	screen.setProgramIndex(1);
	meshes.push_back(screen);

	b1.loadMesh("models/modelli_corretti/clock_up_button.obj", 0);
	b1.setProgramIndex(1);
	meshes.push_back(b1);

	b2.loadMesh("models/modelli_corretti/clock_button_left.obj", 0);
	b2.setProgramIndex(1);
	meshes.push_back(b2);

	b3.loadMesh("models/modelli_corretti/clock_button_right.obj", 0);
	b3.setProgramIndex(1);
	meshes.push_back(b3);

	b4.loadMesh("models/modelli_corretti/clock_button_center.obj", 0);
	b4.setProgramIndex(1);
	meshes.push_back(b4);

	//setup table

	Mesh legs1, legs2, table;

	table.loadMesh("models/modelli_corretti/table_body.obj", 0);
	table.setProgramIndex(0);
	meshes.push_back(table);

	legs1.loadMesh("models/modelli_corretti/legs_1.obj" , 0);
	legs1.setProgramIndex(1);
	meshes.push_back(legs1);

	legs2.loadMesh("models/modelli_corretti/legs_2.obj" , 0);
	legs2.setProgramIndex(1);
	meshes.push_back(legs2);

	scene = new Scene(meshes, programs);
	
	}