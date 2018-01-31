#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H

// const double gravity = 0.004;
// const double max_speed = 0.05;
// const double accelerationX = 0.004;

class Magnet {
public:
    Magnet() {}
    Magnet(int direction);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    float r;
    float direction;
    float thick;
private:
    VAO *magnet;
    VAO *cavity;
    VAO *black;
};

#endif // MAGNET_H
