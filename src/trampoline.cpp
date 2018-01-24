#include <iostream>
using namespace std;

#include "main.h"
#include "pool.h"


// float yoffset;


void rectangle(GLfloat arr[], float x1, float x2, float y1, float y2) {
    for (int j=0 ; j<18 ; ++j)
        arr[j] = 0.0f;

    arr[0]  = x1;
    arr[1]  = y1;
            
    arr[3]  = x2;
    arr[4]  = y1;

    arr[6]  = x2;
    arr[7]  = y2;

    arr[9]  = x1;
    arr[10] = y1;

    arr[12] = x2;
    arr[13] = y2;

    arr[15] = x1;
    arr[16] = y2;
}

Trampoline::Trampoline(float start, float width) {
    this -> r = (5.0f * width) / 6.0f;
    const float half = width / 2.0f;
    float cover = 74.0f;
    float offset = -(180.0f-cover) / 2.0f;
    yoffset = r;
    this -> position = glm::vec3(start + half, -1 + yoffset, 0);

    this -> x = start;
    this -> y = start + width;

    const int n = 100;

    GLfloat bed_data[3*3*n];

    // 180 * 2 / n
    float deg = cover * 2.0 / n;
    float waterH = r * Sin(offset);

    this -> h = -1 + yoffset + waterH;
    
    for (int i=0 ; i<(n>>1) ; ++i) {
        for (int j=0 ; j<18 ; ++j)
            bed_data[(i*18) + j] = 0.0f;

		bed_data[i*18 + 0]  = (waterH / Sin(offset - deg * i)) * Cos(offset - deg * i);
		bed_data[i*18 + 1]  = waterH;
            
        bed_data[i*18 + 3]  = r * Cos(offset - deg * i);
        bed_data[i*18 + 4]  = r * Sin(offset - deg * i);

        bed_data[i*18 + 6]  = r * Cos(offset - deg * (i+1));
        bed_data[i*18 + 7]  = r * Sin(offset - deg * (i+1));

        bed_data[i*18 + 9]  = (waterH / Sin(offset - deg * i)) * Cos(offset - deg * i);
        bed_data[i*18 + 10]  = waterH;

        bed_data[i*18 + 12]  = r * Cos(offset - deg * (i+1));
        bed_data[i*18 + 13]  = r * Sin(offset - deg * (i+1));

        bed_data[i*18 + 15]  = (waterH / Sin(offset - deg * (i+1))) * Cos(offset - deg * (i+1));
        bed_data[i*18 + 16]  = waterH;
    }

    this -> bed = create3DObject(GL_TRIANGLES, 3 * n, bed_data, COLOR_BLACK, GL_FILL);

    GLfloat poles_data[4*3*3];

    rectangle(poles_data, -half, -half+(0.1 * r), -r, (-4.0/5.0) * r);
    rectangle(poles_data + 18, half-(0.1 * r), half, -r, (-4.0/5.0) * r);

    this -> poles = create3DObject(GL_TRIANGLES, 3 * n, poles_data, COLOR_BLACK, GL_FILL);
}

void Trampoline::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this -> position);    // glTranslatef
    // glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this -> bed);
    draw3DObject(this -> poles);
}
