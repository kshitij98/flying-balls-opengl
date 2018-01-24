#include <iostream>
using namespace std;

#include "main.h"
#include "spikes.h"
// #include "helper.h"
// #include "helper.cpp"

// float yoffset;

Spikes::Spikes(float start, float width, float range, float speed) {
    this -> width = width;
    const float half = width / 2.0f;

    this -> position = glm::vec3(start, -1, 0);
    this -> x = start;
    this -> y = start + range;
    this -> direction = 0.01f;

    int n = width / 0.25;

    GLfloat spikes_data[9 * 50];

    for (int i=0 ; i<n ; ++i) {
        for (int j=0 ; j<9 ; ++j)
            spikes_data[i*9 + j] = 0.0f;
        
        spikes_data[i*9 + 0]  = i * (width / (float)n);
        spikes_data[i*9 + 1]  = 0.0f;
                
        spikes_data[i*9 + 3]  = (i + 1) * (width / (float)n);
        spikes_data[i*9 + 4]  = 0.0f;
    
        spikes_data[i*9 + 6]  = (i + 0.5f) * (width / (float)n);
        spikes_data[i*9 + 7]  = 0.2f;
    }

    this -> spikes = create3DObject(GL_TRIANGLES, 4*3*3, spikes_data, COLOR_BLACK, GL_FILL);
}

void Spikes::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this -> position);    // glTranslatef
    // glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this -> spikes);
}

void Spikes::tick() {
    if (this -> position.x < this -> x || this -> y < this -> position.x + this -> width)
        this -> direction *= -1.0f;

    this -> position.x += this -> direction;
}