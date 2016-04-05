#include "gtest/gtest.h"
#include <drill/common/matrix.h>

#include <vector>

using namespace drill;

TEST(Matrix, Simple) {
    common::Matrix<double> matrix1(4);

    matrix1(0, 0) = 1;
    matrix1(1, 1) = 1;
    matrix1(2, 2) = 1;
    matrix1(3, 3) = 1;

    common::Matrix<double> matrix2 = matrix1;
    matrix2 = matrix1 + matrix2;

    matrix1 *= 2;

    ASSERT_EQ(matrix2, matrix1);
}

struct DigitInit {
	void operator()(double &t) {
		t = 0;
	}
};

TEST(Matrix, mul) {
    common::Matrix<double> matrix1(2);

    matrix1(0, 0) = 1;
    matrix1(1, 1) = 2;
    matrix1(1, 0) = 3;
    matrix1(0, 1) = 4;

    common::Matrix<double> matrix2 = drill::common::matrixMultiple(matrix1, matrix1, DigitInit());
    common::Matrix<double> matrix3(2);

	matrix3(0,0) = 13;
	matrix3(0,1) = 12;
	matrix3(1,0) = 9;
	matrix3(1,1) =16;
    ASSERT_EQ(matrix2, matrix3);
}

TEST(Matrix, mul1) {
    common::Matrix<double> matrix1(2, 3, 0);

    matrix1(0, 0) = 1;
    matrix1(0, 1) = 2;
    matrix1(0, 2) = 3;
    matrix1(1, 0) = 4;
	matrix1(1, 1) = 5;
	matrix1(1, 2) = 6;

    common::Matrix<double> matrix2(3, 2, 0);
	matrix2(0, 0) = 1;
	matrix2(1, 0) = 2;
	matrix2(2, 0) = 3;
	matrix2(0, 1) = 4;
	matrix2(1, 1) = 5;
	matrix2(2, 1) = 6;

	common::Matrix<double> matrix3 = drill::common::matrixMultiple(matrix1, matrix2, DigitInit());
    common::Matrix<double> matrix4(2);

	matrix4(0,0) = 14;
	matrix4(0,1) = 32;
	matrix4(1,0) = 32;
	matrix4(1,1) = 77;
    ASSERT_EQ(matrix4, matrix3);
}

