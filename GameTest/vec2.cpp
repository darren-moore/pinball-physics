#include "stdafx.h"
#include "vec2.h"


float vec2::dot(vec2 b){
	return (x*b.x + y*b.y);
}

vec2 vec2::normed() {
	return (*this) / length();
}

float vec2::length() {
	return sqrt(dot(*this));
}

vec2 vec2::normal() {
	return vec2(-y, x).normed();
}

vec2 vec2::projOnto(vec2 b) {
	vec2 unitb = b.normed();
	return (dot(b)/b.length()) * unitb;
}