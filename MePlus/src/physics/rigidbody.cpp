#include "rigidbody.h"

RigidBody::RigidBody(float mass, vec3 pos, vec3 velocity, vec3 acceleration)
	: mass(mass), pos(pos), velocity(velocity), acceleration(acceleration) {}

void RigidBody::update(float dt) {
	pos += velocity * dt + 0.5f * acceleration * (dt * dt);
	velocity += acceleration * dt;
}

void RigidBody::applyForce(vec3 force) {
	acceleration += force / mass;
}

void RigidBody::applyForce(vec3 direction, float magnitude) {
	applyForce(direction * magnitude);
}

// apply an acceleration (remove redundancy of dividing by mass)
void RigidBody::applyAcceleration(glm::vec3 a) {
	acceleration += a;
}

// apply an acceleration (remove redundancy of dividing by mass)
void RigidBody::applyAcceleration(glm::vec3 direction, float magnitude) {
	applyAcceleration(direction * magnitude);
}

void RigidBody::applyImpulse(vec3 force, float dt) {
	velocity += force / mass * dt;
}

void RigidBody::applyImpulse(vec3 direction, float magnitude, float dt) {
	applyImpulse(direction * magnitude, dt);
}

void RigidBody::transferEnergy(float joules) {
	if (joules == 0) {
		return;
	}

	// comes from formula: KE = 1/2 * m * v^2
	//vec3 deltaV = sqrt(2 * abs(joules) / mass) * direction;
	float deltaV = sqrt(2 * abs(joules) / mass);
	velocity += joules > 0 ? deltaV : -deltaV;


}