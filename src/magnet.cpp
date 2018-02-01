#include <iostream>
using namespace std;

#include "magnet.h"
#include "main.h"
#include "helper.h"

inline void magnet_stick(GLfloat arr[], float x1, float x2, float y1, float y2) {
    rectangle(arr, x1, x2, y1, y2);
    rectangle(arr+18, x1, x2, -y1, -y2);
}

Magnet::Magnet(float power) {
    this -> position = glm::vec3(0.93f, 0.8f, 1);
    this -> thick = 0.7f;
    this -> rotation = 0;
    this -> power = power;
    this -> direction = 1;

    const int n = 30;

    GLfloat magnet_data[9 * (n + 4)];
    GLfloat cavity_data[9 * n];
    GLfloat black_data[9 * 4];

    float deg = 180.0 / n;
    float r = 0.1f;  
    float offset = -90.0f;

    pie(magnet_data, r, n, offset, 180);
    magnet_stick(magnet_data + 9*n, -r, 0.0f, (1.0f - thick) * r, r);
    magnet_stick(black_data, -r, -0.5f * r, (1.0f - thick) * r, r);
    pie(cavity_data, (1.0f - thick) * r, n, offset, 180);

    this -> magnet = create3DObject(GL_TRIANGLES, 3*(n+4), magnet_data, COLOR_RED, GL_FILL);
    this -> black = create3DObject(GL_TRIANGLES, 3*4, black_data, COLOR_BLACK, GL_FILL);
    this -> cavity = create3DObject(GL_TRIANGLES, 3*n, cavity_data, COLOR_BACKGROUND, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this -> position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this -> magnet);
    draw3DObject(this -> cavity);
    draw3DObject(this -> black);
}

void Magnet::changeDir(int direction) {
    this -> direction = direction;
    if (direction == 1) {
        rotation = 0;
        this -> position = glm::vec3(0.93f, 0.8f, 1);
    }
    else {
        rotation = 180;
        this -> position = glm::vec3(-0.93f, 0.8f, 1);       
    }
}
