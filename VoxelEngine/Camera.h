#pragma once

#include "includes.h"

using namespace glm;

class Camera {
public:
	Camera(GLfloat speed, GLfloat fastSpeed, GLfloat sensitivity);
	~Camera();

	mat4 getLookAt();
	vec3 getPos();

	void moveX(GLfloat delta);
	void moveY(GLfloat delta);
	void moveZ(GLfloat delta);
	void setPos(vec3 pos);

	void enableFastMode();
	void disableFastMode();

	void lookX(GLfloat delta);
	void lookY(GLfloat delta);
	void lookZ(GLfloat delta);
private:
	vec3 pos, front, up;
	GLfloat yaw, pitch;

	GLfloat speed, fastSpeed, sensitivity;
	bool fastMode;
};

