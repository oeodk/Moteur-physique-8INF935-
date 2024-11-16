#include "TestQuaternion.h"
#include <stdexcept>
#include <stdio.h>
#include <math.h>

bool TestQuaternion::assertFloatEquals(float realValue, float expectedValue) {
	return abs(realValue - expectedValue) < 0.01;
}

void TestQuaternion::testAssertFloat() {
	float f = 3.0;
	f /= 2;
	_ASSERT(assertFloatEquals(f, 1.5));
	_ASSERT(!assertFloatEquals(f, 1.48));
	_ASSERT(!assertFloatEquals(f, 1.52));
}

void TestQuaternion::launchTests() {
	testAssertFloat();

	testStandardConstructor();
	testVectorConstructor();
	testCopyConstructor();

	testStandardSet();
	testVectorSet();

	testSquareNorm();
	testGetNorm();

	testValueProductOperator();
	testDivisionOperator();
	testSumOperator();
	testSubtractOperator();

	testInlineProductOperator();
	testInlineDivisionOperator();
	testInlineSumOperator();
	testInlineSubtractOperator();

	testEquality();
	testInequality();

	testNegate();
	testConjugate();
	testInverse();

	testDifference();
	testDotProduct();
	testPower();
	testSlerp();

	testQuaternionProductOperator();
}

void TestQuaternion::testStandardConstructor() {
	Quaternion q = Quaternion(1, 2, 3, 4);
	_ASSERT(q.w == 1 && q.x == 2 && q.y == 3 && q.z == 4);
}

void TestQuaternion::testVectorConstructor() {
	Vector3D v = Vector3D(15, 13, 20);
	Quaternion q = Quaternion(8, v);
	_ASSERT(q.w == 8 && q.x == 15 && q.y == 13 && q.z == 20);
}

void TestQuaternion::testCopyConstructor() {
	Quaternion q1 = Quaternion(5, 10, 15, 20);
	Quaternion q2 = Quaternion(q1);
	_ASSERT(q2.w == 5 && q2.x == 10 && q2.y == 15 && q2.z == 20);
}

void TestQuaternion::testStandardSet() {
	Quaternion q = Quaternion(1, 2, 3, 4);
	q.set(6, 7, 8, 9);
	_ASSERT(q.w == 6 && q.x == 7 && q.y == 8 && q.z == 9);
}

void TestQuaternion::testVectorSet() {
	Quaternion q = Quaternion(1, 2, 3, 4);
	Vector3D v = Vector3D(12, 11, 24);
	q.set(q.x, v);
	_ASSERT(q.w == 2 && q.x == 12 && q.y == 11 && q.z == 24);
}

void TestQuaternion::testSquareNorm() {
	Quaternion q = Quaternion(1, 2, 3, 4);
	_ASSERT(q.squareNorm() == 30.);
}

void TestQuaternion::testGetNorm() {
	Quaternion q = Quaternion(0, 3, 0, 4);
	_ASSERT(q.getNorm() == 5.);
}

void TestQuaternion::testNegate() {
	Quaternion q1 = Quaternion(3.5, -7.2, -2, 5.1);
	Quaternion q2 = q1.negate();
	_ASSERT(
		assertFloatEquals(q2.w, -3.5)
		&& assertFloatEquals(q2.x, 7.2)
		&& assertFloatEquals(q2.y, 2)
		&& assertFloatEquals(q2.z, -5.1)
	);
}

void TestQuaternion::testConjugate() {
	Quaternion q1 = Quaternion(3.5, -7.2, -2, 5.1);
	Quaternion q2 = q1.conjugate();
	_ASSERT(
		assertFloatEquals(q2.w, 3.5)
		&& assertFloatEquals(q2.x, 7.2)
		&& assertFloatEquals(q2.y, 2)
		&& assertFloatEquals(q2.z, -5.1)
	);
}

void TestQuaternion::testInverse() {
	Quaternion q1 = Quaternion(0, 3, 0, 4);
	Quaternion q2 = q1.inverse();
	_ASSERT(
		assertFloatEquals(q2.w, 0)
		&& assertFloatEquals(q2.x, -0.6)
		&& assertFloatEquals(q2.y, 0)
		&& assertFloatEquals(q2.z, -0.8)
	);
}

