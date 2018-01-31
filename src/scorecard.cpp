#include <iostream>
using namespace std;

#include "scorecard.h"
#include "main.h"

float xrange = 1.0f;
float yrange = 0.05f;


ScoreCard::ScoreCard(float x, float y, color_t color, float r, float _speed) {
    this -> position = glm::vec3(x, y, 0);
    this -> speed = glm::vec3(_speed, 0, 0);
    this -> r = r;

    const int n = 30;

    GLfloat scorecard_data[3*3*n];

    float deg = 360.0 / n;
    for (int i=0 ; i<n ; ++i) {
        for (int j=0 ; j<9 ; ++j)
            scorecard_data[(i*9) + j] = 0.0f;

        scorecard_data[i*9 + 3] = r * cos((deg * i * M_PI) / 180.0);
        scorecard_data[i*9 + 4] = r * sin((deg * i * M_PI) / 180.0);

        scorecard_data[i*9 + 6] = r * cos((deg * (i+1) * M_PI) / 180.0);
        scorecard_data[i*9 + 7] = r * sin((deg * (i+1) * M_PI) / 180.0);
    }

    this -> object = create3DObject(GL_TRIANGLES, 3*n, scorecard_data, color, GL_FILL);
}

void ScoreCard::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= translate;
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this -> object);
}

glm::vec3 ScoreCard::detectCollision(glm::vec3 _position, glm::vec3 _speed) {
    glm::vec3 distance = this -> position - _position + glm::vec3(0, r, 0);
    if (abs(distance.x) < this -> r && abs(distance.y) < abs(_speed.y) && _speed.y < 0.0f) {
        cerr << "HIT!\n";
        return glm::vec3(0, 1, 0);
    }
    return glm::vec3(0, 0, -1);
}

// void ScoreCard::set_position(float x, float y) {
//     this -> position = glm::vec3(x, y, 0);
// }

void ScoreCard::tick() {
    // glm::vec3 damp = (this -> inside ? glm::vec3(0.2, 0.2, 0) : glm::vec3(1, 1, 0));
    this -> position += speed;
    // this -> directionX = speed.x != 0.0 ? ((speed.x > 0) ? 1 : -1) : 0;
    // speed.y -=   gravity;

    // this->position.y -= speed;
}
