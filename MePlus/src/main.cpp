#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include "graphics/Shader.h"
#include "graphics/Texture.h"

#include "graphics/models/cube.hpp"

#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Camera.h"
#include "io/Screen.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(double dt);

float mixVal = 0.5f;


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
float theta = 45.0f;

Screen screen;

GLenum glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__);

int main() {
	cout << "Hello, world!" << endl;

    int success;
    char infoLog[512];

    // initiate openGL version 3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // #ifdef __APPLE__
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
    // #endif

    if (!screen.init()) {
        std::cout << "Could not create window." << std::endl;
        glfwTerminate();
        return -1;
    }

    // initialize GLAD before we call any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set the dimensions of the viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    screen.setParameters();

    // Z BUFFER
    glEnable(GL_DEPTH_TEST);

    // SHADERS====================================================================
    Shader shader("assets/object.vs", "assets/object.fs");

    Cube cube(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
    cube.init();
    

    // ERROR CHECK
    cout << "before mainloop | error code: " << glGetError() << endl;
    glCheckError();

    // MAIN LOOP===============================================================================
    while (!screen.shouldClose())
    {
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(deltaTime);

        // render
        screen.update();

        shader.activate();
        shader.setFloat("mixVal", mixVal);


        // create transformation for screen
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = camera.getViewMatrix();
        projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        cube.render(shader);

        // send new frame to window
        screen.newFrame();
    }

    glfwTerminate();
	return 0;
}

GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}


// function to reorient the viewport if the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;

}

void processInput(double dt)
{   
    // if ESC key is pressed, close window
    if (Keyboard::key(GLFW_KEY_ESCAPE)) {
        screen.setShouldClose(true);
    }

    // change mixValue
    if (Keyboard::key(GLFW_KEY_UP)) {
        mixVal += 0.05f;
        if (mixVal > 1) { mixVal = 1; }
    }
    if (Keyboard::key(GLFW_KEY_DOWN)) {
        mixVal -= 0.05f;
        if (mixVal < 0) { mixVal = 0; }
    }

    // camera movement
    if (Keyboard::key(GLFW_KEY_W)) {
        camera.updateCameraPos(CameraDirection::FORWARD, dt);
    }
    if (Keyboard::key(GLFW_KEY_S)) {
        camera.updateCameraPos(CameraDirection::BACKWARD, dt);
    }
    if (Keyboard::key(GLFW_KEY_A)) {
        camera.updateCameraPos(CameraDirection::LEFT, dt);
    }
    if (Keyboard::key(GLFW_KEY_D)) {
        camera.updateCameraPos(CameraDirection::RIGHT, dt);
    }
    if (Keyboard::key(GLFW_KEY_SPACE)) {
        camera.updateCameraPos(CameraDirection::UP, dt);
    }
    if (Keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
        camera.updateCameraPos(CameraDirection::DOWN, dt);
    }

    // camera look
    double dx = Mouse::getDX(), dy = Mouse::getDY();
    if (dx != 0 || dy != 0) {
        camera.updateCameraDirection(dx,dy);
    }

    // camera zoom
    double scrollDy = Mouse::getScrollDY();
    if (scrollDy != 0) {
        camera.updateCameraZoom(scrollDy);
    }


}

