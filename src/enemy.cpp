#include <iostream>
using namespace std;

#include "enemy.h"
#include "main.h"

float xrange = 1.0f;
float yrange = 0.05f;


Enemy::Enemy(float x, float y, color_t color, float r, float _speed, bool hasPlank, float angle) {
    this -> position = glm::vec3(x, y, 0);
    this -> speed = glm::vec3(_speed, 0, 0);
    this -> r = r;
    this -> rotation = angle;
    this -> hasPlank = hasPlank;

    const int n = 30;

    GLfloat enemy_data[3*3*n];

    float deg = 360.0 / n;
    for (int i=0 ; i<n ; ++i) {
        for (int j=0 ; j<9 ; ++j)
            enemy_data[(i*9) + j] = 0.0f;

        enemy_data[i*9 + 3] = r * cos((deg * i * M_PI) / 180.0);
        enemy_data[i*9 + 4] = r * sin((deg * i * M_PI) / 180.0);

        enemy_data[i*9 + 6] = r * cos((deg * (i+1) * M_PI) / 180.0);
        enemy_data[i*9 + 7] = r * sin((deg * (i+1) * M_PI) / 180.0);
    }


    GLfloat plank_data[18];

    for (int i=0 ; i<18 ; ++i)
    	plank_data[i] = 0.0f;

    plank_data[0] = -r;
    plank_data[1] = 1.3f * r;

    plank_data[3] = -r;
    plank_data[4] = r;

    plank_data[6] = r;
    plank_data[7] = r;

    plank_data[9] = r;
    plank_data[10] = r;

    plank_data[12] = r;
    plank_data[13] = 1.3f * r;

    plank_data[15] = -r;
    plank_data[16] = 1.3f * r;

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
    // disp *= rotate;

    if (-r <= disp.x && disp.x <= r && abs(disp.y - radius - 1.3f*r) < abs(_speed.y) && _speed.y < 0) {
    	return glm::vec3(sin(this -> rotation * -M_PI / 180.0f), cos(this -> rotation * -M_PI / 180.0f), 0);
    	cerr << "DEFLECT\n";
    }

    cerr << "disp = " << disp.x << ' ' << disp.y << endl; 

    // if (this -> hasPlank && ) {

    // }

    if (abs(distance.x) < this -> r && abs(distance.y) < abs(_speed.y) && _speed.y < 0.0f) {
        cerr << "HIT!\n";
        return glm::vec3(0, 1, 0);
    }
    return glm::vec3(0, 0, -1);
}

// void Enemy::set_position(float x, float y) {
//     this -> position = glm::vec3(x, y, 0);
// }

void Enemy::tick() {
    // glm::vec3 damp = (this -> inside ? glm::vec3(0.2, 0.2, 0) : glm::vec3(1, 1, 0));
    this -> position += speed;
    // this -> directionX = speed.x != 0.0 ? ((speed.x > 0) ? 1 : -1) : 0;
    // speed.y -=   gravity;

    // this->position.y -= speed;
}

// bounding_box_t Enemy::bounding_box() {
//     float x = this -> position.x, y = this -> position.y;
//     bounding_box_t bbox = { x, y, 0.4, 0.4 };
//     return bbox;
// }

// void Enemy::move(char direction) {
// 	if (direction == 'r')
//         speed.x = max(speed.x, min(speed.x + this -> acceleration, (this -> max_speed)));
// 	else if (direction == 'l')
//         speed.x = min(speed.x, max(speed.x - this -> acceleration, -(this -> max_speed)));
// 	else {
// 		if (this -> directionX > 0)
//             speed.x = max(0.0f, speed.x - this -> acceleration);
// 		else if (this -> directionX < 0)
//             speed.x = min(0.0f, speed.x + this -> acceleration);
// 	}
// }

// void Enemy::jump() {
// 	// TODO: If lower half is colliding with floor, or something...
// 	speed.y = 0.1;
// }
