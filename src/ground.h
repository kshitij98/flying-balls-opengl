#include "main.h"

#ifndef GROUND_H
#define GROUND_H


class Ground {
public:
    Ground() {}
    Ground(float start, float width);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    float x;
    float y;
    float h;
    void newShape(float start, float width);
private:
    VAO *grass;
    VAO *sand;
};

#endif // GROUND_H
