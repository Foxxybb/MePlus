#ifndef SCREEN_H
#define SCREEN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Screen {
public:
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;
	static Screen instance; // singleton

	// colors used for background
	float r = 0.0f;
	float g = 0.2f;
	float b = 0.2f;

	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	Screen();

	bool init();

	void setParameters();

	void slideBackgroundColors();

	// mainloop
	void update();
	void newFrame();

	// window closing accessor and modifier
	bool shouldClose();
	void setShouldClose(bool shouldClose);

private:
	GLFWwindow* window;
};

#endif
