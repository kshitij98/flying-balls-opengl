#include <iostream>
using namespace std;

#include "enemytype.h"
#include "main.h"
#include "helper.h"


EnemyType::EnemyType(float y1, float y2, float sz1, float sz2, float speed1, float speed2, color_t color) {
    newVals(y1, y2, sz1, sz2, speed1, speed2, color);
}

void EnemyType::newVals(float y1, float y2, float sz1, float sz2, float speed1, float speed2, color_t color) {
    this -> y1 = y1;
    this -> y2 = y2;
    this -> sz1 = sz1;
    this -> sz2 = sz2;
    this -> speed1 = speed1;
    this -> speed2 = speed2;
    this -> color = color;
}

void EnemyType::getVals(float &y, float &sz, float &speed, color_t &color) {
    y = y1 + (y2 - y1) * (randomNum(0, 10000) / 10000.0f);
    sz = sz1 + (sz2 - sz1) * (randomNum(0, 10000) / 10000.0f);
    speed = speed1 + (speed2 - speed1) * (randomNum(0, 10000) / 10000.0f);
    color = this -> color;
}

void EnemyType::print() {
    cerr << "y = " << y1 << " - " << y2 << endl;
    cerr << "sz = " << sz1 << " - " << sz2 << endl;
    cerr << "speed = " << speed1 << " - " << speed2 << endl;
}
