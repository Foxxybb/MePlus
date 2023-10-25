#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>

using namespace glm;

class RigidBody {
public:
	float mass;

	vec3 pos;
	vec3 velocity;
	vec3 acceleration;

	RigidBody(float mass = 1.0f, vec3 pos = vec3(0.0f), vec3 velocity = vec3(0.0f), vec3 acceleration = vec3(0.0f));

	void update(float dt);
};

#endif // !RIGIDBODY_H

