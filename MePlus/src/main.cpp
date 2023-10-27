#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <string>
#include <vector>
#include <stack>

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
#include "graphics/Model.h"

#include "graphics/models/cube.hpp"
#include "graphics/models/lamp.hpp"
#include "graphics/models/gun.hpp"
#include "graphics/models/ico.hpp"
#include "graphics/models/block.hpp"

#include "physics/environment.h"

#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Camera.h"
#include "io/Screen.h"

using namespace std;
using namespace glm;

void processInput(double dt);

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
Camera Camera::defaultCamera(glm::vec3(0.0f, 0.0f, 0.0f));

float dt = 0.0f; // time between frames
float lastFrame = 0.0f; // time of last frame

bool flashLightOn = true;

unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
float theta = 45.0f;

//Ico ico(vec3(0.0f), vec3(0.25f));
vector<Ico> launchObjects;

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

    // MODELS=====================================================================
    //Gun g;
    //g.loadModel("assets/models/scene.gltf");

    // stage block
    Block myBlock;
    myBlock.init();
    myBlock.size = vec3(5.0f);
    myBlock.rb.pos = vec3(0.0f, -5.0f, 0.0f);

    Ico myIco;
    myIco.init();

    // rotating sun
    DirLight dirLight = { glm::vec3(-0.2f, -1.0, -0.3), 
        glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), 
        glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), 
        glm::vec4(0.75f,0.75f,0.75f,1.0f) 
    };

    glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f,  0.2f,  2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -1.0f),
            glm::vec3(0.0f,  0.0f, -3.0f)
    };

    /*Lamp lamps[4];
    for (unsigned int i = 0; i < 4; i++) {
        lamps[i] = Lamp(glm::vec3(1.0f),
            glm::vec4(0.05f, 0.05f, 0.05f, 1.0f),
            glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            1.0f, 0.07f, 0.032f,
            pointLightPositions[i], glm::vec3(0.25f));
        lamps[i].init();
    }*/

    // flashlight
    SpotLight s = {
        camera.defaultCamera.cameraPos, camera.defaultCamera.cameraFront,
        glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.f)),
        1.0f, 0.07f, 0.032f,
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f)
    };

    // MY STUFF =========================================================================
    // set pointLight (myLamp) at center of world (offset 1,1,1)
    Lamp myLamp;
    myLamp = Lamp(vec3(1.0f, 1.0f, 1.0f), // light color
        vec4(0.05f, 0.05f, 0.05f, 1.0f), // ambi
        vec4(0.8f, 0.8f, 0.8f, 1.0f),  // diff
        vec4(1.0f, 1.0f, 1.0f, 1.0f), // spec
        1.0f, 0.07f, 0.032f, // light strength?
        vec3(2.0f), // position
        vec3(0.5f) // size
    );
    myLamp.init();

    // set cube at center of world
    /*Cube myCube;
    myCube = Cube(vec3(0.0f), vec3(0.5f));
    myCube.material = Material::brass;
    myCube.init();*/



    // ERROR CHECK
    cout << "before mainloop | error code: " << glGetError() << endl;
    glCheckError();

    // MAIN LOOP===============================================================================
    while (!screen.shouldClose())
    {
        // calculate dt (deltaTime)
        double currentTime = glfwGetTime();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(dt);

        // RENDER =========================
        screen.update();

        shader.activate();
        shader.set3Float("viewPos", camera.defaultCamera.cameraPos);

        // rotating the dirLight test
        dirLight.direction = glm::vec3(
            glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::vec4(dirLight.direction, 1.0f));
        dirLight.render(shader);

        //myLamp.rb.velocity = vec3(1.0f);

        // render lamps
        /*for (int i = 0; i < 4; i++) {
            lamps[i].pointLight.render(shader, i);
        }
        shader.setInt("noPointLights", 4);*/

        // render my stuff
        myLamp.pointLight.render(shader, 0);
        shader.setInt("noPointLights", 1);

        // flashlight (pov spotLight)
        if (flashLightOn) {
            s.position = camera.defaultCamera.cameraPos;
            s.direction = camera.defaultCamera.cameraFront;
            s.render(shader, 0);
            shader.setInt("noSpotLights", 1);
        }
        else {
            shader.setInt("noSpotLights", 0);
        }
        
        // create transformation for screen
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = camera.defaultCamera.getViewMatrix();
        projection = glm::perspective(glm::radians(camera.defaultCamera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // REMOVE ICO SPHERES
        stack<int> removeObjects;
        for (Ico &ico : launchObjects) {
            if (length(Camera::defaultCamera.cameraPos - ico.rb.pos) > 30.0f) {
                launchObjects.erase(launchObjects.begin());
            }
        }

        for (Ico &ico : launchObjects) {
            ico.render(shader, dt);
        }

        //myIco.render(shader, dt);
        myBlock.render(shader, dt);
        //myCube.render(shader, dt);

        lampShader.activate();
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        //lamp.render(lampShader);
        /*for (int i = 0; i < 4; i++) {
            lamps[i].render(lampShader, dt);
        }*/
        myLamp.render(lampShader, dt);

        // send new frame to window
        screen.newFrame();
    }

    // Cleanup
    for (Ico ico : launchObjects) {
        ico.cleanup();
    }
    myIco.cleanup();
    myBlock.cleanup();

    /*for (int i = 0; i < 4; i++) {
        lamps[i].cleanup();
    }*/
    myLamp.cleanup();

    //myCube.cleanup();

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

void launchItem(float dt) {
    Ico newIco(Camera::defaultCamera.cameraPos, vec3(0.25f));
    newIco.init();
    newIco.rb.applyAcceleration(Environment::gravitationalAcceleration);
    newIco.rb.applyImpulse(Camera::defaultCamera.cameraFront, 1000.0f, dt);
    launchObjects.push_back(newIco);
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
        camera.defaultCamera.updateCameraPos(CameraDirection::FORWARD, dt);
    }
    if (Keyboard::key(GLFW_KEY_S)) {
        camera.defaultCamera.updateCameraPos(CameraDirection::BACKWARD, dt);
    }
    if (Keyboard::key(GLFW_KEY_A)) {
        camera.defaultCamera.updateCameraPos(CameraDirection::LEFT, dt);
    }
    if (Keyboard::key(GLFW_KEY_D)) {
        camera.defaultCamera.updateCameraPos(CameraDirection::RIGHT, dt);
    }
    if (Keyboard::key(GLFW_KEY_SPACE)) {
        camera.defaultCamera.updateCameraPos(CameraDirection::UP, dt);
    }
    if (Keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
        camera.defaultCamera.updateCameraPos(CameraDirection::DOWN, dt);
    }

    if (Keyboard::keyWentDown(GLFW_KEY_F)) {
        launchItem(dt);
    }

    // camera look
    double dx = Mouse::getDX(), dy = Mouse::getDY();
    if (dx != 0 || dy != 0) {
        camera.defaultCamera.updateCameraDirection(dx,dy);
    }

    // camera zoom
    double scrollDy = Mouse::getScrollDY();
    if (scrollDy != 0) {
        camera.defaultCamera.updateCameraZoom(scrollDy);
    }

}

