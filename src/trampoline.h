#include "main.h"

#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H


class Trampoline {
public:
    Trampoline() {}
    Trampoline(float x, float y);
    void newShape(float x, float y);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    float r;
    float h;
    float x;
    float y;
private:
    VAO *grass;
    VAO *sand;
    VAO *bed;
    VAO *poles;
};

#endif // TRAMPOLINE_H
