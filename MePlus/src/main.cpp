#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include "Shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLenum glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__);

int main() {
	cout << "Hello, world!" << endl;

    int success;
    char infoLog[512];

    // GLM Test
    //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    //glm::mat4 trans = glm::mat4(1.0f); // identity matrix
    //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f)); // translation
    //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    //vec = trans * vec;
    //std::cout << vec.x << ' ' << vec.y << ' ' << vec.z << std::endl;

    // initiate openGL version 3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window object
    GLFWwindow* window = glfwCreateWindow(800, 600, "MePlus", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Could not create window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialize GLAD before we call any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set the dimensions of the viewport
    glViewport(0, 0, 800, 600);

    // Set the callback for the window if it gets resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Shaders
    Shader shader("assets/vertex_core.glsl", "assets/fragment_core.glsl");
    Shader shader2("assets/vertex_core.glsl", "assets/fragment_core2.glsl");

    float vertices[] = {
        // positions            colors    
        -0.25f, -0.5f, 0.0f,     1.0f, 1.0f, 0.5f,
         0.15f,  0.0f, 0.0f,     0.5f, 1.0f, 0.75f,
         0.0f,   0.5f, 0.0f,     0.6f, 1.0f, 0.2f,
         0.5f,  -0.4f, 0.0f,     1.0f, 0.2f, 1.0f
    };
    unsigned int indices[] = {
        0,1,2, // first tri
        3,1,2 // second tri
    };

    // VAO, VBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind VAO
    glBindVertexArray(VAO);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set attribute pointers
    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // set eup EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // ERROR CHECK
    cout << "after VAO,VBO bind | error code: " << glGetError() << endl;
    glCheckError();

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    shader.activate();
    shader.setMat4("transform", trans);

    glm::mat4 trans2 = glm::mat4(1.0f);
    trans2 = glm::scale(trans2, glm::vec3(1.5f));
    trans2 = glm::rotate(trans2, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    shader2.activate();
    shader2.setMat4("transform", trans);


    // MAIN LOOP
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // gray screen
        glClear(GL_COLOR_BUFFER_BIT);

        // rotate continuous
        trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.activate();
        shader.setMat4("transform", trans);
        
        // draw shapes
        glBindVertexArray(VAO);
        shader.activate();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        trans2 = glm::rotate(trans2, glm::radians((float)glfwGetTime() / -100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shader2.activate();
        shader2.setMat4("transform", trans2);

        shader2.activate();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));


        // send new frame to window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &EBO);

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
}

void processInput(GLFWwindow* window)
{
    // if ESC key is pressed, close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

