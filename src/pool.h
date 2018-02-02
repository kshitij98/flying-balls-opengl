#include "main.h"

#ifndef POOL_H
#define POOL_H


class Pool {
public:
    Pool() {}
    Pool(float x, float y);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    float r;
    void tick();
    float rotation;
    float flow;
    float acceleration;
    void newShape(float start, float width);
    // bounding_box_t bounding_box();
private:
    VAO *grass;
    VAO *sand;
    VAO *pool;
    VAO *water;
};

#endif // POOL_H
