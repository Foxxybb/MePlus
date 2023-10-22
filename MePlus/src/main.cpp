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
#include "graphics/Light.h"

#include "graphics/models/cube.hpp"
#include "graphics/models/lamp.hpp"

#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Camera.h"
#include "io/Screen.h"

using namespace std;

void processInput(double dt);

float mixVal = 0.5f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool flashLightOn = true;

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

    // ERROR CHECK
    cout << "before shaders | error code: " << glGetError() << endl;
    glCheckError();

    // SHADERS====================================================================
    Shader shader("assets/object.vs", "assets/object.fs");
    Shader lampShader("assets/object.vs", "assets/lamp.fs");

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    Cube cubes[10];
    for (unsigned int i = 0; i < 10; i++) {
        cubes[i] = Cube(Material::gold, cubePositions[i], glm::vec3(1.0f));
        cubes[i].init();
    }

    glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f,  0.2f,  2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3(0.0f,  0.0f, -3.0f)
    };
    Lamp lamps[4];
    for (unsigned int i = 0; i < 4; i++) {
        lamps[i] = Lamp(glm::vec3(1.0f),
            glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f),
            1.0f, 0.07f, 0.032f,
            pointLightPositions[i], glm::vec3(0.25f));
        lamps[i].init();
    }

    DirLight dirLight = { glm::vec3(-0.2f, -1.0, -0.3), glm::vec3(0.1f), glm::vec3(0.4f), glm::vec3(0.75f) };
    
    Lamp lamp(glm::vec3(1.0f), 
        glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 
        1.0f, 0.07f, 0.032f,
        glm::vec3(-1.0f, -0.5f, -0.5f), glm::vec3(0.25));
    lamp.init();

    SpotLight s = {
        camera.cameraPos, camera.cameraFront,
        glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.f)),
        1.0f, 0.07f, 0.032f,
        glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f)
    };

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
        shader.set3Float("viewPos", camera.cameraPos);

        // rotating the dirLight test
        dirLight.direction = glm::vec3(
            glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::vec4(dirLight.direction, 1.0f));
        dirLight.render(shader);

        for (int i = 0; i < 4; i++) {
            lamps[i].pointLight.render(shader, i);
        }
        shader.setInt("noPointLights", 4);

        if (flashLightOn) {
            s.position = camera.cameraPos;
            s.direction = camera.cameraFront;
            s.render(shader, 0);
            shader.setInt("noSpotLights", 1);
        }
        else {
            shader.setInt("noSpotLights", 0);
        }
        

        // create transformation for screen
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = camera.getViewMatrix();
        projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        for (int i = 0; i < 10; i++) {
            cubes[i].render(shader);
        }

        lampShader.activate();
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        lamp.render(lampShader);
        for (int i = 0; i < 4; i++) {
            lamps[i].render(lampShader);
        }

        // send new frame to window
        screen.newFrame();
    }

    for (int i = 0; i < 10; i++) {
        cubes[i].cleanup();
    }
    for (int i = 0; i < 4; i++) {
        lamps[i].cleanup();
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


void processInput(double dt)
{   
    // if ESC key is pressed, close window
    if (Keyboard::key(GLFW_KEY_ESCAPE)) {
        screen.setShouldClose(true);
    }

    // toggle flashLight (first person spotLight)
    if (Keyboard::keyWentDown(GLFW_KEY_TAB)) {
        flashLightOn = !flashLightOn;
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

