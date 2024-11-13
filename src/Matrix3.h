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

	float getCoef(int i, int j) const { return _mat[i][j]; };
	void setCoef(int i, int j, float value) { _mat[i][j] = value; }
	float getDeterminant() const;

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


namespace constants {
	inline static const Matrix3 IDENTITY_MATRIX3 = Matrix3();
}