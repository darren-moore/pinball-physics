#pragma once
#include <math.h>

struct vec2 {
	float x;
	float y;

	vec2() : x(0), y(0){}
	vec2(float _x, float _y) : x(_x), y(_y) {}

	float dot(vec2 b);
	vec2 normed();
	float length();
	vec2 normal();
	vec2 projOnto(vec2 b);


	// Vector addition
	vec2& operator+=(const vec2& b) {
		this->x += b.x;
		this->y += b.y;
		return *this;
	}
	vec2& operator-=(const vec2& b) {
		this->x -= b.x;
		this->y -= b.y;
		return *this;
	}

	// Scalar multiplication
	vec2& operator*=(float s) {
		this->x *= s;
		this->y *= s;
		return *this;
	}
	vec2& operator/=(float s) {
		this->x /= s;
		this->y /= s;
		return *this;
	}


	


};

inline vec2 operator*(vec2 a, float s) {
	a *= s;
	return a;
}
inline vec2 operator*(float s, vec2 a) {
	return a * s;
}

inline vec2 operator/(vec2 a, float s) {
	a /= s;
	return a;
}

inline vec2 operator+(vec2 a, vec2 b) {
	a += b;
	return a;
}
inline vec2 operator-(vec2 a, vec2 b) {
	a -= b;
	return a;
}
