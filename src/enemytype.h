#include "main.h"

#ifndef ENEMYTYPE_H
#define ENEMYTYPE_H


class EnemyType {
public:
    EnemyType() {}
    EnemyType(float y1, float y2, float sz1, float sz2, float speed1, float speed2, color_t color, int points);
    void newVals(float y1, float y2, float sz1, float sz2, float speed1, float speed2, color_t color, int points);
		void getVals(float &y, float &sz, float &speed, color_t &color, int &points);
		void print();

		float y1;
		float y2;
		float sz1;
		float sz2;
		float speed1;
		float speed2;
		color_t color;
		int points;
};

#endif // ENEMYTYPE_H
