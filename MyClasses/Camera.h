
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#ifndef CAMERA_H
#define CAMERA_H

/*code here*/

enum Camera_Movement{
	FORWARD,
	BACK,
	LEFT,
	RIGHT
};

enum Camera_Mode {
	PERSPECTIVE,
	ORTHOGRAPHIC
};

class Camera {

public:

	Camera();
	Camera(glm::vec3 cameraPosition, glm::vec3 viewReferencePoint, glm::vec3 upDirection);
	Camera(glm::vec3 cameraPosition, glm::vec3 viewReferencePoint, glm::vec3 upDirection,
			float movementSpeed, float rotationSpeed);

private:

	///Vettori da cui si ricava il sistema di riferimento della camera
	glm::vec3 _position;
	glm::vec3 _vrp;
	glm::vec3 _upDirection;

	///Questi sono i tre vettori(versori) del sistema di riferimento della camera
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;

	glm::mat4 _projection;

	float _movement_speed; ///< moltiplicatore per traslazioni della camera
	float _rotation_speed; ///< moltiplicatore per rotazioni della camera

	float _pitch; ///< Angolo di eulero per le rotazioni attorno a x (guardare su o giu)
	float _yaw; ///< Angolo di eulero per le rotazioni attorno a y (guardare dx/sx)

	bool _firstMouse = true;

	int _lastx; ///< ultima posizione del mouse nella finestra
	int _lasty;

public:

	///Restituisce la matrice della trasformazione di camera
	glm::mat4 viewTransform();

	void setPerspective(float FOVdeg, float width, float height, float near, float far);

	void resetPosition();

	///Funzioni che spostano la camera lungo l'asse specificato della quantità x, y o z
	void moveX(float x);
	void moveY(float y);
	void moveZ(float z);

	void rotateX(float x);
	void rotateY(float y);

	const glm::vec3& position() const; 
	const glm::vec3& vrp() const;
	const glm::vec3& upDirection() const;

	const glm::mat4& projection() const;

	float& movementSpeed();
	float& rotationSpeed();


	///Funzione che gestisce l'input da tastiera.
	///Eventualmente da registrare come callback GLUT
	void keyboardInput(Camera_Movement direction, float speed);

	///Funzione che gestisce l'input dal mouse
	///Callback GLUT
	void mouseInput(int x, int y);


private:

	///Funzione che ricalcola i versori del sistema di riferimento camera dopo
	///una modifica degli angoli di Eulero.

	///NB: quando la camera viene traslata non 
	void updateAxes();

};

#endif