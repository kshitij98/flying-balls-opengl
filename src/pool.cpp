#include <iostream>
using namespace std;

#include "main.h"
#include "pool.h"


Pool::Pool(float start, float width) {
    // this -> rotation = 0;
    // this -> acceleration = 0.004;
    this -> r = (5.0f * width) / 8.0f;
    const float half = width / 2.0f;
    const float yoffset = ((3.0f * width) / 8.0f);
    this -> position = glm::vec3(start + width/2.0, -1 + yoffset, 0);

    const GLfloat grass_data[] = {
        -half, 0, 0, // vertex 1
        half, 0, 0, // vertex 2
        half, -20, 0, // vertex 3

        half, -20, 0, // vertex 3
        -half, -20, 0, // vertex 4
        -half, 0, 0  // vertex 1
    };

    const GLfloat sand_data[] = {
        -half, 0 - 0.2 - yoffset, 0, // vertex 1
        half, 0 - 0.2 - yoffset, 0, // vertex 2
        half, -20, 0, // vertex 3

        half, -20, 0, // vertex 3
        -half, -20, 0, // vertex 4
        -half, 0 - 0.2 - yoffset, 0  // vertex 1
    };

    const int n = 100;

    GLfloat pool_data[3*3*n];

    float deg = 360.0 / n;
    for (int i=0 ; i<n ; ++i) {
        for (int j=0 ; j<9 ; ++j)
            pool_data[(i*9) + j] = 0.0f;

       	pool_data[i*9 + 1] = 0.0f;

        pool_data[i*9 + 3] = r * cos((deg * i * M_PI) / 180.0);
        pool_data[i*9 + 4] = r * sin((deg * i * M_PI) / 180.0);

        pool_data[i*9 + 6] = r * cos((deg * (i+1) * M_PI) / 180.0);
        pool_data[i*9 + 7] = r * sin((deg * (i+1) * M_PI) / 180.0);
    }

    this -> grass = create3DObject(GL_TRIANGLES, 6, grass_data, COLOR_GREEN, GL_FILL);
    this -> sand = create3DObject(GL_TRIANGLES, 6, sand_data, COLOR_RED, GL_FILL);
    this -> pool = create3DObject(GL_TRIANGLES, 3 * n, pool_data, COLOR_BACKGROUND, GL_FILL);



    // this -> water = 
}

void Pool::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this -> position);    // glTranslatef
    // glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this -> grass);
    draw3DObject(this -> sand);
    draw3DObject(this -> pool);
}
