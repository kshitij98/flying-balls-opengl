#include <iostream>
using namespace std;

#include "magnet.h"
#include "main.h"


void magnet_stick(GLfloat arr[], float x1, float x2, float y1, float y2) {
    for (int i=0 ; i<36 ; ++i)
        arr[i] = 0;

    arr[0] = x1; arr[1] = y2;
    arr[3] = x1; arr[4] = y1;
    arr[6] = x2; arr[7] = y1;
    arr[9] = x1; arr[10] = y2;
    arr[12] = x2; arr[13] = y2;
    arr[15] = x2; arr[16] = y1;

    arr[18] = x1; arr[19] = -y2;
    arr[21] = x1; arr[22] = -y1;
    arr[24] = x2; arr[25] = -y1;
    arr[27] = x1; arr[28] = -y2;
    arr[30] = x2; arr[31] = -y2;
    arr[33] = x2; arr[34] = -y1;
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

    for (int i=0 ; i<n ; ++i) {
        for (int j=0 ; j<9 ; ++j)
            magnet_data[(i*9) + j] = 0.0f;

        magnet_data[i*9 + 3] = r * cos(((offset + deg * i) * M_PI) / 180.0);
        magnet_data[i*9 + 4] = r * sin(((offset + deg * i) * M_PI) / 180.0);

        magnet_data[i*9 + 6] = r * cos(((offset + deg * (i+1)) * M_PI) / 180.0);
        magnet_data[i*9 + 7] = r * sin(((offset + deg * (i+1)) * M_PI) / 180.0);
    }

    magnet_stick(magnet_data + 9*n, -r, 0.0f, (1.0f - thick) * r, r);
    magnet_stick(black_data, -r, -0.5f * r, (1.0f - thick) * r, r);

    for (int i=0 ; i<n ; ++i) {
        for (int j=0 ; j<9 ; ++j)
            cavity_data[(i*9) + j] = 0.0f;

        cavity_data[i*9 + 3] = (1.0f - thick) * r * cos(((offset + deg * i) * M_PI) / 180.0);
        cavity_data[i*9 + 4] = (1.0f - thick) * r * sin(((offset + deg * i) * M_PI) / 180.0);

        cavity_data[i*9 + 6] = (1.0f - thick) * r * cos(((offset + deg * (i+1)) * M_PI) / 180.0);
        cavity_data[i*9 + 7] = (1.0f - thick) * r * sin(((offset + deg * (i+1)) * M_PI) / 180.0);
    }
    float border = 0.01f;

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
