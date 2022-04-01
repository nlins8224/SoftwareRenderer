#ifndef MATRIX_H
#define MATRIX_H

#include <iostream> 
#include "Vec.h"

template<std::size_t dim_rows, std::size_t dim_cols, typename T>
class Matrix {
    private:
        Vec<dim_cols, T> m_rows[dim_rows];
    public:
        Matrix() = default;
        Matrix(const Matrix& other) = default;

        Vec<dim_cols, T>& operator[] (const std::size_t idx) {
            return m_rows[idx];
        }

        const Vec<dim_cols, T>& operator[] (const std::size_t idx) const {
            return m_rows[idx];
        }

        Vec<dim_rows, T> get_column(const std::size_t idx) const {
            Vec<dim_rows, T> result;
            for (std::size_t i = 0; i < dim_rows; i++) {
                result[i] = m_rows[i][idx];
            }
            return result;
        }

        void set_column(std::size_t idx, Vec<dim_rows, T> v) {
            for (std::size_t i = 0; i < dim_rows; i++) {
                m_rows[i][idx] = v[i];
            }
        }

        static Matrix<dim_rows, dim_cols, T> identity() {
            Matrix<dim_rows, dim_cols, T> result;
            for (std::size_t i = 0; i < dim_rows; i++) {
                for (std::size_t j = 0; j < dim_cols; j++) {
                    result[i][j] = (i == j);
                }
            }
            return result;
        }

        Matrix<dim_rows - 1, dim_cols - 1, T> get_minor(std::size_t row, std::size_t col) const {
            Matrix<dim_rows - 1, dim_cols - 1, T> result;
            for (std::size_t i = 0; i < dim_rows - 1; i++) {
                for (std::size_t j = 0; j < dim_cols - 1; j++) {
                    result[i][j] = m_rows[i < row ? i : i + 1][j < col ? j : j + 1];
                }
            }
            return result;
        } 

        T det() const {
            return det_elements(*this);
        }

        private: 
        static T det_elements(const Matrix<dim_cols, dim_cols, T>& src) {
            if constexpr (dim_cols == 1) {
                return src[0][0];
            }
            else {
                T result = 0;
                for (std::size_t i = 0; i < dim_cols; i++) {
                    result += src[0][i] * src.cofactor(0, i);
                }
                return result;
            }
        }

        public:
        T cofactor (std::size_t row, std::size_t col) const {
            int sign = (row + col) % 2 ? -1 : 1;
            return get_minor(row, col).det() * sign;
        }

        Matrix<dim_rows, dim_cols, T> adjugate() const {
            Matrix<dim_rows, dim_cols, T> result;
            for (std::size_t i = 0; i < dim_rows; i++) {
                for (std::size_t j = 0; j < dim_cols; j++) {
                    result[i][j] = cofactor(i, j);
                }
            }
            return result;
        }

        Matrix<dim_rows, dim_cols, T> invert_transpose() {
            Matrix<dim_rows, dim_cols, T> result = adjugate();
            T temp = result[0] * m_rows[0];
            return result / temp;
        }

        Matrix<dim_cols, dim_rows, T> operator/=(const T& value) {
            for (std::size_t i = 0; i < dim_rows; i++) {
                m_rows[i] /= value;
            }
            return *this;
        }

        Matrix<dim_cols, dim_rows, T> operator*=(const T& value) {
            for (std::size_t i = 0; i < dim_rows; i++) {
                m_rows[i] *= value;
            }
            return *this;
        }

};

/* Matrix * Vector */
template<std::size_t dim_rows, std::size_t dim_cols, typename T>
Vec<dim_rows, T> operator*(const Matrix<dim_rows, dim_cols, T>& lhs, const Vec<dim_cols, T>& value) {
    Vec<dim_rows, T> result;
    for (std::size_t i = 0; i < dim_rows; i++) {
        result[i] = lhs[i] * value;
    }
    return result;
}

/* Matrix * Matrix */ 
template<std::size_t R1, std::size_t C1, std::size_t C2, typename T> 
Matrix<R1, C2, T> operator*(const Matrix<R1, C1, T>& lhs, const Matrix<C1, C2, T>& rhs) {
    Matrix<R1, C2, T> result;
    for (std::size_t i = 0; i < R1; i++) {
        for (std::size_t j = 0; j < C2; j++) {
            result[i][j] = lhs[i] * rhs.get_column(j);
        }
    }
    return result;
}

/* Matrix * value */ 
template<std::size_t dim_rows, std::size_t dim_cols, typename T>
Matrix<dim_rows, dim_cols, T> operator*(Matrix<dim_rows, dim_cols, T> lhs, const T& value) {
    return lhs *= value;
}

/* Matrix / value */ 
template<std::size_t dim_rows, std::size_t dim_cols, typename T>
Matrix<dim_rows, dim_cols, T> operator/(Matrix<dim_rows, dim_cols, T> lhs, const T& value) {
    return lhs /= value;
}

template <std::size_t dim_rows, std::size_t dim_cols, typename T> 
std::ostream& operator<<(std::ostream& out, Matrix<dim_rows, dim_cols,T>& m) {
    for (std::size_t i = 0; i < dim_rows; i++) 
        out << m[i] << std::endl;
    return out;
}

using Matrix4f = Matrix<4, 4, float>;

#endif // MATRIX_H