#include "main.h"

#ifndef GROUND_H
#define GROUND_H


class Ground {
public:
    Ground() {}
    Ground(float start, float width);
    glm::vec3 position;
    // float rotation;
    void draw(glm::mat4 VP);
    float x;
    float y;
    float h;
    // void set_position(float x, float y);
    // void tick();
    // int directionX;
    // double speed;
    // double speedY;
    // double max_speed;
    // double acceleration;
    // void move(char direction);
    // void jump();
    // bounding_box_t bounding_box();
private:
    VAO *grass;
    VAO *sand;
};

#endif // GROUND_H
