#include "Screen.h"

#include "Keyboard.h"
#include "Mouse.h"

//unsigned int Screen::SCR_WIDTH = 800;
//unsigned int Screen::SCR_HEIGHT = 600;
unsigned int Screen::SCR_WIDTH = 1280;
unsigned int Screen::SCR_HEIGHT = 720;

void Screen::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

Screen::Screen()
	: window(nullptr) {}

bool Screen::init() {
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MePlus", NULL, NULL);
	if (!window) {
		// window not created
		return false;
	}

	glfwMakeContextCurrent(window);
	return true;
}

void Screen::setParameters() {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glfwSetFramebufferSizeCallback(window, Screen::framebufferSizeCallback);

	glfwSetKeyCallback(window, Keyboard::keyCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	glEnable(GL_DEPTH_TEST);

}

void Screen::update() {
	// restore colors gradually when changed
	if (b > 0.2f) {
		b -= 0.01f;
	}
	if (g > 0.2f) {
		g -= 0.01f;
	}

	//glClearColor(0.2f,0.3f,0.3f,1.0f); //gray
	glClearColor(r, b, g, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Screen::slideBackgroundColors() {
	b = 0.5f;
	g = 0.5f;
}

void Screen::newFrame() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Screen::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Screen::setShouldClose(bool shouldClose) {
	glfwSetWindowShouldClose(window, shouldClose);
}
