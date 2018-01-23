#include "main.h"

#ifndef ENEMY_H
#define ENEMY_H

const double gravity = 0.004;
// const double max_speed = 0.05;
// const double accelerationX = 0.004;

class Enemy {
public:
    Enemy() {}
    Enemy(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    int directionX;
    glm::vec3 speed;
    float max_speed;
    float acceleration;
    void move(char direction);
    void jump();
    bounding_box_t bounding_box();
    bool inside;
private:
    VAO *object;
};

#endif // ENEMY_H
