#include <iostream>
using namespace std;

#include "main.h"
#include "pool.h"


inline float Cos(float x) {
    return cos((x * M_PI) / 180.f);
}

inline float Sin(float x) {
    return sin((x * M_PI) / 180.f);
}

float yoffset;

Pool::Pool(float start, float width) {
    this -> rotation = 0;
    this -> flow = 0.1f;
    this -> acceleration = 0.005f;
    this -> r = (5.0f * width) / 8.0f;
    const float half = width / 2.0f;
    yoffset = ((3.0f * width) / 8.0f);
    this -> position = glm::vec3(start + width/2.0, -1 + yoffset, 0);

    const GLfloat grass_data[] = {
        -half, -yoffset, 0, // vertex 1
        half, -yoffset, 0, // vertex 2
        half, -20, 0, // vertex 3

        half, -20, 0, // vertex 3
        -half, -20, 0, // vertex 4
        -half, -yoffset, 0  // vertex 1
    };

    const GLfloat sand_data[] = {
        -half, 0 - 0.2 - yoffset, 0, // vertex 1
        half, 0 - 0.2 - yoffset, 0, // vertex 2
        half, -20, 0, // vertex 3

        half, -20, 0, // vertex 3
        -half, -20, 0, // vertex 4
        -half, 0 - 0.2 - yoffset, 0  // vertex 1
    };

    const int n = 1000;

    GLfloat pool_data[3*3*n];

    float deg = 360.0 / n;
    for (int i=0 ; i<n ; ++i) {
        for (int j=0 ; j<9 ; ++j)
            pool_data[(i*9) + j] = 0.0f;

        pool_data[i*9 + 3] = r * Cos(deg * i);
        pool_data[i*9 + 4] = r * Sin(deg * i);

        pool_data[i*9 + 6] = r * Cos(deg * (i+1));
        pool_data[i*9 + 7] = r * Sin(deg * (i+1));
    }

    this -> grass = create3DObject(GL_TRIANGLES, 6, grass_data, COLOR_GREEN, GL_FILL);
    this -> sand = create3DObject(GL_TRIANGLES, 6, sand_data, COLOR_RED, GL_FILL);
    this -> pool = create3DObject(GL_TRIANGLES, 3 * n, pool_data, COLOR_BACKGROUND, GL_FILL);

    GLfloat water_data[3*3*n];

    float cover = 104.0f;
    float offset = -(180.0f-cover) / 2.0f;

    // 180 * 2 / n
    deg = cover * 2.0 / n;
    float waterH = r * Sin(offset);
    for (int i=0 ; i<(n>>1) ; ++i) {
        for (int j=0 ; j<18 ; ++j)
            water_data[(i*18) + j] = 0.0f;

				water_data[i*18 + 0]  = (waterH / Sin(offset - deg * i)) * Cos(offset - deg * i);
				water_data[i*18 + 1]  = waterH;
            
        water_data[i*18 + 3]  = r * Cos(offset - deg * i);
        water_data[i*18 + 4]  = r * Sin(offset - deg * i);

        water_data[i*18 + 6]  = r * Cos(offset - deg * (i+1));
        water_data[i*18 + 7]  = r * Sin(offset - deg * (i+1));

        water_data[i*18 + 9]  = (waterH / Sin(offset - deg * i)) * Cos(offset - deg * i);
        water_data[i*18 + 10]  = waterH;

        water_data[i*18 + 12]  = r * Cos(offset - deg * (i+1));
        water_data[i*18 + 13]  = r * Sin(offset - deg * (i+1));

        water_data[i*18 + 15]  = (waterH / Sin(offset - deg * (i+1))) * Cos(offset - deg * (i+1));
        water_data[i*18 + 16]  = waterH;
    }

    this -> water = create3DObject(GL_TRIANGLES, 3 * n, water_data, COLOR_BLUE, GL_FILL);
}

void Pool::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this -> position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this -> grass);
    draw3DObject(this -> sand);
    draw3DObject(this -> pool);
    Matrices.model = glm::mat4(1.0f) * glm::translate (this -> position) * rotate;
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this -> water);
}

void Pool::tick() {
    this -> rotation += this -> flow; 

    if (abs(this -> flow) >= 0.1f)
        this -> acceleration *= -1.0f;

    this -> flow += this -> acceleration;
}