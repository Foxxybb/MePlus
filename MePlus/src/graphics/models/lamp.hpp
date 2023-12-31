#ifndef LAMP_HPP
#define LAMP_HPP

#include "cube.hpp"
#include "../Light.h"
#include "../Material.h"

#include <glm/glm.hpp>

class Lamp : public Cube {
public:
    glm::vec3 lightColor;

    // light strength values
    PointLight pointLight;

    Lamp() {}

    Lamp(glm::vec3 lightColor,
        glm::vec4 ambient,
        glm::vec4 diffuse,
        glm::vec4 specular,
        float k0,
        float k1,
        float k2,
        glm::vec3 pos,
        glm::vec3 size)
        : lightColor(lightColor),
        pointLight({ pos, k0, k1, k2, ambient, diffuse, specular }),
        Cube( pos, size) {}

    void render(Shader shader, float dt) {
        // set light color
        shader.set3Float("lightColor", lightColor);
        Cube::render(shader, dt);

        // update pointlight position to match lamp model
        pointLight.position = this->rb.pos;
    }
};

#endif