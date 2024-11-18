#pragma once
#include "glm/mat4x4.hpp"
#include <array>

class Matrix4 {
private:
	std::array<std::array<float, 4>, 4> _mat;
public:


	Matrix4() : _mat({ std::array<float, 4>({1,0,0,0}),std::array<float, 4>({0,1,0,0}),
		std::array<float, 4>({0,0,1,0}),std::array<float,4>({0,0,0,1})}) {};
	Matrix4(std::array<std::array<float, 4>, 4> mat) : _mat(mat) {};

	float getCoef(int i, int j) const { return _mat[i][j]; };
	void setCoef(int i, int j, float value) { _mat[i][j] = value; }
	float getDeterminant() const;

	Matrix4 getCofactors() const;

	Matrix4 getInverse() const;
	Matrix4 getTranspose() const;
	const float* data() const { return &_mat[0][0]; }

	static void testMatrix4();
};

bool operator==(const Matrix4& m1, const Matrix4& m2);
bool operator!=(const Matrix4& m1, const Matrix4& m2);
Matrix4 operator*(const Matrix4& m1, float value);
Matrix4 operator*(float value, const Matrix4& m1) {
	return m1 * value;
}
Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
Matrix4 operator+(const Matrix4& m1, const Matrix4& m2);


namespace constants {
	inline static const Matrix4 IDENTITY_MATRIX4 = Matrix4();
}
