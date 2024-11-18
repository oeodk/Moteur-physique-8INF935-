#pragma once
#include "Matrix3.h"
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <ostream>

using namespace constants;

Matrix3 Matrix3::getTranspose() const {
	Matrix3 transpose;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			transpose._mat[i][j] = _mat[j][i];
		}
	}
	return transpose;
}

Matrix3 Matrix3::getCofactors() const {
	float det1,det2,det3,det4,det5,det6,det7,det8,det9;

	det1 = _mat[1][1] * _mat[2][2] - _mat[2][1] * _mat[1][2];
	det2 = _mat[1][0] * _mat[2][2] - _mat[2][0] * _mat[1][2];
	det3 = _mat[1][0] * _mat[2][1] - _mat[2][0] * _mat[1][1];
	det4 = _mat[0][1] * _mat[2][2] - _mat[2][1] * _mat[0][2];
	det5 = _mat[0][0] * _mat[2][2] - _mat[2][0] * _mat[0][2];
	det6 = _mat[0][0] * _mat[2][1] - _mat[2][0] * _mat[0][1];
	det7 = _mat[0][1] * _mat[1][2] - _mat[1][1] * _mat[0][2];
	det8 = _mat[0][0] * _mat[1][2] - _mat[1][0] * _mat[0][2];
	det9 = _mat[0][0] * _mat[1][1] - _mat[1][0] * _mat[0][1];

	std::array<std::array<float, 3>, 3> cofactors{ { {det1,-det2,det3},{-det4,det5,-+det6},{det7,-det8,det9} } };

	return Matrix3(cofactors);
	
}

Matrix3 Matrix3::getInverse() const{
	float det = getDeterminant();
	assert(("Determinant nul : Matrice non inversible",det != 0));
	Matrix3 adjMatrix = getTranspose().getCofactors();

	return 1/det * adjMatrix;
	
}

float Matrix3::getDeterminant() const {
	Matrix3 cofactors = getCofactors();
	float det = _mat[0][0] * (cofactors.getCoef(0,0))
		+ _mat[1][0] * (cofactors.getCoef(1,0))
		+ _mat[2][0] * (cofactors.getCoef(2,0));
	return det;
}

void Matrix3::setToIdentity()
{
	*this = Matrix3();
}

bool operator==(const Matrix3& m1, const Matrix3& m2) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (m1.getCoef(i,j) != m2.getCoef(i,j)) {
				return false;
			}
		}
	}
	return true;
}

bool operator!=(const Matrix3& m1, const Matrix3& m2) {
	return !(m1 == m2);
}

Matrix3 operator*(const Matrix3& m1, float value) {
	Matrix3 product;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			product.setCoef(i, j, value * m1.getCoef(i, j));
		}
	}
	return product;
}

Matrix3 operator*(const Matrix3& m1, const Matrix3& m2) {
	Matrix3 product;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			float sum = 0;
			for (int k = 0; k < 3; k++) {
				sum += m1.getCoef(i, k) * m2.getCoef(k, j);
			}
			product.setCoef(i, j,sum);
		}
	}
	return product;
}
Vector3D operator*(const Matrix3& m1, const Vector3D& v2) {
	Vector3D product;
	product.x = m1.getCoef(0, 0) * v2.x + m1.getCoef(0, 1) * v2.y + m1.getCoef(0, 2) * v2.z;
	product.y = m1.getCoef(1, 0) * v2.x + m1.getCoef(1, 1) * v2.y + m1.getCoef(2, 2) * v2.z;
	product.z = m1.getCoef(2, 0) * v2.x + m1.getCoef(2, 1) * v2.y + m1.getCoef(2, 2) * v2.z;

	return product;
}

void Matrix3::testMatrix3() {
	printf("-- TESTING MATRIX3 --\n");
	Matrix3 m1;
	std::array<std::array<float, 3>, 3> values{ {{1,2,3},{4,5,6},{7,8,9}} };
	Matrix3 m2(values);
	std::array<std::array<float, 3>, 3> values2{ {{5,5,5},{6,8,7},{1,2,3}} };
	Matrix3 m3(values2);
	Matrix3 m4 = m2 * m3;
	std::array<std::array<float, 3>, 3> values3{ {{20,27,28},{56,72,73},{92,117,118}} };
	std::array<std::array<float, 3>, 3> values4{ {{1,2,3},{0,1,4},{0,0,1}} };
	std::array<std::array<float, 3>, 3> values5{ {{1,-2,5},{0,1,-4},{0,0,1}} };
	Matrix3 m5(values4);

	Vector3D v(1, 2, 3);
	
	assert(m1 == IDENTITY_MATRIX3);

	Vector3D v2 = IDENTITY_MATRIX3 * v;

	//assert(v == v2);

	assert(m1 != m2);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			assert(m2.getCoef(i, j) == values[i][j]);
		}
	}
	assert(m1.getDeterminant() == 1);
	assert(m2.getDeterminant() == 0);
	assert(m3.getDeterminant() == 15);
	assert(m4 == Matrix3(values3));
	assert(m5.getInverse() == Matrix3(values5));
	printf("-- TEST SUCCEEDED -- \n");
}