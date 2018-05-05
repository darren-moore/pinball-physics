#pragma once

#include "utils.h"
#include "table.h"
#include "Flipper.h"
#include "Sector.h"
#include <math.h>


class Ball {
public:
	vec2 pos;
	vec2 pos_1;
	vec2 vel;
	vec2 vel_1;
	vec2 accel;

	float r;
	float red;
	float green;
	float blue;

	float resti = .9;

	bool removeMe = false;

	Ball(vec2 _pos, float _r, float _red, float _green, float _blue)
	: pos(_pos), r(_r), red(_red), green(_green), blue(_blue), vel(vec2(0,0)),vel_1(vec2(0,0)), pos_1(_pos), accel(vec2(0,0)) {}

	float resolvePlanarCollision(CLineSegment seg, float dt);
	float resolveBallCollision(Ball &b2);
	void resolveSectorCollision(Sector sec, float force, float dt);
	void impulse(vec2 force);
	
private:
	vec2 getSecEnd(Sector sec, float angle);
	
	
};