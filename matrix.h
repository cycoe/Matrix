#ifndef __MATRIX_MATRIX_H__
#define __MATRIX_MATRIX_H__

#include <cstring>
#include <cstddef>
#include <array>
#include <algorithm>


template<typename T, size_t N>
class MatrixRow
{
public:
    MatrixRow(T* ptr) : m_ptr(ptr) { }
    MatrixRow(MatrixRow const&) = default;
    MatrixRow(MatrixRow&&) = default;

    T& operator[](size_t index) { return m_ptr[index]; }

protected:
    T* m_ptr;
};

/// TODO implement
template<typename T, size_t N>
class MatrixCol;

template<typename T, size_t NR, size_t NC, typename Enable =
typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Matrix
{
public:
    using BinaryOperator_t = std::function<T(T, T)>;

public:
    static Matrix zeros(void)
    {
        Matrix matrix;
        std::fill(matrix.m_data.begin(), matrix.m_data.end(), 0);
        return matrix;
    }

    static Matrix indentity(void)
    {
        /// Indentity matrix must be a square matrix
        static_assert(NR == NC);

        Matrix matrix = zeros();
        for (size_t r = 0; r < NR; ++r)
        {
            matrix.m_data[r * NR + r] = 1;
        }
        return matrix;
    }

public:
    Matrix(void) { }
    Matrix(std::initializer_list<T> const& il)
    {
        size_t copy_size = std::min(il.size(), m_data.size());
        std::copy(il.begin(), il.begin() + copy_size, m_data.begin());
    }
    Matrix(Matrix const& rhs) : m_data(rhs.m_data) { }
    Matrix(Matrix&& rhs) : m_data(std::move(rhs.m_data)) { }

    void swap(Matrix& rhs) { m_data.swap(rhs.m_data); }

    void swap_row(size_t a, size_t b)
    {
        T buf[NC];
        size_t cpy_size = sizeof(T) * NC;
        memcpy(buf, m_data.data() + a * NC, cpy_size);
        memcpy(m_data.data() + a * NC, m_data.data() + sizeof(T) * b, cpy_size);
        memcpy(m_data.data() + b * NC, buf, cpy_size);
    }

    Matrix& operator=(Matrix rhs)
    {
        swap(rhs);
        return *this;
    }

    /// Minus of Matrix
    Matrix operator-(void) const
    {
        Matrix matrix;
        std::transform(m_data.cbegin(), m_data.cend(), matrix.m_data.begin(), std::negate<T>());
        return matrix;
    }

    Matrix arithmetic_operation(Matrix const& rhs, BinaryOperator_t op) const
    {
        Matrix matrix;
        for (size_t index = 0; index < NR * NC; ++index)
        {
            matrix.m_data[index] = op(m_data[index], rhs.m_data[index]);
        }
        return matrix;
    }

    Matrix operator+(Matrix const& rhs) const
    {
        return arithmetic_operation(rhs, std::plus<T>());
    }

    Matrix operator-(Matrix const& rhs) const
    {
        return arithmetic_operation(rhs, std::minus<T>());
    }

    Matrix operator*(Matrix const& rhs) const
    {
        return arithmetic_operation(rhs, std::multiplies<T>());
    }

    Matrix operator/(Matrix const& rhs) const
    {
        return arithmetic_operation(rhs, std::divides<T>());
    }

    MatrixRow<T, NC> operator[](size_t index)
    {
        return MatrixRow<T, NC>(m_data.data() + index * NC);
    }

    template<size_t RHS_NC>
    Matrix<T, NR, RHS_NC> mul(Matrix<T, NC, RHS_NC> const& rhs) const
    {
        Matrix<T, NR, RHS_NC> matrix = zeros();
        for (size_t r = 0; r < NR; ++r)
        {
            for (size_t c = 0; c < RHS_NC; ++c)
            {
                for (size_t i = 0; i < NC; ++i)
                {
                    matrix.m_data[r * RHS_NC + c] += m_data[r * NC + i] * rhs.m_data[i * RHS_NC + c];
                }
            }
        }
        return matrix;
    }

    Matrix<T, NC, NR> transpose(void) const
    {
        Matrix<T, NC, NR> matrix;

        for (size_t r = 0; r < NR; ++r)
        {
            for (size_t c = 0; c < NC; ++c)
            {
                matrix.m_data[c * NR + r] = m_data[r * NC + c];
            }
        }

        return matrix;
    }

    Matrix inverse(void) const
    {
        static_assert(NR == NC);

        Matrix __origin(*this);
        Matrix __inverse(indentity());

        for (size_t r = 0; r < NR; ++r)
        {
            /// Find the next row that M[r, r] is not zero
            if (__origin.m_data[r * NC + r] == 0)
            {
                size_t nr = r + 1;
                for (; nr < NR; ++nr)
                {
                    if (__origin.m_data[nr * NC + r] != 0)
                    {
                        break;
                    }
                }

                if (nr == NR)
                {
                    return __inverse;
                }

                __origin.swap_row(r, nr);
            }

            T w = __origin.m_data[r * NC + r];
            for (size_t c = 0; c < NC; ++c)
            {
                __origin.m_data[r * NC + c] /= w;
                __inverse.m_data[r * NC + c] /= w;
            }

            for (size_t nr = r + 1; nr < NR; ++nr)
            {
                T w = __origin.m_data[nr * NC + r];
                for (size_t c = 0; c < NC; ++c)
                {
                    __origin.m_data[nr * NC + c] -= w * __origin.m_data[r * NC + c];
                    __inverse.m_data[nr * NC + c] -= w * __inverse.m_data[r * NC + c];
                }
            }
        }

        for (int r = NR - 1; r >= 0; --r)
        {
            for (int nr = r - 1; nr >= 0; --nr)
            {
                T w = __origin.m_data[nr * NC + r];
                for (size_t c = 0; c < NC; ++c)
                {
                    __inverse.m_data[nr * NC + c] -= w * __inverse.m_data[r * NC + c];
                }
            }
        }

        return __inverse;
    }

    /// For Debug
    friend std::ostream& operator<<(std::ostream& os, Matrix const& rhs)
    {
        for (size_t r = 0; r < NR; ++r)
        {
            for (size_t c = 0; c < NC; ++c)
            {
                os << rhs.m_data[r * NC + c] << " ";
            }
            os << std::endl;
        }

        return os;
    }

private:
    std::array<T, NR * NC> m_data;
};

