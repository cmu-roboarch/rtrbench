/** $lic$
 * Copyright (c) 2022 Carnegie Mellon University
 *
 * This file is part of RTRBench.
 *
 * Permission is hereby granted, free of charge, to any
 * person obtaining a copy of this software and
 * associated documentation files (the "Software"), to
 * deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
 * EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#pragma once

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include "log.h"
#include "parallel_hashmap/phmap.h"

#define PI (3.141592654)
#define TWO_PI (6.283185308)

template<typename T>
static void printContainer(T c) {
    for (auto e : c) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}


template<typename T>
static void printArray(T a, size_t size) {
    for (size_t i = 0; i < size; i++) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
}


template<typename T>
static void printNonIterableContrainer(T c) {
    while (!c.empty()) {
        std::cout << c.top() << " ";
        c.pop();
    }
    std::cout << std::endl;
}


template<typename T>
static std::string containerToString(T c) {
    std::string str = "<";
    for (auto e : c) {
        str += std::to_string(e);
        str += " ";
    }
    str.pop_back();
    str.append(">");
    return str;
}


// The base class for the "enhanced" parallel set (EPS). It supports containers
// like vector as the key. The vector, depending on its "size," is hashed using
// one of the above functions.
template<typename T>
class BaseEPS {
    public:
        static_assert(std::is_same_v<T, std::vector<int>> \
                || std::is_same_v<T, std::list<int>>);

        BaseEPS() = default;

        void insert(const T &c) {
            uint64_t h = this->getContainerHash(c);
            this->theSet.insert(h);
            assert(this->contains(c));
        }

        bool contains(const T &c) {
            uint64_t h = this->getContainerHash(c);
            return this->theSet.contains(h);
        }

        void clear() {
            this->theSet.clear();
        }

    protected:
        virtual uint64_t getContainerHash(const T &c) = 0;

    private:
        phmap::parallel_flat_hash_set<uint64_t> theSet;
};


template<typename T>
class ShortEPS : public BaseEPS<T> {
    public:
        explicit ShortEPS(int shamount) : BaseEPS<T>() {
            this->shamount = shamount;
        }

    protected:
        inline uint64_t getContainerHash(const T &c) {
            // Hash the container C of type integer. Different elements of C
            // are "mapped" to different bits of the output. The assumption is
            // the values are strictly smaller than 2^shamount
            uint64_t r = 0;
            for (const auto &e : c) {
                r <<= this->shamount;
                r += e;
            }

            return r;
        }

    private:
        int shamount;
};


template<typename T>
class LongEPS : public BaseEPS<T> {
    public:
        LongEPS() : BaseEPS<T>() {}

    protected:
        inline uint64_t getContainerHash(const T &c) {
            std::stringstream r;
            std::copy(c.begin(), c.end(), std::ostream_iterator<int>(r, ","));
            std::hash<std::string> hasher;
            return hasher(r.str());
        }
};


// Similar to EPS, but the data structure is "map"
template<typename K, typename V>
class BaseEPM {
    public:
        static_assert(std::is_same_v<K, std::vector<int>> \
                || std::is_same_v<K, std::list<int>>);

        BaseEPM() = default;

        void insert(const K &c, V value) {
            uint64_t h = this->getContainerHash(c);
            this->theMap[h] = value;
            assert(this->contains(c));
        }

        bool contains(const K &c) {
            uint64_t h = this->getContainerHash(c);
            return this->theMap.contains(h);
        }

        V getValue(const K &c) {
            assert(this->contains(c));
            uint64_t h = this->getContainerHash(c);
            return this->theMap[h];
        }

        void clear() {
            this->theMap.clear();
        }

    protected:
        virtual uint64_t getContainerHash(const K &c) = 0;

    private:
        phmap::parallel_flat_hash_map<uint64_t, V> theMap;
};


template<typename K, typename V>
class ShortEPM : public BaseEPM<K, V> {
    public:
        explicit ShortEPM(int shamount) : BaseEPM<K, V>() {
            this->shamount = shamount;
        }

    protected:
        inline uint64_t getContainerHash(const K &c) {
            // Hash the container C of type integer. Different elements of C
            // are "mapped" to different bits of the output. The assumption is
            // the values are strictly smaller than 2^shamount
            uint64_t r = 0;
            for (const auto &e : c) {
                r <<= this->shamount;
                r += e;
            }
            return r;
        }

    private:
        int shamount;
};


template<typename K, typename V>
class LongEPM : public BaseEPM<K, V> {
    public:
        LongEPM() : BaseEPM<K, V>() {}

    protected:
        inline uint64_t getContainerHash(const K &c) {
            std::stringstream r;
            std::copy(c.begin(), c.end(), std::ostream_iterator<int>(r, ","));
            std::hash<std::string> hasher;
            return hasher(r.str());
        }
};


template<typename T>
static double getEuclideanDistance(const T &v1, const T &v2) {
    assert(v1.size() == v2.size());
    double dist = 0;
    for (int i = 0; i < static_cast<int>(v1.size()); i++) {
        dist += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }

    return sqrt(dist);
}


template<typename T>
static int getManhattanDistance(const T &v1, const T &v2) {
    assert(v1.size() == v2.size());
    int dist = 0;
    for (int i = 0; i < static_cast<int>(v1.size()); i++) {
        dist += std::abs(v1[i] - v2[i]);
    }

    return dist;
}

template<typename T>
bool matrix2dInverse(T **A, double **inverse) {
    double det = A[0][0]*A[1][1] - A[1][0]*A[0][1];
    if (det == 0) return false;

    double invDet = 1.0 / det;

    inverse[0][0] = invDet * A[1][1];
    inverse[0][1] = -invDet * A[0][1];
    inverse[1][0] = -invDet * A[1][0];
    inverse[1][1] = invDet * A[0][0];

    return true;
}

template<typename T>
void matrixInverse(T **A, double **inv, int n) {
    // Based on <https://chi3x10.wordpress.com/2008/05/28/calculate-matrix-inversion-in-c/>

    auto getMinor = [](double **src, double **dest, int row, int col, int n) {
        int rowCount = 0;
        for (int i = 0; i < n; i++) {
            if (i != row) {
                int colCount = 0;
                for (int j = 0; j < n; j++) {
                    if (j != col) {
                        dest[rowCount][colCount] = src[i][j];
                        colCount++;
                    }
                }
                rowCount++;
            }
        }
    };

    const auto calcDeterminant = [&getMinor](double **mat, int n) -> double {
        auto detImpl = [&getMinor](double **mat, int n, auto& detRef) mutable {
            if (n == 1) return mat[0][0];
            double det = 0;

            double **minor = new double*[n-1];
            for (int i = 0; i < n-1; i++) {
                minor[i] = new double[n-1];
            }

            for (int i = 0; i < n; i++) {
                getMinor(mat, minor, 0, i, n);
                det += (i%2 == 1 ? -1.0 : 1.0) * mat[0][i] * \
                       detRef(minor, n-1, detRef);
            }

            for (int i = 0; i < n-1; i++) {
                delete[] minor[i];
            }

            delete[] minor;
            return det;
        };

        return detImpl(mat, n, detImpl);
    };

    double det = 1.0 / calcDeterminant(A, n);

    double *temp = new double[(n-1) * (n-1)];
    double **minor = new double*[n-1];
    for (int i = 0; i < n-1; i++) {
        minor[i] = temp + (i * (n-1));
    }

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            getMinor(A, minor, j, i, n);
            inv[i][j] = det * calcDeterminant(minor, n-1);
            if ((i+j)%2 == 1) {
                inv[i][j] = -inv[i][j];
            }
        }
    }

    delete[] temp;
    delete[] minor;
}

template<typename T>
void matrixMultiplication(T **A, T **B, T **res, \
        int rowA, int rowB, int colB) {
    assert(A != res && B != res);
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            res[i][j] = 0;
            for (int k = 0; k < rowB; k++) {
                res[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

template<typename T>
void matrixTranspose(T **A, T **B, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            B[j][i] = A[i][j];
        }
    }
}

template<typename T>
void matrixAddition(T **A, T **B, T **res, \
        int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            res[i][j] = A[i][j] + B[i][j];
        }
    }
}

template<typename T>
void matrixSubtraction(T **A, T **B, T **res, \
        int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            res[i][j] = A[i][j] - B[i][j];
        }
    }
}

template<typename T>
void printMatrix(T **A, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
static std::string matrixToString(T **A, int rows, int cols) {
    std::string str = "<";

    for (int i = 0; i < rows; i++) {
        str.append("(");
        for (int j = 0; j < cols; j++) {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(8) << A[i][j];
            str += ss.str() + ", ";
        }
        str.pop_back();
        str.pop_back();
        str.append("), ");
    }
    str.pop_back();
    str.pop_back();
    str.append(">");

    return str;
}

template<typename T>
static double **reshape3dTo2d(T ***A, int x, int y, int z) {
    int newX = x * y;
    int newY = z;

    double **newMatrix = new double*[newX];
    for (int i = 0; i < newX; i++) {
        newMatrix[i] = new double[newY];
    }

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                int newRow = i*y + j;
                int newCol = k;
                newMatrix[newRow][newCol] = A[i][j][k];
            }
        }
    }

    return newMatrix;
}

template<typename T>
static double degToRad(T angle) {
    return angle * (PI / 180);
}

template<typename T>
static double wrapToPi(T angle) {
    while (angle > PI) angle -= TWO_PI;
    while (angle < -PI) angle += TWO_PI;
    return angle;
}
