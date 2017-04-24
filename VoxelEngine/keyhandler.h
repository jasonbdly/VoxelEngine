#pragma once

#include "includes.h"

using namespace std;

class KeyHandler
{
public:
	static KeyHandler *instance();

	bool isKeyDown(int key);

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action != GLFW_REPEAT) {
			instance()->keyStates[key] = action;
			instance()->keyRepeatState[key] = false;
		}
		else {
			instance()->keyRepeatState[key] = true;
		}
	}

private:
	static KeyHandler *inst;

	KeyHandler();
	~KeyHandler();

	vector<int> keyStates;
	vector<bool> keyRepeatState;
};

