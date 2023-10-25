#include "environment.h"

using namespace glm;

/*
    define constants in Environment class
*/

// up vector in world
vec3 Environment::worldUp = vec3(0.0f, 1.0f, 0.0f);

// acceleration due to gravity
vec3 Environment::gravitationalAcceleration = vec3(0.0f, -9.81f, 0.0f);