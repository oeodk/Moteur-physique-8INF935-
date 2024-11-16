#pragma once
#include "../Quaternion.h"

class TestQuaternion
{
public:
	static void launchTests();

private:
	static bool assertFloatEquals(float realValue, float expectedValue);
	static void testAssertFloat();

	static void testStandardConstructor();
	static void testVectorConstructor();
	static void testCopyConstructor();
	
	static void testStandardSet();
	static void testVectorSet();
	
	static void testSquareNorm();
	static void testGetNorm();
	
	static void testNegate();
	static void testConjugate();
	static void testInverse();
	
	static void testDifference();
	static void testDotProduct();
	static void testPower();
	static void testSlerp();
	
	static void testValueProductOperator();
	static void testQuaternionProductOperator();
	static void testDivisionOperator();
	static void testSumOperator();
	static void testSubtractOperator();
	
	static void testInlineProductOperator();
	static void testInlineDivisionOperator();
	static void testInlineSumOperator();
	static void testInlineSubtractOperator();
	
	static void testEquality();
	static void testInequality();
};

