#include "vector_math.h"
#include "math.h"

Vector2::Vector2() {}

Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2 Vector2::operator+(Vector2 v) {
	return Vector2(x+v.x, y+v.y);
}
Vector2 Vector2::operator-(Vector2 v) {
	return Vector2(x-v.x, y-v.y);
}
Vector2 Vector2::operator*(float s) {
	return Vector2(x*s, y*s);
}
Vector2 Vector2::operator/(float s) {
	return Vector2(x/s, y/s);
}

void Vector2::operator+=(Vector2 v) {
	x += v.x;
	y += v.y;
}
void Vector2::operator-=(Vector2 v) {
	x -= v.x;
	y -= v.y;
}
void Vector2::operator*=(float s) {
	x *= s;
	y *= s;
}
void Vector2::operator/=(float s) {
	x /= s;
	y /= s;
}

float Vector2::length() {
	return sqrtf(x*x + y*y);
}

float Vector2::length_squared() {
	return x*x + y*y;
}

void Vector2::normalize() {
	float length = sqrtf(x*x + y*y);
	if (length == 0.0f) {
		x = 0.0f;
		y = 0.0f;
	} else {
		x /= length;
		y /= length;
	}
}
