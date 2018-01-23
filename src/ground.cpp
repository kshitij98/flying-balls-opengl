#include <iostream>
using namespace std;

#include "main.h"
#include "ground.h"


Ground::Ground(float start, float width) {
		float r = width / 2.0;

		this -> x = start;
		this -> y = start + width;
		this -> h = -1;
    this -> position = glm::vec3(start + r, 0, 0);

    cerr << this -> x << ' ' << this -> y << endl;

    GLfloat vertex_buffer_data[] = {
        -r, -1, 0, // vertex 1
        r, -1, 0, // vertex 2
        r, -20, 0, // vertex 3

        r, -20, 0, // vertex 3
        -r, -20, 0, // vertex 4
        -r, -1, 0  // vertex 1
    };

    GLfloat vertex_buffer_data2[] = {
        -r, -1 - 0.2, 0, // vertex 1
        r, -1 - 0.2, 0, // vertex 2
        r, -20, 0, // vertex 3

        r, -20, 0, // vertex 3
        -r, -20, 0, // vertex 4
        -r, -1 - 0.2, 0  // vertex 1
    };

    this -> grass = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, COLOR_GREEN, GL_FILL);
    this -> sand = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data2, COLOR_RED, GL_FILL);
}

void Ground::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this -> position);    // glTranslatef
    // glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this -> grass);
    draw3DObject(this -> sand);
}
