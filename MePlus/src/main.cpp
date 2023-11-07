#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <string>
#include <vector>
#include <stack>
#include <stdlib.h>

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
#include "graphics/models/stage.hpp"
#include "graphics/models/data.hpp"

#include "physics/environment.h"

#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Camera.h"
#include "io/Screen.h"

#include "algorithms/MyMalloc.h"

using namespace std;
using namespace std::chrono;
using namespace glm;

void processInput(double dt);
void sendLamp(float dt);

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
Camera Camera::defaultCamera(vec3(-10.0f, 6.0f, 0.0f));

float dt = 0.0f; // time between frames
float lastFrame = 0.0f; // time of last frame
int altInt = 1; // used to flip + and - with RNG

bool flashLightOn = false;

//unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
unsigned int SCR_WIDTH = 1280, SCR_HEIGHT = 720;
float theta = 45.0f;

//Ico ico(vec3(0.0f), vec3(0.25f));
vector<Ico> launchObjects;
vector<Lamp> sentLamps;
//vector<Data> dataCubes;
unsigned int numPointLights = 1;

Screen screen;

MyMalloc myMalloc;

//Data testData;

// list of dataCubes that need to be moved around
//list<Data> dataCubes;

bool allocThreadActive = true;

void allocThread();
GLenum glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__);

