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

	void applyForce(vec3 force);
	void applyForce(vec3 direction, float magnitude);

	// apply an acceleration (remove redundancy of dividing by mass)
	void applyAcceleration(glm::vec3 a);

	// apply an acceleration (remove redundancy of dividing by mass)
	void applyAcceleration(glm::vec3 direction, float magnitude);

	void applyImpulse(vec3 force, float dt);
	void applyImpulse(vec3 direction, float magnitude, float dt);
	
	void transferEnergy(float joules);
};

#endif // !RIGIDBODY_H

