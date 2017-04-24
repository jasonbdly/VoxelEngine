#include "Camera.h"

Camera::Camera(GLfloat speed, GLfloat fastSpeed, GLfloat sensitivity) {
	pos = vec3(0.0f, 0.0f, 0.0f);
	front = vec3(0.0f, 0.0f, -1.0f);
	up = vec3(0.0f, 1.0f, 0.0f);
	yaw = -90.0f;
	pitch = 0.0f;

	this->fastMode = false;
	this->speed = speed;
	this->fastSpeed = fastSpeed;
	this->sensitivity = sensitivity;
}

Camera::~Camera() {
}

mat4 Camera::getLookAt() {
	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));
	return lookAt(pos, pos + normalize(front), up);
}

vec3 Camera::getPos() {
	return pos;
}

void Camera::moveX(GLfloat delta) {
	if (fastMode) {
		pos += normalize(cross(front, up)) * delta * fastSpeed;
	} else {
		pos += normalize(cross(front, up)) * delta * speed;
	}
}

void Camera::moveY(GLfloat delta) {
	if (fastMode) {
		pos += up * delta * fastSpeed;
	} else {
		pos += up * delta * speed;
	}
}

void Camera::moveZ(GLfloat delta) {
	if (fastMode) {
		pos += front * delta * fastSpeed;
	} else {
		pos += front * delta * speed;
	}
}


void Camera::setPos(vec3 pos) {
	this->pos = pos;
}

void Camera::enableFastMode() {
	this->fastMode = true;
}

void Camera::disableFastMode() {
	this->fastMode = false;
}

void Camera::lookX(GLfloat delta) {
	yaw += delta * sensitivity;
}

void Camera::lookY(GLfloat delta) {
	pitch += delta * sensitivity;
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}
}

void Camera::lookZ(GLfloat delta) {
	
}