int main() {
    // SETUP ======================================================
	cout << "Hello, world!" << endl;

    // seed RNG
    srand(time(NULL));

    int success;
    char infoLog[512];

    // initiate openGL version 3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // #ifdef __APPLE__
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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
    Shader lampShader("assets/object.vs", "assets/lamp.fs");

    // MODELS=====================================================================

    // fill dataCube list
    //myMalloc.dataCubes.push_back(testData);

    // initialize each cube in the dataCubes list
    /*for (Data &dataCube : myMalloc.dataCubes) {
        dataCube.init();
        dataCube.size = vec3(0.7f);
        dataCube.rb.pos = myMalloc.spawnPos;
    }*/

    // data cube
    //Data testData;
    /*testData.init();
    testData.size = vec3(0.7f);
    testData.rb.pos = vec3(0.0f, 3.0f, 0.0f);*/

    // new stage block
    Stage myStage;
    myStage.init();
    myStage.size = vec3(5.0f);
    myStage.rb.pos = vec3(0.0f, -5.0f, 0.0f);

    Ico myIco;
    myIco.init();

    // rotating sun
    DirLight dirLight = { glm::vec3(-0.2f, -1.0, -0.3), 
        glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), 
        glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), 
        glm::vec4(0.75f,0.75f,0.75f,1.0f) 
    };

    // flashlight
    SpotLight s = {
        camera.defaultCamera.cameraPos, camera.defaultCamera.cameraFront,
        glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.f)),
        1.0f, 0.07f, 0.032f,
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f)
    };

    // MY STUFF =========================================================================
    // set pointLight (myLamp) at center of world
    //Lamp testLamp;
    //testLamp = Lamp(vec3(1.0f, 1.0f, 1.0f), // light color
    //    vec4(0.05f, 0.05f, 0.05f, 1.0f), // ambi
    //    vec4(0.8f, 0.8f, 0.8f, 1.0f),  // diff
    //    vec4(1.0f, 1.0f, 1.0f, 1.0f), // spec
    //    1.0f, 0.07f, 0.032f, // light strength?
    //    vec3(4.0f, 4.0f, 4.0f), // position
    //    vec3(0.1f) // size
    //);
    //testLamp.init();

    camera.defaultCamera.updateCameraDirection(0.0f, -30.0f); // looking down at the stage

    // ERROR CHECK
    /*cout << "before mainloop | error code: " << glGetError() << endl;
    glCheckError();*/

    // start lamp sending thread
    //auto three_seconds = 3s;
    //thread (sendLamp, dt).detach();
    float lampTimer = 0.0f;
    float lampTimerIncrement = 2.0f;

    // MAIN LOOP ===============================================================================
    while (!screen.shouldClose())
    {
        // calculate dt (deltaTime)
        double currentTime = glfwGetTime();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;

        // code to send lamps every few seconds
        // if currentTime exceeds lampTimer, fire sendLamp() and extend timer
        if (currentTime > lampTimer) {
            sendLamp(dt);
            lampTimer += lampTimerIncrement;
        }

        processInput(dt);
        // check for thread event
        // if allocThread is not active, the cubes have reached their target positions
        if (!allocThreadActive) {

            // insert a new function here that uses switch statement to automate cubes

            // switch statement to fire the appropriate function
            switch (myMalloc.rotate) {
                case myMalloc.rotate :
                    myMalloc.rotateCubes();
                default:
                    break;
            }

            //myMalloc.autoAlloc();
            allocThreadActive = true;
            thread myThread(allocThread);
            myThread.detach();
        }

        // RENDER =========================
        screen.update();

        shader.activate();
        shader.set3Float("viewPos", camera.defaultCamera.cameraPos);

        // rotating the dirLight test
        dirLight.direction = glm::vec3(
            glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::vec4(dirLight.direction, 1.0f));
        dirLight.render(shader);

        // POSITION CHECK ==================================================================================================
        
        myMalloc.positionCheck();

        // RENDER LIGHTS ===========================================
        //testLamp.pointLight.render(shader, 0);

        for (int i = 0; i < sentLamps.size(); i++) {
            sentLamps[i].pointLight.render(shader, i);
        }

        // update number of point lights for shader
        shader.setInt("noPointLights", sentLamps.size());

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
        for (Ico &ico : launchObjects) {
            if (length(Camera::defaultCamera.cameraPos - ico.rb.pos) > 30.0f) {
                launchObjects.erase(launchObjects.begin());
            }
        }

        // REMOVE LAMPS
        for (Lamp &lamp : sentLamps) {
            // erase when lamp is too far from camera
            if (length(Camera::defaultCamera.cameraPos - lamp.rb.pos) > 50.0f) {
                sentLamps.erase(sentLamps.begin());
                numPointLights--;
            }
        }

        // RENDER ICO
        for (Ico &ico : launchObjects) {
            ico.render(shader, dt);
        }

        //myIco.render(shader, dt);
        //myBlock.render(shader, dt);
        
        myStage.render(shader, dt);
        
        // render all cubes held by myMalloc
        for (vector<Data> &dataBlock : myMalloc.dataBlocks) {
            for (Data &dataCube : dataBlock) {
                dataCube.render(shader, dt);
            }
        }

        lampShader.activate();
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);

        // RENDER SENTLAMPS
        for (Lamp &lamp : sentLamps) {
            lamp.render(lampShader, dt);
        }

        // send new frame to window
        screen.newFrame();
    }

    // Cleanup
    for (Ico ico : launchObjects) {
        ico.cleanup();
    }

    myStage.cleanup();
    //testData.cleanup();

    for (vector<Data> dataBlock : myMalloc.dataBlocks) {
        for (Data dataCube : dataBlock) {
            dataCube.cleanup();
        }
    }

    for (Lamp lamp : sentLamps) {
        lamp.cleanup();
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

void launchItem(float dt) {
    Ico newIco(Camera::defaultCamera.cameraPos, vec3(0.25f));
    newIco.init();
    newIco.rb.applyAcceleration(Environment::gravitationalAcceleration);
    newIco.rb.applyImpulse(Camera::defaultCamera.cameraFront, 1000.0f, dt);
    launchObjects.push_back(newIco);
}

void sendLamp(float dt) {
    
    // get a random float to spawn a lamp on the Z plane with
    float randZ = (float(rand()) / float((RAND_MAX)) * 6.0f);

    randZ *= altInt;
    altInt = altInt * -1; // flip altInt
    //cout << randZ << " : " << altInt << " : " << randZ << endl;

    Lamp newLamp;
    newLamp = Lamp(vec3(1.0f, 1.0f, 1.0f), // light color
        vec4(0.05f, 0.05f, 0.05f, 1.0f), // ambi
        vec4(0.8f, 0.8f, 0.8f, 1.0f),  // diff
        vec4(1.0f, 1.0f, 1.0f, 1.0f), // spec
        1.0f, 0.07f, 0.032f, // light strength?
        vec3(30.0f, 3.0f, randZ), // position
        vec3(0.1f) // size
    );
    newLamp.init();
    newLamp.rb.applyImpulse(vec3(-1.0f, 0.0f, 0.0f), 500.0f, dt);
    sentLamps.push_back(newLamp);
    numPointLights++; // update number of point lights for shader
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

    // shoot ball
    if (Keyboard::keyWentDown(GLFW_KEY_F)) {
        launchItem(dt);
    }

    // send lamp across stage
    if (Keyboard::keyWentDown(GLFW_KEY_Z)) {
        sendLamp(dt);
    }

    // test MyMalloc
    if (Keyboard::keyWentDown(GLFW_KEY_X)) {
        myMalloc.alloc();
    }

    // test spawn
    if (Keyboard::keyWentDown(GLFW_KEY_C)) {
        // init vector of cubes
        myMalloc.spawnCubes(3);
    }

    // test move
    if (Keyboard::keyWentDown(GLFW_KEY_V)) {
        
        myMalloc.moveCubes(0);
    }

    // test place
    if (Keyboard::keyWentDown(GLFW_KEY_B)) {

        myMalloc.placeCubes();
    }

    // test main script
    if (Keyboard::keyWentDown(GLFW_KEY_G)) {
        
        myMalloc.spawnAndSet();

        // start thread alont side allocation script
        allocThreadActive = true;
        thread myThread(allocThread);
        myThread.detach();
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

// function to use as a detached thread to report when cubes having finished movement
void allocThread() {

    while (myMalloc.cubesAreMoving) {
        //cout << "cubes moving" << endl;
    }
    // after cubes stop moving, do something else
    cout << "cubes stopped" << endl;
    
    //myMalloc.autoAlloc();
    // change a variable to trigger next step in automation
    allocThreadActive = false;
    //cout << allocThreadActive << endl;

}
