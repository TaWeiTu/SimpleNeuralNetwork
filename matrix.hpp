#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <cassert>
#include <cstdio>
#include <random>
#include <functional>


template <typename T>
struct matrix {
    std::vector<std::vector<T>> data;
    std::vector<size_t> dim;

    matrix();
    matrix(size_t, size_t);
    matrix(size_t, size_t, const std::function<T(size_t, size_t)> &);
    matrix(const std::vector<size_t> &);
    matrix(const std::vector<size_t> &_, const std::function<T(size_t, size_t)> &);

    matrix<T> operator+(const matrix<T> &) const;
    matrix<T> operator-(const matrix<T> &) const;
    matrix<T> operator*(const matrix<T> &) const;
    matrix<T> operator*(const T &) const;
    matrix<T> operator^(const matrix<T> &) const;

    matrix<T> &operator+=(const matrix<T> &);
    matrix<T> &operator-=(const matrix<T> &);
    matrix<T> &operator*=(const matrix<T> &);
    matrix<T> &operator*=(const T &);
    matrix<T> &operator^=(const matrix<T> &);

    std::vector<T> &operator[](const size_t &);
    const std::vector<T> operator[](const size_t &) const;

    void resize(size_t, size_t);
    void resize(const std::vector<size_t> &);
    void randomized(size_t, size_t);
    void randomized(const std::vector<size_t> &);
    void randomized();

    matrix<T> apply(std::function<T(T)>) const;
    matrix<T> transpose() const;
    matrix<T> slice(int, int, int, int) const;

    void debug() const;
};

template <typename T> matrix<T> concate_h(matrix<T>, matrix<T>);
template <typename T> matrix<T> concate_v(matrix<T>, matrix<T>);

template <typename T> matrix<T> ones(size_t, size_t);
template <typename T> matrix<T> ones(const std::vector<size_t> &);
template <typename T> matrix<T> zeros(size_t, size_t);
template <typename T> matrix<T> zeros(const std::vector<size_t> &);
template <typename T> matrix<T> identity(size_t);


template <typename T>
matrix<T>::matrix() {}

template <typename T>
matrix<T>::matrix(size_t n, size_t m) {
    dim = { n, m };
    data.resize(n);
    for (size_t i = 0; i < n; ++i) 
        data[i].resize(m);
} 

template <typename T>
matrix<T>::matrix(size_t n, size_t m, const std::function<T(size_t, size_t)> &f) {
    dim = { n, m };
    data.resize(n);
    for (size_t i = 0; i < n; ++i)
        data[i].resize(m);
    for (size_t i = 0; i < n; ++i) for (int j = 0; j < m; ++j)
        data[i][j] = f(i, j);
}

template <typename T>
matrix<T>::matrix(const std::vector<size_t> &d) {
    dim = d;
    data.resize(dim[0]);
    for (size_t i = 0; i < dim[0]; ++i)
        data[i].resize(dim[1]);
}

template <typename T>
matrix<T>::matrix(const std::vector<size_t> &d, const std::function<T(size_t, size_t)> &f) {
    dim = d;
    data.resize(dim[0]);
    for (size_t i = 0; i < dim[0]; ++i)
        data[i].resize(dim[1]);
    for (size_t i = 0; i < dim[0]; ++i) for (size_t j = 0; j < dim[1]; ++j)
        data[i][j] = f(i, j);
}

template <typename T>
std::vector<T>& matrix<T>::operator[](const size_t &i) {
    return data[i];
}

template <typename T>
const std::vector<T> matrix<T>::operator[](const size_t &i) const {
    return data[i];
}

template <typename T>
matrix<T> matrix<T>::operator+(const matrix<T> &rhs) const {
    assert(dim == rhs.dim);
    matrix res(dim);
    for (size_t i = 0; i < dim[0]; ++i) for (size_t j = 0; j < dim[1]; ++j) 
        res.data[i][j] = data[i][j] + rhs.data[i][j];
    return res;
}

