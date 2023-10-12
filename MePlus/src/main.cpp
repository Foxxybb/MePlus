#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
string loadShaderSrc(const char* filename);
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
    // Compile vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    string vertShaderSrc = loadShaderSrc("assets/vertex_core.glsl");
    const GLchar* vertShader = vertShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &vertShader, NULL);
    glCompileShader(vertexShader);

    // catch error
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "Error with vertex shader comp.:" << std::endl << infoLog << std::endl;
    }

    // Compile fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    string fragShaderSrc = loadShaderSrc("assets/fragment_core.glsl");
    const GLchar* fragShader = fragShaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &fragShader, NULL);
    glCompileShader(fragmentShader);

    // catch error
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "Error with frag shader comp.:" << std::endl << infoLog << std::endl;
    }

    // BIND fragments to verticies
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // catch error
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Linking error:" << std::endl << infoLog << std::endl;
    }

    // cleanup shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // vertex array
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    // VAO, VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind VAO
    glBindVertexArray(VAO);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    cout << "after VAO,VBO bind: " << glGetError() << endl;
    glCheckError();

    // MAIN LOOP
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // gray screen
        glClear(GL_COLOR_BUFFER_BIT);

        /*cout << "before draw: " << glGetError() << endl;
        glCheckError();*/

        // draw shapes
        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLES,0,3); // (this breaks)

        // send new frame to window
        glfwSwapBuffers(window);
        glfwPollEvents();
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
}

void processInput(GLFWwindow* window)
{
    // if ESC key is pressed, close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

string loadShaderSrc(const char* filename)
{
    ifstream file;
    stringstream buf;

    string ret = "";

    file.open(filename);

    if (file.is_open())
    {
        buf << file.rdbuf();
        ret = buf.str();
    }
    else {
        cout << "Could not open " << filename << endl;
    }

    file.close();
    return ret;
}