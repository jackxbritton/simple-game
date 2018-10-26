#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

struct Vector2 {

    float x, y;

	Vector2();
	Vector2(float x, float y);

	Vector2 operator+(Vector2 v);
	Vector2 operator-(Vector2 v);
	Vector2 operator*(float s);
	Vector2 operator/(float s);
	void operator+=(Vector2 v);
	void operator-=(Vector2 v);
	void operator*=(float s);
	void operator/=(float s);

	float length();
	float length_squared();
	void normalize();

};


#endif
