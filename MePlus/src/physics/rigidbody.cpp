#include "rigidbody.h"

RigidBody::RigidBody(float mass, vec3 pos, vec3 velocity, vec3 acceleration)
	: mass(mass), pos(pos), velocity(velocity), acceleration(acceleration) {}

void RigidBody::update(float dt) {
	pos += velocity * dt + 0.5f * acceleration * (dt * dt);
	velocity += acceleration * dt;
}