template <typename T>
matrix<T>& matrix<T>::operator+=(const matrix<T> &rhs) {
    assert(dim == rhs.dim);
    for (size_t i = 0; i < dim[0]; ++i) for (size_t j = 0; j < dim[1]; ++j) 
        data[i][j] += rhs.data[i][j];
    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator-(const matrix<T> &rhs) const {
    assert(dim == rhs.dim);
    matrix<T> res(dim);
    for (size_t i = 0; i < dim[0]; ++i) for (size_t j = 0; j < dim[1]; ++j) 
        res.data[i][j] = data[i][j] - rhs.data[i][j];
    return res;
}

template <typename T>
matrix<T>& matrix<T>::operator-=(const matrix<T> &rhs) {
    assert(dim == rhs.dim);
    for (size_t i = 0; i < dim[0]; ++i) for (size_t j = 0; j < dim[1]; ++j) 
        data[i][j] -= rhs.data[i][j];
    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator*(const T &scalar) const {
    matrix<T> res(dim);
    for (size_t i = 0; i < dim[0]; ++i) for (size_t j = 0; j < dim[1]; ++j) 
        res.data[i][j] = data[i][j] * scalar;
    return res;
}

template <typename T>
matrix<T>& matrix<T>::operator*=(const T &scalar) {
    for (size_t i = 0; i < dim[0]; ++i) for (size_t j = 0; j < dim[1]; ++j) 
        data[i][j] *= scalar;
    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator*(const matrix<T> &rhs) const {
    assert(dim[1] == rhs.dim[0]);
    matrix<T> res(dim[0], rhs.dim[1]);
    for (size_t i = 0; i < dim[0]; ++i) for (size_t k = 0; k < dim[1]; ++k) {
        for (size_t j = 0; j < rhs.dim[1]; ++j) 
            res.data[i][j] += data[i][k] * rhs.data[k][j];
    }
    return res;
}

template <typename T>
matrix<T>& matrix<T>::operator*=(const matrix<T> &rhs) {
    matrix<T> res = (*this) * rhs;
    (*this) = res;
    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator^(const matrix<T> &rhs) const {
    assert(dim == rhs.dim);
    matrix res(dim);
    for (size_t i = 0; i < dim[0]; ++i) for (size_t j = 0; j < dim[1]; ++j)
        res.data[i][j] = data[i][j] * rhs.data[i][j];
    return res;
}

template <typename T>
matrix<T>& matrix<T>::operator^=(const matrix<T> &rhs) {
    assert(dim == rhs.dim);
    for (size_t i = 0; i < dim[0]; ++i) for (size_t j = 0; j < dim[1]; ++j)
        data[i][j] *= rhs.data[i][j];
    return *this;
}

template <typename T>
matrix<T> matrix<T>::apply(std::function<T(T)> f) const {
    matrix res(dim);
    for (size_t i = 0; i < dim[0]; ++i) for (size_t j = 0; j < dim[1]; ++j) 
        res.data[i][j] = f(data[i][j]);
    return res;
}

template <typename T>
void matrix<T>::resize(size_t new_n, size_t new_m) {
    resize({ new_n, new_m }); 
}

template <typename T>
void matrix<T>::resize(const std::vector<size_t> &d) {
    dim = d;
    data.resize(dim[0]);
    for (size_t i = 0; i < dim[0]; ++i) 
        data[i].resize(dim[1]);
}

template <typename T>
void matrix<T>::randomized(size_t new_n, size_t new_m) {
    randomized({ new_n, new_m }); 
}

template <typename T>
void matrix<T>::randomized(const std::vector<size_t> &d) {
    resize(d);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-5.0, 5.0);
    for (size_t i = 0; i < dim[0]; ++i) {
        for (size_t j = 0; j < dim[1]; ++j)
            data[i][j] = dis(gen);
    }
}

template <typename T>
void matrix<T>::randomized() {
    randomized(dim); 
}

template <typename T>
matrix<T> matrix<T>::transpose() const {
    matrix<T> res(dim[1], dim[0]);
    for (size_t i = 0; i < dim[0]; ++i) for (size_t j = 0; j < dim[1]; ++j) 
        res.data[j][i] = data[i][j];
    return res;
}

template <typename T>
matrix<T> concate_h(matrix<T> a, matrix<T> b) {
    assert(a.dim[0] == b.dim[0]);
    matrix<T> res(a.dim[0], a.dim[1] + b.dim[1]);
    for (size_t i = 0; i < a.dim[0]; ++i) {
        for (size_t j = 0; j < a.dim[1]; ++j) 
            res[i][j] = a[i][j];
        for (size_t j = 0; j < b.dim[1]; ++j) 
            res[i][j + a.dim[1]] = b[i][j];
    }
    return res;
}

template <typename T>
matrix<T> concate_v(matrix<T> a, matrix<T> b) {
    assert(a.dim[1] == b.dim[1]);
    matrix<T> res(a.dim[0] + b.dim[0], a.dim[1]);
    for (size_t j = 0; j < a.dim[1]; ++j) {
        for (size_t i = 0; i < a.dim[0]; ++i) 
            res[i][j] = a[i][j];
        for (size_t i = 0; i < b.dim[0]; ++i) 
            res[i + a.dim[0]][j] = b[i][j];
    }
    return res;
}

template <typename T>
matrix<T> ones(const std::vector<size_t> &d) {
    matrix<T> res(d);
    for (size_t i = 0; i < res.dim[0]; ++i) for (size_t j = 0; j < res.dim[1]; ++j)
        res[i][j] = 1;
    return res;
}

template <typename T>
matrix<T> ones(size_t n, size_t m) {
    return ones<T>({ n, m }); 
}

template <typename T>
matrix<T> zeros(const std::vector<size_t> &d) {
    matrix<T> res(d);
    for (size_t i = 0; i < res.dim[0]; ++i) for (size_t j = 0; j < res.dim[1]; ++j)
        res[i][j] = 0;
    return res;
}

template <typename T>
matrix<T> zeros(size_t n, size_t m) {
    return zeros<T>({ n, m }); 
}

template <typename T>
matrix<T> identity(size_t n) {
    matrix<T> res(n, n);
    for (size_t i = 0; i < n; ++i) 
        res[i][i] = 1;
    return res;
}

#ifdef DEBUG

template <typename T>
void matrix<T>::debug() const {
    printf("size: %zu x %zu\n", n, m);
    for (size_t i = 0; i < dim[0]; ++i) {
        for (size_t j = 0; j < dim[1]; ++j) printf("%.3lf ", data[i][j]);
        puts("");
    }
    puts("");
}

#else

template <typename T>
void matrix<T>::debug() const {}

#endif

template <typename T>
matrix<T> matrix<T>::slice(int u, int d, int l, int r) const {
    u = u == -1 ? 0 : u; d = d == -1 ? dim[0] : d;
    l = l == -1 ? 0 : l; r = r == -1 ? dim[1] : r;
    matrix<T> res(d - u, r - l);
    for (size_t i = u; i < d; ++i) {
        for (size_t j = l; j < r; ++j)
            res.data[i - u][j - l] = data[i][j];
    }
    return res;
}

#endif
