#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <glm/glm.hpp>

using namespace glm;

/*
    Environment class
    - stores values for environment (constants, etc)

    NOTE: 1 cubic unit in OpenGL space is equivalent to 1 meter cubed
*/

class Environment {
public:
    static vec3 worldUp;						// up vector in world
    static vec3 gravitationalAcceleration;		// acceleration due to gravity
};

#endif