#include "keyhandler.h"

#define NUM_KEYS 348
#define KEY_DOWN -1

KeyHandler *KeyHandler::inst = 0;

KeyHandler *KeyHandler::instance() {
	if (!inst) {
		inst = new KeyHandler();
	}
	return inst;
}

KeyHandler::KeyHandler() {
	keyStates.resize(NUM_KEYS);
	keyRepeatState.resize(NUM_KEYS);
	for (int i = 0; i < NUM_KEYS; i++) {
		keyStates[i] = KEY_DOWN;
		keyRepeatState[i] = false;
	}
}

KeyHandler::~KeyHandler() {
	delete inst;
	inst = 0;
}

bool KeyHandler::isKeyDown(int key) {
	return keyStates[key] == GLFW_PRESS;
}