#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "world.h"
#include "keyhandler.h"
#include "MouseHandler.h"

int main(int argc, char* argv[]) {
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Voxel Test", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyHandler::key_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, MouseHandler::mouse_callback);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	glClearColor(0.390625f, 0.58203125f, 0.92578125f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glFrontFace(GL_CW);
	//World gameWorld(0.5f);


	World gameWorld;
	gameWorld.init();

	//Add some test blocks
	for (int x = -5; x < 5; x += 2) {
		for (int y = -5; y < 5; y += 2) {
			for (int z = -5; z < 5; z += 2) {
				gameWorld.addVoxel(x, y, z);
			}
		}
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		if (KeyHandler::instance()->isKeyDown(GLFW_KEY_ESCAPE)) {
			break;
		}
		gameWorld.update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gameWorld.render();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}