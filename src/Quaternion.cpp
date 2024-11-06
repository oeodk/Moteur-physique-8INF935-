#include "Quaternion.h"
#include <cmath>
#include <stdexcept>

// Constructors
Quaternion::Quaternion(float w, float x, float y, float z) :
	w(w), x(x), y(y), z(z) {}

Quaternion::Quaternion(float w, Vector3D v) :
	w(w), x(v.x), y(v.y), z(v.z) {}


void Quaternion::set(float qw, float qx, float qy, float qz) {
	w = qw;
	x = qx;
	y = qy;
	z = qz;
}


// Operations
float Quaternion::squareNorm() const {
	return (w * w + x * x + y * y + z * z);
}

float Quaternion::getNorm() const {
	return std::sqrt(squareNorm());
}

Quaternion Quaternion::negate() const {
	return Quaternion(-w, -x, -y, -z);
}

Quaternion Quaternion::conjugate() const {
	return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::inverse() const {
	return conjugate() / getNorm();
}

Quaternion Quaternion::difference(const Quaternion& q1, const Quaternion& q2){
	return q2 * q1.conjugate();
}

float Quaternion::dotProduct(const Quaternion& q1, const Quaternion& q2) {
	return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

Quaternion Quaternion::pow(const Quaternion& q, float exponent) {
	const Vector3D v = Vector3D(q.x, q.y, q.z);
	const float alpha = acos(q.w);

	return Quaternion(cos(exponent * alpha), (sin(exponent * alpha) / sin(alpha)) * v);
}

Quaternion Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, float t) {
	return pow(q2 * q1.inverse(), t) * q1;
}


// Operators
void Quaternion::operator*=(float value) {
	w *= value;
	x *= value;
	y *= value;
	z *= value;
}

void Quaternion::operator/=(float value) {
	if (value == 0.0f)
		throw std::runtime_error("Error: division by 0");

	w /= value;
	x /= value;
	y /= value;
	z /= value;
}

void Quaternion::operator+=(const Quaternion& q) {
	w += q.w;
	x += q.x;
	y += q.y;
	z += q.z;
}

void Quaternion::operator-=(const Quaternion& q) {
	w -= q.w;
	x -= q.x;
	y -= q.y;
	z -= q.z;
}


Quaternion operator*(const Quaternion& q, float value) {
	return Quaternion(q.w * value, q.x * value, q.y * value, q.z * value);
}
Quaternion operator*(float value, const Quaternion& q) {
	return Quaternion(q.w * value, q.x * value, q.y * value, q.z * value);
}

// Hamilton product
Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
	const Vector3D v1 = Vector3D(q1.x, q1.y, q1.z);
	const Vector3D v2 = Vector3D(q2.x, q2.y, q2.z);

	return Quaternion(
		q1.w * q2.w,
		-Vector3D::dotProduct(v1, v2) * (q1.w * v2 + q2.w * Vector3D::crossProduct(v1, v2))
	);
}

Quaternion operator/(const Quaternion& q, float value) {
	return Quaternion(q.w / value, q.x / value, q.y / value, q.z / value);
}

Quaternion operator+(const Quaternion& q1, const Quaternion& q2) {
	return Quaternion(q1.w + q2.w, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z);
}

Quaternion operator-(const Quaternion& q1, const Quaternion& q2) {
	return Quaternion(q1.w - q2.w, q1.x - q2.x, q1.y - q2.y, q1.z - q2.z);
}

bool operator==(const Quaternion& q1, const Quaternion& q2) {
	return (q1.w == q2.w) && (q1.x == q2.x) && (q1.y == q2.y) && (q1.z == q2.z);
}

bool operator!=(const Quaternion& q1, const Quaternion& q2) {
	return !(q1 == q2);
}