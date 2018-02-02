#include <iostream>
using namespace std;

#include "enemy.h"
#include "main.h"
#include "helper.h"

float xrange = 1.0f;
float yrange = 0.05f;


Enemy::Enemy(float x, float y, color_t color, float r, float _speed, bool hasPlank, float angle) {
    newShape(x, y, color, r, _speed, hasPlank, angle);
}

void Enemy::newShape(float x, float y, color_t color, float r, float _speed, bool hasPlank, float angle) {
    this -> position = glm::vec3(x, y, 0);
    this -> speed = glm::vec3(_speed, 0, 0);
    this -> r = r;
    this -> rotation = angle;
    this -> hasPlank = hasPlank;

    const int n = 30;

    GLfloat enemy_data[3*3*n];
    pie(enemy_data, r, n, 0, 360);

    GLfloat plank_data[18];
    rectangle(plank_data, -r, r, r, 1.3f * r);

    this -> ball = create3DObject(GL_TRIANGLES, 3*n, enemy_data, color, GL_FILL);
    this -> plank = create3DObject(GL_TRIANGLES, 6, plank_data, COLOR_BROWN, GL_FILL);
}

void Enemy::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this -> position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this -> ball);
    if (this -> hasPlank)
	    draw3DObject(this -> plank);
}

glm::vec3 Enemy::detectCollision(glm::vec3 _position, float radius, glm::vec3 _speed) {
    glm::vec3 distance = this -> position + glm::vec3(0, r, 0) - (_position - glm::vec3(0, radius, 0));

    glm::vec3 disp = _position - this -> position;
    glm::mat4 rotate = glm::rotate((float) (this -> rotation * -M_PI / 180.0f), glm::vec3(0, 0, 1));
    disp = glm::vec3(rotate * glm::vec4(disp, 1.0));

    if (this -> hasPlank && -r <= disp.x && disp.x <= r && abs(disp.y - radius - 1.3f*r) < abs(_speed.y) && _speed.y < 0) {
    	return glm::vec3(sin(this -> rotation * -M_PI / 180.0f), cos(this -> rotation * -M_PI / 180.0f), 0);
    	cerr << "DEFLECT\n";
    }

    if (abs(distance.x) < this -> r && abs(distance.y) < abs(_speed.y) && _speed.y < 0.0f) {
        cerr << "HIT!\n";
        return glm::vec3(0, 1, 0);
    }
    return glm::vec3(0, 0, -1);
}

void Enemy::tick() {
    this -> position += speed;
}