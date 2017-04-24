#include "MouseHandler.h"

MouseHandler *MouseHandler::inst = 0;

MouseHandler *MouseHandler::instance() {
	if (!inst) {
		inst = new MouseHandler();
	}
	return inst;
}

GLfloat MouseHandler::getXOffset() {
	GLfloat tempOffset = xOffset;
	xOffset = 0;
	return tempOffset;
}

GLfloat MouseHandler::getYOffset() {
	GLfloat tempOffset = yOffset;
	yOffset = 0;
	return tempOffset;
}

MouseHandler::MouseHandler() {
	lastX = -1;
	lastY = -1;
	xOffset = 0;
	yOffset = 0;
}


MouseHandler::~MouseHandler() {

}