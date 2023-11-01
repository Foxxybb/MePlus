#ifndef STAGE_HPP
#define STAGE_HPP

#include "../Model.h"

using namespace glm;

class Stage : public Model {
public:
	Stage(vec3 pos = vec3(0.0f), vec3 size = vec3(1.0f))
		: Model(pos, size, true) {}

	void init() {
		loadModel("assets/models/blockStage.gltf");
	}
};

#endif
