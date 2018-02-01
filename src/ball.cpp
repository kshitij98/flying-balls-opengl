#include <iostream>
using namespace std;

#include "ball.h"
#include "main.h"
#include "helper.h"


Ball::Ball(float x, float y, color_t color) {
    this -> position = glm::vec3(x, y, 0);
    this -> rotation = 0;
    this -> max_speed = 0.05f;
    this -> directionX = 0;
    this -> inside = false;

    // speed = 0;
    // gravity = 0.004;

    this -> speed = glm::vec3(0, 0, 0);

    const int n = 30;

    GLfloat ball_data[3*3*n];
    pie(ball_data, 0.2, n, 0, 360);

    this -> object = create3DObject(GL_TRIANGLES, 3*n, ball_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this -> position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    glm::vec3 damp = (this -> inside ? glm::vec3(0.2, 0.2, 0) : glm::vec3(1, 1, 0));
    this -> position += damp * speed;
    this -> directionX = speed.x != 0.0 ? ((speed.x > 0) ? 1 : -1) : 0;
    speed.y -= gravity;

    // this->position.y -= speed;
}

bounding_box_t Ball::bounding_box() {
    float x = this -> position.x, y = this -> position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}

void Ball::move(char direction, float acc) {
	if (direction == 'r')
        speed.x = max(speed.x, min(speed.x + acc, (this -> max_speed)));
	else if (direction == 'l')
        speed.x = min(speed.x, max(speed.x - acc, -(this -> max_speed)));
	else {
		if (this -> directionX > 0)
            speed.x = max(0.0f, speed.x - acc);
		else if (this -> directionX < 0)
            speed.x = min(0.0f, speed.x + acc);
	}
}

void Ball::jump() {
	// TODO: If lower half is colliding with floor, or something...
	speed.y = 0.1;
}
