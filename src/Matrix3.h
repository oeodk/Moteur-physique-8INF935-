#pragma once
#include "Vector3D.h"
#include "glm/mat3x3.hpp"
#include <array>

class Matrix3 {
private:
	std::array<std::array<float, 3>, 3> _mat;
public:
	

	Matrix3() : _mat({ std::array<float, 3>({1,0,0}),std::array<float, 3>({0,1,0}),std::array<float, 3>({0,0,1}) }) {};
	Matrix3(std::array<std::array<float, 3>, 3> mat) : _mat(mat) {};
	Matrix3(Vector3D v1, Vector3D v2, Vector3D v3) : _mat({
		std::array<float, 3>({ v1.x, v1.y, v1.z }),
		std::array<float, 3>({ v2.x, v2.y, v2.z }),
		std::array<float, 3>({ v3.x, v3.y, v3.z })
		}) {};

	float getCoef(int i, int j) const { return _mat[i][j]; };
	void setCoef(int i, int j, float value) { _mat[i][j] = value; }
	float getDeterminant() const;

	void setToIdentity();

	Matrix3 getCofactors() const;

	Matrix3 getInverse() const;
	Matrix3 getTranspose() const;
	const float* data() const { return &_mat[0][0]; }

	static void testMatrix3();
};

bool operator==(const Matrix3& m1, const Matrix3& m2);
bool operator!=(const Matrix3& m1, const Matrix3& m2);
Matrix3 operator*(const Matrix3& m1, float value);
Matrix3 operator*(float value, const Matrix3& m1) {
	return m1 * value;
}
Matrix3 operator*(const Matrix3& m1, const Matrix3& m2);
Vector3D operator*(const Matrix3& m1, const Vector3D& v2);
Matrix3 operator+(const Matrix3& m1, const Matrix3& m2);


namespace constants {
	inline static const Matrix3 IDENTITY_MATRIX3 = Matrix3();
}