#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "../Model.h"

using namespace glm;

class Block : public Model {
public:
	Block(vec3 pos = vec3(0.0f), vec3 size = vec3(1.0f))
		: Model(pos, size, true) {}

	void init() {
		loadModel("assets/models/w1block.gltf");
	}
};

#endif
