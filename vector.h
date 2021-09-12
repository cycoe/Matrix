#ifndef __MATRIX_VECTOR_H__
#define __MATRIX_VECTOR_H__

#include <cmath>
#include <cstring>
#include <cstddef>
#include <array>
#include <algorithm>


template<typename T, size_t N, typename Enable =
typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Vector
{
public:
    using BinaryOperator_t = std::function<T(T, T)>;

public:
    static Vector zeros(void)
    {
        Vector vector;
        std::fill(vector.m_data.begin(), vector.m_data.end(), 0);
        return vector;
    }

public:
    Vector(void) { }
    Vector(std::initializer_list<T> const& il)
    {
        size_t copy_size = std::min(il.size(), m_data.size());
        std::copy(il.begin(), il.begin() + copy_size, m_data.begin());
    }
    Vector(Vector const& rhs) : m_data(rhs.m_data) { }
    Vector(Vector&& rhs) : m_data(std::move(rhs.m_data)) { }

    void swap(Vector& rhs) { m_data.swap(rhs.m_data); }

    Vector& operator=(Vector rhs)
    {
        swap(rhs);
        return *this;
    }

    /// Minus of Matrix
    Vector operator-(void) const
    {
        Vector vector;
        std::transform(m_data.cbegin(), m_data.cend(), vector.m_data.begin(), std::negate<T>());
        return vector;
    }

    Vector arithmetic_operation(Vector const& rhs, BinaryOperator_t op) const
    {
        Vector vector;
        for (size_t index = 0; index < N; ++index)
        {
            vector.m_data[index] = op(m_data[index], rhs.m_data[index]);
        }
        return vector;
    }

    Vector operator+(Vector const& rhs) const
    {
        return arithmetic_operation(rhs, std::plus<T>());
    }

    Vector operator-(Vector const& rhs) const
    {
        return arithmetic_operation(rhs, std::minus<T>());
    }

    Vector operator*(Vector const& rhs) const
    {
        return arithmetic_operation(rhs, std::multiplies<T>());
    }

    Vector operator/(Vector const& rhs) const
    {
        return arithmetic_operation(rhs, std::divides<T>());
    }

    T dot(Vector const& rhs) const
    {
        T value = 0;
        for (size_t index = 0; index < N; ++index)
        {
            value += m_data[index] * rhs.m_date[index];
        }

        return value;
    }

    Vector cross(Vector const& rhs) const
    {
        /// TODO cross is only work on vector<3>
        static_assert(N == 3);

        return Vector<T, N>({
            m_data[1] * rhs.m_data[2] - m_data[2] * rhs.m_data[1],
            m_data[2] * rhs.m_data[0] - m_data[0] * rhs.m_data[2],
            m_data[0] * rhs.m_data[1] - m_data[1] * rhs.m_data[0]
        });
    }

    T norm(void) const
    {
        T value = 0;
        for (size_t index = 0; index < N; ++index)
        {
            value += m_data[index] * m_data[index];
        }

        return std::sqrt(value);
    }

    T normalized(void) const
    {
        return (*this) / norm();
    }

    template<size_t NN>
    Vector<T, NN> head(void) const
    {
        Vector<T, NN> vector;
        std::copy(m_data.cbegin(), m_data.cbegin() + std::min(N, NN), vector.m_data.begin());
        return vector;
    }

    /// For Debug
    friend std::ostream& operator<<(std::ostream& os, Vector const& rhs)
    {
        for (size_t index = 0; index < N; ++index)
        {
            os << rhs.m_data[index] << " ";
        }

        return os;
    }

private:
    std::array<T, N> m_data;
};

template<typename T, size_t N>
class Vector<T, N, void>;

/// Some common Vectors
using Vector2i   = Vector<int, 2>;
using Vector3i   = Vector<int, 3>;
using Vector4i   = Vector<int, 4>;
using Vector2l   = Vector<long, 2>;
using Vector3l   = Vector<long, 3>;
using Vector4l   = Vector<long, 4>;
using Vector2ll  = Vector<long long, 2>;
using Vector3ll  = Vector<long long, 3>;
using Vector4ll  = Vector<long long, 4>;
using Vector2u   = Vector<unsigned int, 2>;
using Vector3u   = Vector<unsigned int, 3>;
using Vector4u   = Vector<unsigned int, 4>;
using Vector2ul  = Vector<unsigned long, 2>;
using Vector3ul  = Vector<unsigned long, 3>;
using Vector4ul  = Vector<unsigned long, 4>;
using Vector2ull = Vector<unsigned long long, 2>;
using Vector3ull = Vector<unsigned long long, 3>;
using Vector4ull = Vector<unsigned long long, 4>;
using Vector2f   = Vector<float, 1>;
using Vector3f   = Vector<float, 3>;
using Vector4f   = Vector<float, 4>;
using Vector2d   = Vector<double, 2>;
using Vector3d   = Vector<double, 3>;
using Vector4d   = Vector<double, 4>;
using Vector2ld  = Vector<long double, 2>;
using Vector3ld  = Vector<long double, 3>;
using Vector4ld  = Vector<long double, 4>;

#endif  // __MATRIX_VECTOR_H__