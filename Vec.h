#ifndef VEC_H
#define VEC_H

#include <iostream>

template <typename T, std::size_t DIM>
class Vec {
    private:
    T m_data[DIM];

    public:
    Vec() {
        // could be for each loop?
        for (std::size_t i{ DIM }; i >= 0; --i) {
            m_data[i] = T();
        }
    }

    T& operator[](const std::size_t i) {
        return m_data[i];
    }
    const T& operator[](const std::size_t i) const {
        return m_data[i];
    }

    Vec& operator += (T const& other) {}

    
};

// is this even needed?
template <typename T> class Vec<T, 2> {
    private:
        T m_x, m_y;
    public:
        Vec(): x{T()}, y(T()) {}
        Vec(T x, T y): m_x{x}, m_y{y} {}

        template <typename U> Vec<2, T> (const Vec<2, U> &v);

};

#endif // VEC_H
