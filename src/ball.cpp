#include <iostream>
using namespace std;

#include "ball.h"
#include "main.h"

double gravity;

Ball::Ball(float x, float y, color_t color) {
    this -> position = glm::vec3(x, y, 0);
    this -> rotation = 0;
    this -> max_speed = 0.05;
    this -> acceleration = 0.004;
    this -> directionX = 0;
    speed = 0;
    gravity = 0.004;
    static const GLfloat vertex_buffer_data[] = {
        -0.2, -0.2, 0, // vertex 1
        0.2,  -0.2, 0, // vertex 2
        0.2,  0.2, 0, // vertex 3

        0.2,  0.2, 0, // vertex 3
        -0.2, 0.2, 0, // vertex 4
        -0.2, -0.2, 0 // vertex 1
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
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
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this -> position.x += speed;
    this -> position.y += speedY;

    this -> directionX = this -> speed ? ((this -> speed > 0) ? 1 : -1) : 0;
    this -> speedY -= gravity;

    // this->position.y -= speed;
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}

void Ball::move(char direction) {
	if (direction == 'r')
		this -> speed = max(this -> speed, min(this -> speed + this -> acceleration, this -> max_speed));
	else if (direction == 'l')
		this -> speed = min(this -> speed, max(this -> speed - this -> acceleration, -(this -> max_speed)));
	else {
		if (this -> directionX > 0)
			this -> speed = max(0.0, this -> speed - this -> acceleration);
		else if (this -> directionX < 0)
			this -> speed = min(0.0, this -> speed + this -> acceleration);
	}
}

void Ball::jump() {
	// TODO: If lower half is colliding with floor, or something...
	this -> speedY = 0.1;
}