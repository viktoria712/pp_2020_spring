// Copyright 2020 Isaev Ilya
#include <vector>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <omp.h>
#include "../../../modules/task_2/isaev_fox_alg/fox_alg.h"

Matrix getRandomMatrix(const int& n) {
    if (n <= 0) {
        throw std::exception();
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 100);

    Matrix res(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            res[i][j] = dis(gen);
        }
    }
    return res;
}

Matrix naiveMultiplication(const Matrix& mat1, const Matrix& mat2) {
    if (mat1[0].size() != mat2.size())
        throw std::exception();

    size_t n = mat1.size();
    size_t m = mat2[0].size();
    Matrix res(n, std::vector<double>(n, 0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            for (size_t k = 0; k < mat2.size(); ++k) {
                res[i][j] += mat1[i][k]*mat2[k][j];
            }
        }
    }
    return res;
}

Matrix blockMultiplication(const Matrix& mat1, const Matrix& mat2) {
    if (mat1[0].size() != mat2.size())
        throw std::exception();
    size_t n = mat1.size();
    size_t q = n / std::sqrt(n);
    size_t block_size = n / q;
    Matrix res(n, std::vector<double>(n, 0));

    for (size_t jj = 0; jj < n; jj+=block_size) {
        for (size_t kk = 0; kk < n; kk+=block_size) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = jj; j < std::min(jj+block_size, n); ++j) {
                    for (size_t k = kk; k < std::min(kk+block_size, n); ++k) {
                        res[i][j] += mat1[i][k] * mat2[k][j];
                    }
                }
            }
        }
    }

    return res;
}

bool matrixComparison(const Matrix& mat1, const Matrix& mat2) {
    if (mat1.size() != mat2.size() || mat1[0].size() != mat2[0].size())
        throw std::exception();
    bool isequal = true;
    for (size_t i = 0; i < mat1.size(); ++i) {
        for (size_t j = 0; j < mat1[0].size(); ++j)
            if (!doubleComparison(mat1[i][j], mat2[i][j]))
                isequal = false;
    }
    return isequal;
}

Matrix foxAlgParallel(const Matrix& mat1, const Matrix& mat2, const unsigned& n_threads) {
    if (!isSquared(mat1) || !isSquared(mat2) || mat1.size() != mat2.size())
        throw std::logic_error("Matrix should be squared");

    size_t n = mat1.size();
    size_t q = std::sqrt(n_threads);
    size_t block_size = n / q;

    Matrix res(n, std::vector<double>(n, 0));

    #pragma omp parallel for num_threads(n_threads)
    for (size_t jj = 0; jj < n; jj+=block_size) {
        for (size_t kk = 0; kk < n; kk+=block_size) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = jj; j < std::min(jj+block_size, n); ++j) {
                    for (size_t k = kk; k < std::min(kk+block_size, n); ++k) {
                        res[i][j] += mat1[i][k] * mat2[k][j];
                    }
                }
            }
        }
    }
    return res;
}
