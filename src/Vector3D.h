#pragma once
#include <glm/vec3.hpp>

struct Vector3D {
public:
	float x, y, z;

	Vector3D(float v = 0.f) : Vector3D(v, v, v) {}
	Vector3D(float px, float py, float pz);
	Vector3D(const glm::vec3& v) : Vector3D(v.x, v.y, v.z) {}
	Vector3D(const Vector3D&) = default;  // Default copy constructor

	void normalize();
	float getNorm() const;
	float squareNorm() const;
	void invert();
	const float* data() const { return &x; }
	void set(float vx, float vy, float vz);
	void set(const Vector3D& v) { set(v.x, v.y, v.z); };


	void operator*=(float value);
	void operator/=(float value);
	void operator+=(float value);
	void operator-=(float value);
	void operator+=(const Vector3D& v);
	void operator-=(const Vector3D& v);

	operator glm::vec3() const { return glm::vec3(x, y, z); }

	static Vector3D elementWiseProduct(const Vector3D& v1, const Vector3D v2);
	static Vector3D pow(const Vector3D& v, float exponent);
	static Vector3D crossProduct(const Vector3D& v1, const Vector3D& v2);
	static float dotProduct(const Vector3D& v1, const Vector3D& v2);
	static void testVector3D();

};

Vector3D operator*(const Vector3D& v1, float value);
Vector3D operator*(float value, const Vector3D& v1);
Vector3D operator/(const Vector3D& v1, float value);
Vector3D operator+(const Vector3D& v1, const Vector3D& v2);
Vector3D operator-(const Vector3D& v1, const Vector3D& v2);
bool operator==(const Vector3D& v1, const Vector3D& v2);
bool operator!=(const Vector3D& v1, const Vector3D& v2);



namespace constants {
	inline static const Vector3D EMPTY_VECTOR3D = Vector3D();
}
