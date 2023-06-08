#include "Camera.h"

Camera::Camera() {
	_position = glm::vec3(0, 0, 5);
	_vrp = glm::vec3(0, 0, 0);
	_upDirection = glm::vec3(0, 1, 0);

	//Ricavo la direzione di puntamento dalla posizione e view reference point
	_front = (_vrp - _position);

	_yaw = -90.0;
	_pitch = 0.0;

	//imposta velocità di movimento e rotazione di default
	_movement_speed = 0.1;
	_rotation_speed = 2;

	//calcolo e normalizzo gli assi del sistema di riferimento della camera
	updateAxes();
}

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 viewReferencePoint, glm::vec3 upDirection) : 
	_position(cameraPosition), _vrp(viewReferencePoint), _upDirection(upDirection) {
		updateAxes();
	}

void Camera::updateAxes() {

	_front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front.y = sin(glm::radians(_pitch));
	_front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(_front);

	_right = glm::normalize(glm::cross(_front, _upDirection));
	_up = glm::normalize(glm::cross(_right, _front));
}

glm::mat4 Camera::viewTransform() {
	return glm::lookAt(_position, _position + _front, _up);
}

void Camera::resetPosition() {
	_yaw = -90;
	_pitch = 0;
	_position = glm::vec3(0, 0, 2);
	updateAxes();
}

void Camera::moveX(float x) {
	_position.x += x;
}

void Camera::moveY(float y) {
	_position.y += y;
}

void Camera::moveZ(float z) {
	_position.z += z;
}

void Camera::keyboardInput(Camera_Movement direction, float speed) {
	if(direction == FORWARD){
		_position -= _front * speed;
	}	
	if(direction == LEFT){
		_position += _right * speed;
	}	
	if(direction == BACK){
		_position += _front * speed;
	}	
	if(direction == RIGHT){
		_position -= _right * speed;
	}
}

void Camera::mouseInput(int x, int y) {


	if(_firstMouse) {
		_firstMouse = false;
		_lastx = x;
		_lasty = y;
	}

	float xoffset = ((float) x - (float) _lastx) * _rotation_speed;
	float yoffset = ((float) _lasty - (float) y) * _rotation_speed;

	_lastx = x;
	_lasty = y;

	if(xoffset > 2) {
		xoffset = 2;
	}

	if(yoffset > 2) {
		yoffset = 2;
	}

	_yaw += xoffset;
	_pitch += yoffset;

	if(_pitch > 89.0)
		_pitch = 89.0;
	if(_pitch < -89.0)
		_pitch = -89.0;

	updateAxes();
}

const glm::vec3& Camera::position() const {
	return _position;
}

const glm::vec3& Camera::vrp() const {
	return _vrp;
}

const glm::vec3& Camera::upDirection() const {
	return _upDirection;
}

float& Camera::movementSpeed() {
	return _movement_speed;
}

float& Camera::rotationSpeed() {
	return _rotation_speed;
}