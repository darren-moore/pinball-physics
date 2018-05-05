#pragma once

#include "vec2.h"

struct Sector {
	vec2 foot;
	float length;

	// Angles in radians.
	float initAngle;
	float endAngle;

	// Denotes the parity of the sector's "fill"
	bool clockwise;

	Sector(vec2 _foot, float _length, float _initAngle, float _endAngle, bool _clockwise)
		: foot(_foot), length(_length), initAngle(_initAngle), endAngle(_endAngle), clockwise(_clockwise) {}

	vec2 getEnd(float angle) {
		return vec2(foot.x + length*cos(angle), foot.y + length*sin(angle));
	}
};