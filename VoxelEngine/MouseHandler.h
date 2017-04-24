#pragma once

#include "includes.h"

class MouseHandler
{
public:
	static MouseHandler *instance();

	GLfloat getXOffset();
	GLfloat getYOffset();

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		if (!instance()->mouseInit) {
			instance()->lastX = xpos;
			instance()->lastY = ypos;
			instance()->mouseInit = true;
		}

		instance()->xOffset += xpos - instance()->lastX;
		instance()->yOffset += instance()->lastY - ypos;

		if (instance()->xOffset != 0 || instance()->yOffset != 0) {
			//printf("XOffset: %f, X: %f, lastX: %f\n", instance()->xOffset, xpos, instance()->lastX);
			//printf("YOffset: %f, Y: %f, lastT: %f\n", instance()->yOffset, ypos, instance()->lastY);
		}

		instance()->lastX = xpos;
		instance()->lastY = ypos;
	}
private:
	static MouseHandler *inst;

	bool mouseInit = false;
	GLfloat lastX, lastY, xOffset, yOffset;


	MouseHandler();
	~MouseHandler();
};

