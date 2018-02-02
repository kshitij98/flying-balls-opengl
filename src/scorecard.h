#include "main.h"

#ifndef SCORECARD_H
#define SCORECARD_H

class ScoreCard {
public:
    ScoreCard() {}
    ScoreCard(int time, float health, int points);
    void newShape(int time, float health, int points);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    int timeLeft;
    int points;
private:
    VAO *white;
    VAO *red;
};

#endif // SCORECARD_H
