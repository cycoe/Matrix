#include <iostream>
#include "matrix.h"
#include "vector.h"


void test_create_matrix(void)
{
    Matrix<int, 3, 4> matrix_empty;
    Matrix<int, 3, 2> matrix_zeros = Matrix<int, 3, 2>::zeros();
    Matrix<int, 3, 3> matrix_indentity = Matrix<int, 3, 3>::indentity();
    Matrix<float, 3, 3> matrix_from_il({1, 2, 3, 4, 5, 6, 7, 8, 9});
    Matrix<float, 3, 3> matrix_from_matrix(matrix_from_il);

    std::cout << "matrix_empty" << std::endl;
    std::cout << matrix_empty << std::endl;

    std::cout << "matrix_zeros" << std::endl;
    std::cout << matrix_zeros << std::endl;

    std::cout << "matrix_from_li" << std::endl;
    std::cout << matrix_from_il << std::endl;

    std::cout << "matrix_from_matrix" << std::endl;
    std::cout << matrix_from_matrix << std::endl;
}

void test_assignment(void)
{
    Matrix3f matrix1({1, 2, 3, 4, 5, 6, 7, 8, 9});
    Matrix3f matrix2;

    matrix2 = matrix1;
    std::cout << "assignment from right value" << std::endl;
    std::cout << matrix2 << std::endl;

    matrix2 = Matrix3f::indentity();
    std::cout << "assignment from left value" << std::endl;
    std::cout << matrix2 << std::endl;
}

void test_access(void)
{
    Matrix2f matrix({1, 2, 3, 4});
    std::cout << "matrix[1][1] is " << matrix[1][1] << std::endl;
    matrix[1][1] = 5;
    std::cout << matrix << std::endl;
}

void test_arithmetic_operation(void)
{
    Matrix<float, 2, 2> matrix1({1, 2, 3, 4});
    Matrix<float, 2, 2> matrix2({4, 3, 2, 1});

    std::cout << "operator-()" << std::endl;
    std::cout << -matrix1 << std::endl;

    std::cout << "operator+(Matrix)" << std::endl;
    std::cout << matrix1 + matrix2<< std::endl;

    std::cout << "operator-(Matrix)" << std::endl;
    std::cout << matrix1 - matrix2<< std::endl;

    std::cout << "operator*(Matrix)" << std::endl;
    std::cout << matrix1 * matrix2<< std::endl;

    std::cout << "operator/(Matrix)" << std::endl;
    std::cout << matrix1 / matrix2<< std::endl;
}

void test_transform(void)
{
    // 1 2 mul 4 3
    // 3 4     2 1
    Matrix2f matrix1({1, 2, 3, 4});
    Matrix2f matrix2({4, 3, 2, 1});

    std::cout << "multiply" << std::endl;
    std::cout << matrix1.mul(matrix2) << std::endl;

    std::cout << "inverse" << std::endl;
    std::cout << matrix1.inverse() << std::endl;

    std::cout << "transpose" << std::endl;
    std::cout << matrix1.transpose() << std::endl;
}

int main(void)
{
    test_create_matrix();
    test_assignment();
    test_access();
    test_arithmetic_operation();
    test_transform();
}