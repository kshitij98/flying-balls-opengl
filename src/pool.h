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
    // bounding_box_t bounding_box();
private:
    VAO *grass;
    VAO *sand;
    VAO *pool;
};

#endif // POOL_H
