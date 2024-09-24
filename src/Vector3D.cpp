#include "Vector3D.h"
#include <cmath>
#include <stdexcept>

Vector3D::Vector3D(float px, float py, float pz)
	: x(px), y(py), z(pz) {

}

void Vector3D::normalize() {
	if (getNorm() == 0.0f) {
		throw std::runtime_error("Norm = 0");
	}
	set(*this / getNorm());
}

float Vector3D::getNorm() const {
	return std::sqrt(squareNorm());
}

float Vector3D::squareNorm() const {
	return (x * x + y * y + z * z);
}

void Vector3D::invert() {
	x = -x;
	y = -y;
	z = -z;
}


void Vector3D::set(float vx, float vy, float vz) {
	x = vx;
	y = vy;
	z = vz;
}

void Vector3D::operator*=(float value) {
	x *= value;
	y *= value;
	z *= value;
}

void Vector3D::operator/=(float value) {
	if (value == 0.0f) {
		throw std::runtime_error("Division by 0");
	}
	x /= value;
	y /= value;
	z /= value;
}

void Vector3D::operator+=(float value) {
	x += value;
	y += value;
	z += value;
}

void Vector3D::operator-=(float value) {
	x -= value;
	y -= value;
	z -= value;
}

void Vector3D::operator+=(const Vector3D& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void Vector3D::operator-=(const Vector3D& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

Vector3D Vector3D::elementWiseProduct(const Vector3D& v1, const Vector3D v2) {
	return Vector3D(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

Vector3D Vector3D::pow(const Vector3D& v, float exponent) {
	return Vector3D(std::pow(v.x, exponent), std::pow(v.y, exponent), std::pow(v.z, exponent));
}

Vector3D  Vector3D::crossProduct(const Vector3D& v1, const Vector3D& v2) {
	return Vector3D(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

float  Vector3D::dotProduct(const Vector3D& v1, const Vector3D& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3D operator+(const Vector3D& v1, const Vector3D& v2) {
	return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3D operator-(const Vector3D& v1, const Vector3D& v2) {
	return Vector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

bool operator==(const Vector3D& v1, const Vector3D& v2) {
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

bool operator!=(const Vector3D& v1, const Vector3D& v2) {
	return !(v1 == v2);
}

Vector3D operator*(const Vector3D& v1, float value) {
	return Vector3D(v1.x * value, v1.y * value, v1.z * value);
}

Vector3D operator/(const Vector3D& v1, float value) {
	if (value == 0.0f) {
		throw std::runtime_error("Division by 0");
	}
	return Vector3D(v1.x / value, v1.y / value, v1.z / value);

}

Vector3D operator*(float value, const Vector3D& v1) {
	return Vector3D(v1.x * value, v1.y * value, v1.z * value);
}


void Vector3D::testVector3D() {
	Vector3D vec1, vec2(1, 2, 3), vec3(-1, -2, -3);
	_ASSERT(vec1.x == 0 && vec1.y == 0 && vec1.z == 0);
	_ASSERT(vec2.x == 1 && vec2.y == 2 && vec2.z == 3);
	_ASSERT(vec3.x == -1 && vec3.y == -2 && vec3.z == -3);

	_ASSERT(vec2 == Vector3D(1, 2, 3));
	_ASSERT(vec1 != vec2);

	_ASSERT(vec2 + vec1 == vec2);
	_ASSERT(vec2 + Vector3D(1, 1, 1) == Vector3D(2, 3, 4));
	_ASSERT(vec2 - vec1 == vec2);
	_ASSERT(vec2 - Vector3D(1, 1, 1) == Vector3D(0, 1, 2));

	_ASSERT(vec2 * 2 == Vector3D(2, 4, 6));
	_ASSERT(vec2 / 2 == Vector3D(0.5f, 1, 1.5f));
	try {
		vec2 / 0;
		_ASSERT(false);
	}
	catch (const std::runtime_error& e) {
		_ASSERT(std::string(e.what()) == "Division by 0");
	}

	Vector3D vec4(3, 0, 0);
	vec4.normalize();
	_ASSERT(vec4 == Vector3D(1, 0, 0));
	Vector3D vec5(0, 0, 0);
	try {
		vec5.normalize();
		_ASSERT(false);
	}
	catch (const std::runtime_error& e) {
		_ASSERT(std::string(e.what()) == "Norm = 0");
	}

	vec2.invert();
	_ASSERT(vec2 == Vector3D(-1, -2, -3));

	vec1 += 1;
	_ASSERT(vec1 == Vector3D(1, 1, 1));
	vec1 -= 1;
	_ASSERT(vec1 == Vector3D(0, 0, 0));
	vec1 += vec2;
	_ASSERT(vec1 == Vector3D(-1, -2, -3));
	vec1 -= vec2;
	_ASSERT(vec1 == Vector3D(0, 0, 0));

	_ASSERT(Vector3D::elementWiseProduct(Vector3D(1, 2, 3), Vector3D(4, 5, 6)) == Vector3D(4, 10, 18));
	_ASSERT(Vector3D::pow(Vector3D(1, 2, 3), 2) == Vector3D(1, 4, 9));

	Vector3D cross = Vector3D::crossProduct(Vector3D(1, 0, 0), Vector3D(0, 1, 0));
	_ASSERT(cross == Vector3D(0, 0, 1));
	float dot = Vector3D::dotProduct(Vector3D(1, 2, 3), Vector3D(4, 5, 6));
	_ASSERT(dot == 32);

	_ASSERT(vec2.getNorm() == std::sqrtf(14));
	_ASSERT(vec2.squareNorm() == 14);

	vec1.set(5, 6, 7);
	_ASSERT(vec1 == Vector3D(5, 6, 7));

	vec1 *= 2;
	_ASSERT(vec1 == Vector3D(10, 12, 14));
	try {
		vec1 /= 0;
		_ASSERT(false);
	}
	catch (const std::runtime_error& e) {
		_ASSERT(std::string(e.what()) == "Division by 0");
	}

	Vector3D vec6 = Vector3D::pow(vec1, 2);
	_ASSERT(vec6 == Vector3D(100, 144, 196));
}
