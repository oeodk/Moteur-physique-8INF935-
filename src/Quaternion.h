#pragma once
#include "Vector3D.h"
#include "Matrix4.h"
#include "glm/gtc/quaternion.hpp"

class Quaternion
{
public:
	float w, x, y, z;
	Quaternion(float w = 0, float x = 0, float y = 0, float z = 0);
	Quaternion(float w, Vector3D v);
	Quaternion(const Quaternion&) = default;

	void set(float qw, float qx, float qy, float qz);
	void set(float qw, const Vector3D& v) { set(qw, v.x, v.y, v.z); };

	float squareNorm() const;
	float getNorm() const;

	void normalize();

	Quaternion negate() const;
	Quaternion conjugate() const;
	Quaternion inverse() const;

	Matrix4 toMatrix() const;

	static Quaternion difference(const Quaternion& q1, const Quaternion& q2);
	static float dotProduct(const Quaternion& q1, const Quaternion& q2);
	static Quaternion pow(const Quaternion& q, float exponent);
	static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);

	void operator*=(float value);
	void operator/=(float value);
	void operator+=(const Quaternion& q);
	void operator-=(const Quaternion& q);

	operator glm::quat() const { return glm::quat(w, x, y, z); }
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
	inline static const Quaternion IDENTITY_POSITIVE = Quaternion(1, 0, 0, 0);
	inline static const Quaternion IDENTITY_NEGATIVE = Quaternion(-1, 0, 0, 0);
}
