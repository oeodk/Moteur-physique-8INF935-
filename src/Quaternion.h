#pragma once
#include "Vector3D.h"
#include <glm/mat4x4.hpp>

class Quaternion
{
public:
	float w, x, y, z;
	Quaternion(float w, float x, float y, float z);
	Quaternion(float w, Vector3D v);
	Quaternion(const Quaternion&) = default;

	void set(float qw, float qx, float qy, float qz);
	void set(float qw, const Vector3D& v) { set(qw, v.x, v.y, v.z); };

	float squareNorm() const;
	float getNorm() const;

	Quaternion negate() const;
	Quaternion conjugate() const;
	Quaternion inverse() const;

	static Quaternion difference(const Quaternion& q1, const Quaternion& q2);
	static float dotProduct(const Quaternion& q1, const Quaternion& q2);
	static Quaternion pow(const Quaternion& q, float exponent);
	static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);

	void operator*=(float value);
	void operator/=(float value);
	void operator+=(const Quaternion& q);
	void operator-=(const Quaternion& q);
};

Quaternion operator*(const Quaternion& q, float value);
Quaternion operator*(float value, const Quaternion& q);
Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
Quaternion operator/(const Quaternion& q, float value);
Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
bool operator==(const Quaternion& q1, const Quaternion& q2);
bool operator!=(const Quaternion& q1, const Quaternion& q2);

namespace constants {
	static const Quaternion IDENTITY_POSITIVE = Quaternion(1, 0, 0, 0);
	static const Quaternion IDENTITY_NEGATIVE = Quaternion(-1, 0, 0, 0);
}
