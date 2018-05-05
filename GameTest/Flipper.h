#pragma once

#include "utils.h"
#include "Sector.h"

class Flipper {
public:
	vec2 pos;
	float length;

	// Angles in radians.
	float initAngle; // world co-ords
	float rotAngle; // Amount of rotation, not final angle

	float speed;
	float currentAngle;

	bool clockwise;

	bool notMoving;
	bool activated;
	int controlKey;
	
	Flipper(vec2 _pos, float _length, float _initAngle, float _rotAngle, bool _clockwise, float _speed, int _controlKey)
		: pos(_pos), length(_length), initAngle(_initAngle), rotAngle(_rotAngle), clockwise(_clockwise), speed(_speed), controlKey(_controlKey),
		currentAngle(_initAngle), notMoving(true), activated(false){}

	// Return a sector corresponding to the amount of the flipper angle flipped
	Sector getSector(float percent) {
		return Sector(pos, length, initAngle, initAngle + percent*rotAngle, clockwise);
	}

	CLineSegment getSeg() {
		vec2 end = getEnd();
		return CLineSegment(pos.x, pos.y, end.x, end.y, LineType::eLine_Flipper);
	}

	vec2 getEnd() {
		return vec2(pos.x + length*cos(currentAngle), pos.y + length*sin(currentAngle));
	}

	// Immediate jerk to the end
	void moveToEnd() {
		currentAngle = initAngle + rotAngle;
	}

	// Smoother animation back towards resting state.
	void moveToStart() {
		int parity = 1;
		if (!clockwise) {
			if (currentAngle > initAngle)
				currentAngle -= speed;

			if (currentAngle < initAngle)
				currentAngle = initAngle;
		}
		else {
			if (currentAngle < initAngle)
				currentAngle += speed;

			if (currentAngle > initAngle)
				currentAngle = initAngle;

		}
	}

};