void TestQuaternion::testDifference() {
	// TODO
}

void TestQuaternion::testDotProduct() {
	// TODO
}

void TestQuaternion::testPower() {
	// TODO
}

void TestQuaternion::testSlerp() {
	// TODO
}

void TestQuaternion::testValueProductOperator() {
	Quaternion q = Quaternion(1, 2, 3, 4);
	
	Quaternion q2 = q * 5;
	_ASSERT(q2.w == 5 && q2.x == 10	&& q2.y == 15 && q2.z == 20);

	Quaternion q3 = 3 * q;
	_ASSERT(q3.w == 3 && q3.x == 6 && q3.y == 9 && q3.z == 12);
}

void TestQuaternion::testQuaternionProductOperator() {
	Quaternion q1 = Quaternion(1, 2, 3, 4);
	Quaternion q2 = Quaternion(6, 7, 8, 9);

	Quaternion q3 = q1 * q2;
	// TODO
	//_ASSERT(q.w == 3 && q.x == 6 && q.y == 9 && q.z == 12);
}

void TestQuaternion::testDivisionOperator() {
	Quaternion q1 = Quaternion(1, 3, 7, 9);
	Quaternion q2 = q1 / 5;
	_ASSERT(
		assertFloatEquals(q2.w, 0.2)
		&& assertFloatEquals(q2.x, 0.6)
		&& assertFloatEquals(q2.y, 1.4)
		&& assertFloatEquals(q2.z, 1.8)
	);
}

void TestQuaternion::testSumOperator() {
	Quaternion q1 = Quaternion(1, 2, 4, 5);
	Quaternion q2 = Quaternion(10, -4, 3, 7);
	Quaternion q3 = q1 + q2;
	_ASSERT(q3.w == 11 && q3.x == -2 && q3.y == 7 && q3.z == 12);
}

void TestQuaternion::testSubtractOperator() {
	Quaternion q1 = Quaternion(1, 2, 4, 5);
	Quaternion q2 = Quaternion(10, -4, 3, 7);
	Quaternion q3 = q1 - q2;
	_ASSERT(q3.w == -9 && q3.x == 6 && q3.y == 1 && q3.z == -2);
}

void TestQuaternion::testInlineProductOperator() {
	Quaternion q = Quaternion(1, 2, 3, 4);
	q *= 4.2;
	_ASSERT(
		assertFloatEquals(q.w, 4.2)
		&& assertFloatEquals(q.x, 8.4)
		&& assertFloatEquals(q.y, 12.6)
		&& assertFloatEquals(q.z, 16.8)
	);
}

void TestQuaternion::testInlineDivisionOperator() {
	Quaternion q = Quaternion(3, 9, 9, 12);
	q /= 6;
	_ASSERT(
		assertFloatEquals(q.w, 0.5)
		&& assertFloatEquals(q.x, 1.5)
		&& assertFloatEquals(q.y, 1.5)
		&& assertFloatEquals(q.z, 2)
	);
}

void TestQuaternion::testInlineSumOperator() {
	Quaternion q = Quaternion(1, 2, 0, 0);
	q += Quaternion(3, 0, 0, 7);
	_ASSERT(q.w == 4 && q.x == 2 && q.y == 0 && q.z == 7);
}

void TestQuaternion::testInlineSubtractOperator() {
	Quaternion q = Quaternion(1, 2, 0, 0);
	q -= Quaternion(3, 0, 0, 7);
	_ASSERT(q.w == -2 && q.x == 2 && q.y == 0 && q.z == -7);
}

void TestQuaternion::testEquality() {
	Vector3D v = Vector3D(2, 3, 4);
	Quaternion q1 = Quaternion(1, 2, 3, 4);
	Quaternion q2 = Quaternion(1, v);
	Quaternion q3 = Quaternion(q2);

	_ASSERT(q1 == q2 && q2 == q3 && q3 == q1);
}

void TestQuaternion::testInequality() {
	Quaternion q1 = Quaternion(5, 7, -4, 0);
	Quaternion q2 = Quaternion(5, 7.01, -4, 0);

	_ASSERT(q1 != q2 && q2 != q1);
}