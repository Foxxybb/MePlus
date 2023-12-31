#include "../Model.h"

#include "../../io/Camera.h"

using namespace glm;

class Gun : public Model {
public:
	Gun()
		: Model(vec3(0.0f), vec3(1/300.0f), true) {}

	void render(Shader shader, float dt, bool setModel = false) {
		mat4 model = mat4(1.0f);

		// set position
		vec3 down = cross(Camera::defaultCamera.cameraFront, Camera::defaultCamera.cameraRight);
		rb.pos = Camera::defaultCamera.cameraPos + vec3(Camera::defaultCamera.cameraFront * 0.2f) - vec3(Camera::defaultCamera.cameraUp * 0.1f);
		model = translate(model, rb.pos);

		float theta;

		// rotate around right
		theta = acos(dot(Camera::defaultCamera.worldUp, Camera::defaultCamera.cameraFront) /
			length(Camera::defaultCamera.cameraUp) / length(Camera::defaultCamera.cameraFront));
		model = rotate(model, atanf(1) * 2 - theta, Camera::defaultCamera.cameraRight);

		// rotate around cameraUp using dot product
		vec2 front2d = vec2(Camera::defaultCamera.cameraFront.x, Camera::defaultCamera.cameraFront.z);
		theta = acos(dot(vec2(1.0f, 0.0f), front2d) / length(front2d));
		model = rotate(model, Camera::defaultCamera.cameraFront.z < 0 ? theta : -theta, Camera::defaultCamera.worldUp);

		// scale
		model = scale(model, size);

		shader.setMat4("model", model);

		Model::render(shader, dt, false);
	}
};