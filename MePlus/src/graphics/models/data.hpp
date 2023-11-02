#ifndef DATA_HPP
#define DATA_HPP

#include "../Model.h"

using namespace glm;

class Data : public Model {
public:
	Data(vec3 pos = vec3(0.0f), vec3 size = vec3(1.0f))
		: Model(pos, size, true) {}

	void init() {
		loadModel("assets/models/dataCube.gltf");
	}
};

#endif
