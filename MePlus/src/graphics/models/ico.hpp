#ifndef ICO_HPP
#define ICO_HPP

#include "../Model.h"

using namespace glm;

class Ico : public Model {
public:
	Ico(vec3 pos = vec3(0.0f), vec3 size = vec3(1.0f))
		: Model(pos, size, true) {}

	void init() {
		loadModel("assets/models/ico.gltf");
	}
};

#endif // !ICO_HPP
