#pragma once
#include "Matrix4.h"
#include "Matrix3.h"
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <ostream>

using namespace constants;

Matrix4 Matrix4::getTranspose() const {
	Matrix4 transpose;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			transpose._mat[i][j] = _mat[j][i];
		}
	}
	return transpose;
}

Matrix4 Matrix4::getCofactors() const {
	std::array<std::array<float, 4>,4 > cofactors;

	cofactors[0][0] = _mat[1][1] * (_mat[2][2] * _mat[3][3] - _mat[3][2] * _mat[2][3]) -
		_mat[2][1] * (_mat[1][2] * _mat[3][3] - _mat[3][2] * _mat[1][3]) +
		_mat[3][1] * (_mat[1][2] * _mat[2][3] - _mat[2][2] * _mat[1][3]);
	cofactors[0][1] =  - (_mat[1][0] * (_mat[2][2] * _mat[3][3] - _mat[3][2] * _mat[2][3]) -
		_mat[2][0] * (_mat[1][2] * _mat[3][3] - _mat[3][2] * _mat[1][3]) +
		_mat[3][0] * (_mat[1][2] * _mat[2][3] - _mat[2][2] * _mat[1][3]));
	cofactors[0][2] = _mat[1][0] * (_mat[2][1] * _mat[3][3] - _mat[3][1] * _mat[2][3]) -
		_mat[2][0] * (_mat[1][1] * _mat[3][3] - _mat[3][1] * _mat[1][3]) +
		_mat[3][0] * (_mat[1][1] * _mat[2][3] - _mat[2][1] * _mat[1][3]);
	cofactors[0][3] = - (_mat[1][0] * (_mat[2][1] * _mat[3][2] - _mat[3][1] * _mat[2][2]) -
		_mat[2][0] * (_mat[1][1] * _mat[3][2] - _mat[3][1] * _mat[1][2]) +
		_mat[3][0] * (_mat[1][1] * _mat[2][2] - _mat[2][1] * _mat[1][2]));
	cofactors[1][0] = - (_mat[0][1] * (_mat[2][2] * _mat[3][3] - _mat[3][2] * _mat[2][3]) -
		_mat[2][1] * (_mat[0][2] * _mat[3][3] - _mat[3][2] * _mat[0][3]) +
		_mat[3][1] * (_mat[0][2] * _mat[2][3] - _mat[2][2] * _mat[0][3]));
	cofactors[1][1] = _mat[0][0] * (_mat[2][2] * _mat[3][3] - _mat[3][2] * _mat[2][3]) -
		_mat[2][0] * (_mat[0][2] * _mat[3][3] - _mat[3][2] * _mat[0][3]) +
		_mat[3][0] * (_mat[0][2] * _mat[2][3] - _mat[2][2] * _mat[0][3]);
	cofactors[1][2] = - (_mat[0][0] * (_mat[2][1] * _mat[3][3] - _mat[3][1] * _mat[2][3]) -
		_mat[2][0] * (_mat[0][1] * _mat[3][3] - _mat[3][1] * _mat[0][3]) +
		_mat[3][0] * (_mat[0][1] * _mat[2][3] - _mat[2][1] * _mat[0][3]));
	cofactors[1][3] = _mat[0][0] * (_mat[2][1] * _mat[3][2] - _mat[3][1] * _mat[2][2]) -
		_mat[2][0] * (_mat[0][1] * _mat[3][2] - _mat[3][1] * _mat[0][2]) +
		_mat[3][0] * (_mat[0][1] * _mat[2][2] - _mat[2][1] * _mat[0][2]);
	cofactors[2][0] = _mat[0][1] * (_mat[1][2] * _mat[3][3] - _mat[3][2] * _mat[1][3]) -
		_mat[1][1] * (_mat[0][2] * _mat[3][3] - _mat[3][2] * _mat[0][3]) +
		_mat[3][1] * (_mat[0][2] * _mat[2][3] - _mat[2][2] * _mat[0][3]);
	cofactors[2][1] =  - (_mat[0][0] * (_mat[1][2] * _mat[3][3] - _mat[3][2] * _mat[1][3]) -
		_mat[1][0] * (_mat[0][2] * _mat[3][3] - _mat[3][2] * _mat[0][3]) +
		_mat[3][0] * (_mat[0][2] * _mat[1][3] - _mat[1][2] * _mat[0][3]));
	cofactors[2][2] = _mat[0][0] * (_mat[1][1] * _mat[3][3] - _mat[3][1] * _mat[1][3]) -
		_mat[1][0] * (_mat[0][1] * _mat[3][3] - _mat[3][1] * _mat[0][3]) +
		_mat[3][0] * (_mat[0][1] * _mat[2][3] - _mat[2][1] * _mat[0][3]);
	cofactors[2][3] = - (_mat[0][0] * (_mat[1][1] * _mat[3][2] - _mat[3][1] * _mat[1][2]) -
		_mat[1][0] * (_mat[0][1] * _mat[3][2] - _mat[3][1] * _mat[0][2]) +
		_mat[3][0] * (_mat[0][1] * _mat[1][2] - _mat[1][1] * _mat[0][2]));
	cofactors[3][0] = - (_mat[0][1] * (_mat[1][2] * _mat[2][3] - _mat[2][2] * _mat[1][3]) -
		_mat[1][1] * (_mat[0][2] * _mat[2][3] - _mat[2][2] * _mat[0][3]) +
		_mat[2][1] * (_mat[0][2] * _mat[1][3] - _mat[1][2] * _mat[0][3]));
	cofactors[3][1] = _mat[0][0] * (_mat[1][2] * _mat[2][3] - _mat[2][2] * _mat[1][3]) -
		_mat[1][0] * (_mat[0][2] * _mat[2][3] - _mat[2][2] * _mat[0][3]) +
		_mat[2][0] * (_mat[0][2] * _mat[1][3] - _mat[1][2] * _mat[0][3]);
	cofactors[3][2] = - (_mat[0][0] * (_mat[1][1] * _mat[2][3] - _mat[2][1] * _mat[1][3]) -
		_mat[1][0] * (_mat[0][1] * _mat[2][3] - _mat[2][1] * _mat[0][3]) +
		_mat[2][0] * (_mat[0][1] * _mat[1][3] - _mat[1][1] * _mat[0][3]));
	cofactors[3][3] = _mat[0][0] * (_mat[1][1] * _mat[2][2] - _mat[2][1] * _mat[1][2]) -
		_mat[1][0] * (_mat[0][1] * _mat[2][2] - _mat[2][1] * _mat[0][2]) +
		_mat[2][0] * (_mat[0][1] * _mat[1][2] - _mat[1][1] * _mat[0][2]);

	return Matrix4(cofactors);

}

