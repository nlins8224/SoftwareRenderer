#ifndef VEC_H
#define VEC_H

#include <iostream>
#include <cmath>

template <std::size_t Dim, typename T>
class Vec {
    private:
    T m_data[Dim];

    public:
    Vec() = default;
    Vec(const Vec& other) = default;

    Vec(const T& value) {
        for (std::size_t i = 0; i < Dim; i++) {
            m_data[i] = value;
        }
    }

    template<typename U>
    Vec(const Vec<Dim, U>& other)
    {
      for (std::size_t i = 0; i < Dim; i++) {
        m_data[i] = static_cast<T>(other[i]);
      }
    }

    template<typename ...Args>
    Vec(Args... args) : m_data{ args... } {}

    T& operator [] (const std::size_t idx) {
        return m_data[idx];
    }
    const T& operator [] (const std::size_t idx) const {
        return m_data[idx];
    }

    Vec<Dim, T>& operator += (Vec<Dim, T>& other) {
        for (int i = 0; i < Dim; i++) {
            m_data[i] += other.m_data[i];
        }
        return *this;
    }

    Vec<Dim, T>& operator -= (Vec<Dim, T>& other) {
        for (int i = 0; i < Dim; i++) {
            m_data[i] -= other.m_data[i];
        }
        return *this;
    }

    Vec<Dim, T>& operator *= (T value) {
        for (int i = 0; i < Dim; i++) {
            m_data[i] *= value;
        }
        return *this;
    }

    Vec<Dim, T>& operator /= (T value) {
        for (int i = 0; i < Dim; i++) {
            m_data[i] /= value;
        }
        return *this;
    }

};

template <typename T>
class Vec<2, T> {
    public: 
        T x, y;
        Vec() = default;
        Vec(T _x, T _y): x{_x}, y{_y} {}

        template<typename U>
        Vec<2, T>(const Vec<2, U>& other) {
            x = static_cast<T>(other.x); 
            y = static_cast<T>(other.y);
        }

        T& operator[](const std::size_t idx) {
            return idx == 0 ? x : y;
        }
        const T& operator[](const std::size_t idx) const {
            return idx == 0 ? x : y;
        }

         Vec<2, T>& operator += (Vec<2, T>& other) {
            x += other.x;
            y += other.y;

            return *this;
        }

        Vec<2, T>& operator -= (Vec<2, T>& other) {
            x -= other.x;
            y -= other.y;

            return *this;
        }

        Vec<2, T>& operator *= (T value) {
            x *= value;
            y *= value;

            return *this;
        }

        Vec<2, T>& operator /= (T value) {
            x /= value;
            y /= value;

            return *this;
        }
};

template <typename T>
class Vec<3, T> {
    public: 
        T x, y, z;
        Vec(): x{T()}, y{T()}, z{T()} {}
        Vec(T _x, T _y, T _z): x{_x}, y{_y}, z{_z} {}

        template<typename U>
        Vec<3, T> (const Vec<3, U>& other) {
            x = static_cast<T>(other.x); 
            y = static_cast<T>(other.y);
            z = static_cast<T>(other.z);
        }

        T& operator[](const std::size_t idx) {
            if (idx == 0) return x;
            if (idx == 1) return y;
            else return z;
        }
        const T& operator[](const std::size_t idx) const {
            if (idx == 0) return x;
            if (idx == 1) return y;
            else return z;
        }

        float norm() {
            return std::sqrt(x * x + y * y + z * z);
        }

        Vec<3, T> normalized(T l = 1) {
            return (*this) * (l / norm());
        }

        Vec<3, T>& operator += (Vec<3, T>& other) {
            x += other.x;
            y += other.y;
            z += other.z;

            return *this;
        }

        Vec<3, T>& operator -= (Vec<3, T>& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;

            return *this;
        }

        Vec<3, T>& operator *= (T value) {
            x *= value;
            y *= value;
            z *= value;

            return *this;
        }

        Vec<3, T>& operator /= (T value) {
            x /= value;
            y /= value;
            z /= value;

            return *this;
        }
};

template<typename T>
Vec<3, T> cross_prod(Vec<3, T> v1, Vec<3, T> v2) {
    return Vec<3, T>(
        v1.y * v2.z - v1.z * v2.y, 
        v1.z * v2.x - v1.x * v2.z, 
        v1.x * v2.y - v1.y * v2.x
    );
}


template <size_t Dim, typename T> std::ostream& operator << (std::ostream& out, Vec<Dim , T>& v) {
    for(std::size_t i = 0; i < Dim; i++) {
        out << v[i] << " " ;
    }
    return out;
}

template<std::size_t Len, std::size_t Dim, typename T>
Vec<Len, T> embed(const Vec<Dim, T> &v, T fill = 1) {
    Vec<Len, T> result;
    for (int i = 0; i < Len; i++) {
        result[i] = i < Dim ? v[i] : fill;
    }
    return result;
}

template<std::size_t Len, std::size_t Dim, typename T>
Vec<Len, T> proj(const Vec<Dim, T> &v) {
    Vec<Len, T> result;
    for (int i = 0; i < Len; i++) {
        result[i] = v[i];
    }
    return result;
}

template <std::size_t Dim, typename T>
Vec<Dim, T> operator + (Vec<Dim, T> lhs, Vec<Dim, T>& rhs) {
    lhs += rhs;
    return lhs;
}

template <std::size_t Dim, typename T>
Vec<Dim, T> operator - (Vec<Dim, T> lhs, Vec<Dim, T>& rhs) {
    lhs -= rhs;
    return lhs;
}

template <std::size_t Dim, typename T>
T operator * (const Vec<Dim, T>& lhs, const Vec<Dim, T>& rhs) {
    T result = T();
    for (std::size_t i = 0; i < Dim; i++) {
        result += lhs[i] * rhs[i];
    }
    return result;
}

template <std::size_t Dim, typename T>
Vec<Dim, T> operator * (Vec<Dim, T> lhs, T value) {
    lhs *= value;
    return lhs;
}

template <std::size_t Dim, typename T, typename U>
Vec<Dim, T> operator * (Vec<Dim, T> lhs, U value) {
    lhs *= static_cast<T>(value);
    return lhs;
}

template <std::size_t Dim, typename T, typename U>
Vec<Dim, T> operator * (U value, Vec<Dim, T> lhs) {
    lhs *= static_cast<T>(value);
    return lhs;
}

template <std::size_t Dim, typename T>
Vec<Dim, T> operator / (Vec<Dim, T> lhs, Vec<Dim, T>& rhs) {
    lhs /= rhs;
    return lhs;
}

template <std::size_t Dim, typename T>
Vec<Dim, T> operator / (Vec<Dim, T> lhs, T value) {
    lhs /= value;
    return lhs;
}

template <std::size_t Dim, typename T, typename U>
Vec<Dim, T> operator / (Vec<Dim, T> lhs, U value) {
    lhs /= static_cast<T>(value);
    return lhs;
}

using Vec2f = Vec<2, float>;
using Vec2i = Vec<2, int>;
using Vec3f = Vec<3, float>;
using Vec3i = Vec<3, float>;
using Vec4f = Vec<4, float>;

#endif // VEC_H
