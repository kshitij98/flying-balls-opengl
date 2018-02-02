#include "main.h"

#ifndef SPIKES_H
#define SPIKES_H


class Spikes {
public:
    Spikes() {}
    Spikes(float start, float width, float range, float speed);
    void newShape(float start, float width, float range, float speed);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    float width;
    float h;
    float x;
    float y;
    float direction;
    void tick();
private:
    VAO *spikes;
};

#endif // SPIKES_H
