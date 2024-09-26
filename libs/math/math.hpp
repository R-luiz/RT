#ifndef MATH_HPP
#define MATH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <immintrin.h>

class Vector3
{
private:
    __m128 data;
    float _x;
    float _y;
    float _z;

public:
    Vector3() : data(_mm_setzero_ps()), _x(0), _y(0), _z(0) {}
    Vector3(float x, float y, float z) : data(_mm_set_ps(0, z, y, x)), _x(x), _y(y), _z(z) {}
    explicit Vector3(__m128 m) : data(m) {}

    float x() const { return _mm_cvtss_f32(data); }
    float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(data, data, _MM_SHUFFLE(1, 1, 1, 1))); }
    float z() const { return _mm_cvtss_f32(_mm_shuffle_ps(data, data, _MM_SHUFFLE(2, 2, 2, 2))); }

    void x(float f) { data = _mm_set_ps(0, _z, _y, f); }
    void y(float f) { data = _mm_set_ps(0, _z, f, _x); }
    void z(float f) { data = _mm_set_ps(0, f, _y, _x); }

    Vector3 operator+(const Vector3& v) const {
        return Vector3(_mm_add_ps(data, v.data));
    }

    Vector3 operator-(const Vector3& v) const {
        return Vector3(_mm_sub_ps(data, v.data));
    }

    Vector3 operator*(float s) const {
        return Vector3(_mm_mul_ps(data, _mm_set1_ps(s)));
    }

    float operator*(const Vector3& v) const {
        return _mm_cvtss_f32(_mm_dp_ps(data, v.data, 0x71));
    }

    Vector3 operator/(float s) const {
        return Vector3(_mm_div_ps(data, _mm_set1_ps(s)));
    }

    Vector3 cross(const Vector3& v) const {
        return Vector3(_mm_sub_ps(
            _mm_mul_ps(_mm_shuffle_ps(data, data, _MM_SHUFFLE(3, 0, 2, 1)),
                       _mm_shuffle_ps(v.data, v.data, _MM_SHUFFLE(3, 1, 0, 2))),
            _mm_mul_ps(_mm_shuffle_ps(data, data, _MM_SHUFFLE(3, 1, 0, 2)),
                       _mm_shuffle_ps(v.data, v.data, _MM_SHUFFLE(3, 0, 2, 1)))
        ));
    }

    float length() const {
        return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(data, data, 0x71)));
    }

    Vector3 normalize() const {
        __m128 len = _mm_sqrt_ss(_mm_dp_ps(data, data, 0x7F));
        len = _mm_shuffle_ps(len, len, 0);
        return Vector3(_mm_div_ps(data, len));
    }

    void print() const {
        std::cout << "[" << x() << ", " << y() << ", " << z() << "]" << std::endl;
    }
};

inline Vector3 operator*(float s, const Vector3& v) {
    return v * s;
}

#endif