#include "helper.h"

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

float Cos(float x) {
    return cos((x * M_PI) / 180.f);
}

float Sin(float x) {
    return sin((x * M_PI) / 180.f);
}

void pie(GLfloat arr[], float r, int n, float offset, float deg) {
    deg /= (float)n;
    for (int i=0 ; i<n ; ++i) {
        for (int j=0 ; j<9 ; ++j)
            arr[(i*9) + j] = 0.0f;

        arr[i*9 + 3] = r * Cos(offset + deg * i);
        arr[i*9 + 4] = r * Sin(offset + deg * i);

        arr[i*9 + 6] = r * Cos(offset + deg * (i+1));
        arr[i*9 + 7] = r * Sin(offset + deg * (i+1));
    }
}