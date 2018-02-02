#include <iostream>
using namespace std;

#include "scorecard.h"
#include "main.h"
#include "helper.h"

ScoreCard::ScoreCard(int time, float health, int points) {
    newShape(time, health, points);
}

void ScoreCard::newShape(int time, float health, int points) {
    this -> position = glm::vec3(0, -0.9f, -1);

    float half = 0.3f;

    GLfloat white_data[18];
    GLfloat red_data[18];

    rectangle(white_data, -half, half, 0, 0.1f);
    rectangle(red_data, -half, -half + health * (half*2), 0, 0.1f);

    this -> white = create3DObject(GL_TRIANGLES, 6, white_data, COLOR_WHITE, GL_FILL);
    this -> red = create3DObject(GL_TRIANGLES, 6, red_data, COLOR_BLACK, GL_FILL);
}

void ScoreCard::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this -> position);    // glTranslatef
    // glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this -> white);
    draw3DObject(this -> red);
}
