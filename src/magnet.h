#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H

// const double gravity = 0.004;
// const double max_speed = 0.05;
// const double accelerationX = 0.004;

class Magnet {
public:
    Magnet() {}
    Magnet(float power);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    float r;
    int direction;
    float thick;
    float rotation;
    float power;
    void changeDir(int dir);
private:
    VAO *magnet;
    VAO *cavity;
    VAO *black;
};

#endif // MAGNET_H