template<typename T, size_t NR, size_t NC>
class Matrix<T, NR, NC, void>;

/// Some common Matrixs
using Matrix2i   = Matrix<int, 2, 2>;
using Matrix3i   = Matrix<int, 3, 3>;
using Matrix4i   = Matrix<int, 4, 4>;
using Matrix2l   = Matrix<long, 2, 2>;
using Matrix3l   = Matrix<long, 3, 3>;
using Matrix4l   = Matrix<long, 4, 4>;
using Matrix2ll  = Matrix<long long, 2, 2>;
using Matrix3ll  = Matrix<long long, 3, 3>;
using Matrix4ll  = Matrix<long long, 4, 4>;
using Matrix2u   = Matrix<unsigned int, 2, 2>;
using Matrix3u   = Matrix<unsigned int, 3, 3>;
using Matrix4u   = Matrix<unsigned int, 4, 4>;
using Matrix2ul  = Matrix<unsigned long, 2, 2>;
using Matrix3ul  = Matrix<unsigned long, 3, 3>;
using Matrix4ul  = Matrix<unsigned long, 4, 4>;
using Matrix2ull = Matrix<unsigned long long, 2, 2>;
using Matrix3ull = Matrix<unsigned long long, 3, 3>;
using Matrix4ull = Matrix<unsigned long long, 4, 4>;
using Matrix2f   = Matrix<float, 2, 2>;
using Matrix3f   = Matrix<float, 3, 3>;
using Matrix4f   = Matrix<float, 4, 4>;
using Matrix2d   = Matrix<double, 2, 2>;
using Matrix3d   = Matrix<double, 3, 3>;
using Matrix4d   = Matrix<double, 4, 4>;
using Matrix2ld  = Matrix<long double, 2, 2>;
using Matrix3ld  = Matrix<long double, 3, 3>;
using Matrix4ld  = Matrix<long double, 4, 4>;

#endif  // __MATRIX_MATRIX_H__