Matrix4 Matrix4::getInverse() const {
	float det = getDeterminant();
	assert(("Determinant nul : Matrice non inversible", det != 0));
	Matrix4 adjMatrix = getTranspose().getCofactors();

	return 1 / det * adjMatrix;

}

float Matrix4::getDeterminant() const {
	Matrix4 cofactors = getCofactors();
	float det = _mat[0][0] * cofactors.getCoef(0, 0)
		+ _mat[1][0] * cofactors.getCoef(1, 0)
		+ _mat[2][0] * cofactors.getCoef(2, 0)
		+ _mat[3][0] * cofactors.getCoef(3, 0);
	return det;
}

bool operator==(const Matrix4& m1, const Matrix4& m2) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (m1.getCoef(i, j) != m2.getCoef(i, j)) {
				return false;
			}
		}
	}
	return true;
}

bool operator!=(const Matrix4& m1, const Matrix4& m2) {
	return !(m1 == m2);
}

Matrix4 operator*(const Matrix4& m1, float value) {
	Matrix4 product;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			product.setCoef(i, j, value * m1.getCoef(i, j));
		}
	}
	return product;
}

Matrix4 operator*(const Matrix4& m1, const Matrix4& m2) {
	Matrix4 product;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			float sum = 0;
			for (int k = 0; k < 3; k++) {
				sum += m1.getCoef(i, k) * m2.getCoef(k, j);
			}
			product.setCoef(i, j, sum);
		}
	}
	return product;
}

void Matrix4::testMatrix4()
{
	printf("-- Testing Matrix4 --\n");
	Matrix4 m1;
	std::array<std::array<float, 4>, 4> values{ {{1,2,3,4},{2,4,6,8},{3,6,9,12},{4,8,12,16}} };
	Matrix4 m2(values);
	std::array<std::array<float, 4>, 4> values2{ {{1,2,3,4},{0,1,2,3},{0,0,1,2},{0,0,0,1}} };
	std::array<std::array<float, 4>, 4> values3{ {{1,-2,1,0},{0,1,-2,1},{0,0,1,-2},{0,0,0,1}} };
	std::array<std::array<float, 4>, 4> values4{ {{1,4,10,20},{2,8,20,40},{3,12,30,60},{4,16,40,80}} };
	Matrix4 m3(values2);
	Matrix4 m4 = m2 * m3;
	

	assert(m1 == IDENTITY_MATRIX4);
	assert(m1 != m2);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			assert(m2.getCoef(i, j) == values[i][j]);
		}
	}
	assert(m1.getDeterminant() == 1);
	assert(m2.getDeterminant() == 0);
	assert(m3.getInverse() == Matrix4(values3));
	assert(m4 == Matrix4(values4));
	printf("-- TEST SUCCEEDED --\n");
}