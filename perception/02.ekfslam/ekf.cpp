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

#include "ekf.h"
#include <vector>
#include "rtrbench_utils.h"

ExtKalmanFillter::ExtKalmanFillter(double sigX2, double sigY2, \
        double sigAlpha2, double sigBeta2, double sigR2, \
        std::vector<double> initialMeasurement) {
    this->numLandmarks = static_cast<int>(initialMeasurement.size()) / 2;

    this->controlCov = new double*[this->STATE_ENTRIES];
    for (int i = 0; i < this->STATE_ENTRIES; i++) {
        this->controlCov[i] = new double[this->STATE_ENTRIES]();
    }
    this->controlCov[0][0] = sigX2;
    this->controlCov[1][1] = sigY2;
    this->controlCov[2][2] = sigAlpha2;

    this->measurementCov = new double*[this->NUM_MEAS_TYPE];
    for (int i = 0; i < this->NUM_MEAS_TYPE; i++) {
        this->measurementCov[i] = new double[this->NUM_MEAS_TYPE]();
    }
    this->measurementCov[0][0] = sigBeta2;
    this->measurementCov[1][1] = sigR2;

    // Initial pos and uncertainty
    double rX = 0, rY = 0, rTheta = 0;
    double *pos = new double[this->STATE_ENTRIES];
    pos[0] = rX;
    pos[1] = rY;
    pos[2] = rTheta;

    double **posCov = new double*[this->STATE_ENTRIES];
    for (int i = 0; i < this->STATE_ENTRIES; i++) {
        posCov[i] = new double[this->STATE_ENTRIES]();
    }
    posCov[0][0] = 0.02 * 0.02;
    posCov[1][1] = 0.02 * 0.02;
    posCov[2][2] = 0.1 * 0.1;

    // Landmarks
    double *landmarks = new double[2*this->numLandmarks]();
    double **landmarksCov = new double*[2*this->numLandmarks];
    for (int i = 0; i < 2*this->numLandmarks; i++) {
        landmarksCov[i] = new double[2*this->numLandmarks]();
    }

    for (int l = 0; l < this->numLandmarks; l++) {
        double lBeta = initialMeasurement[2*l];
        double lRange = initialMeasurement[2*l + 1];

        double lX = rX + lRange * cos(rTheta + lBeta);
        double lY = rY + lRange * sin(rTheta + lBeta);

        landmarks[2*l] = lX;
        landmarks[2*l + 1] = lY;

        double **jacobian = new double*[this->NUM_MEAS_TYPE];
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) {
            jacobian[i] = new double[this->NUM_MEAS_TYPE];
        }
        jacobian[0][0] = -lRange * sin(rTheta + lBeta);
        jacobian[0][1] = cos(rTheta + lBeta);
        jacobian[1][0] = lRange * cos(rTheta + lBeta);
        jacobian[1][1] = sin(rTheta + lBeta);

        double **jacobianT = new double*[this->NUM_MEAS_TYPE];
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) {
            jacobianT[i] = new double[this->NUM_MEAS_TYPE];
        }
        matrixTranspose(jacobian, jacobianT, this->NUM_MEAS_TYPE, \
                this->NUM_MEAS_TYPE);

        double **temp = new double*[this->NUM_MEAS_TYPE];
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) {
            temp[i] = new double[this->NUM_MEAS_TYPE];
        }
        matrixMultiplication(jacobian, this->measurementCov, temp, \
                this->NUM_MEAS_TYPE, this->NUM_MEAS_TYPE, this->NUM_MEAS_TYPE);

        double **lCov = new double*[this->NUM_MEAS_TYPE];
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) {
            lCov[i] = new double[this->NUM_MEAS_TYPE];
        }
        matrixMultiplication(temp, jacobianT, lCov, this->NUM_MEAS_TYPE, \
                this->NUM_MEAS_TYPE, this->NUM_MEAS_TYPE);

        landmarksCov[2*l][2*l] = lCov[0][0];
        landmarksCov[2*l][2*l+1] = lCov[0][1];
        landmarksCov[2*l+1][2*l] = lCov[1][0];
        landmarksCov[2*l+1][2*l+1] = lCov[1][1];

        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) delete[] jacobian[i];
        delete[] jacobian;
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) delete[] jacobianT[i];
        delete[] jacobianT;
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) delete[] temp[i];
        delete[] temp;
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) delete[] lCov[i];
        delete[] lCov;
    }

    // State vector X: pos and landmarks
    // Covarianec matrix P: pos and landmarks covariances
    this->numEntries = this->STATE_ENTRIES + 2*this->numLandmarks;
    this->X = new double[this->numEntries];
    this->X[0] = pos[0];
    this->X[1] = pos[1];
    this->X[2] = pos[2];
    for (int i = 0; i < this->numLandmarks; i++) {
        this->X[this->STATE_ENTRIES + 2*i] = landmarks[2*i];
        this->X[this->STATE_ENTRIES + 2*i + 1] = landmarks[2*i + 1];
    }

    this->P = new double*[this->numEntries];
    for (int i = 0; i < this->numEntries; i++) {
        this->P[i] = new double[this->numEntries]();
    }
    for (int i = 0; i < this->STATE_ENTRIES; i++) {
        for (int j = 0; j < this->STATE_ENTRIES; j++) {
            this->P[i][j] = posCov[i][j];
        }
    }
    for (int i = 0; i < 2*this->numLandmarks; i++) {
        for (int j = 0; j < 2*this->numLandmarks; j++) {
            int xIdx = this->STATE_ENTRIES + i;
            int yIdx = this->STATE_ENTRIES + j;
            this->P[xIdx][yIdx] = landmarksCov[i][j];
        }
    }

    this->eye = new double*[this->numEntries];
    for (int i = 0; i < this->numEntries; i++) {
        this->eye[i] = new double[this->numEntries]();
    }
    for (int i = 0; i < this->numEntries; i++) this->eye[i][i] = 1.0;

    delete[] pos;
    for (int i = 0; i < this->STATE_ENTRIES; i++) delete[] posCov[i];
    delete[] posCov;
    delete[] landmarks;
    for (int i = 0; i < 2*this->numLandmarks; i++) delete[] landmarksCov[i];
    delete[] landmarksCov;
}

ExtKalmanFillter::~ExtKalmanFillter() {
    for (int i = 0; i < this->STATE_ENTRIES; i++) {
        delete[] this->controlCov[i];
    }
    delete[] this->controlCov;

    for (int i = 0; i < this->NUM_MEAS_TYPE; i++) {
        delete[] this->measurementCov[i];
    }
    delete[] this->measurementCov;

    delete[] this->X;

    for (int i = 0; i < this->numEntries; i++) {
        delete[] this->P[i];
    }
    delete[] P;

    for (int i = 0; i < this->numEntries; i++) {
        delete[] this->eye[i];
    }
    delete[] this->eye;
}

void ExtKalmanFillter::predict(double d, double alpha) {
    double xT = this->X[0], yT = this->X[1], thetaT = this->X[2];
    this->X[0] = xT + d*cos(thetaT);
    this->X[1] = yT + d*sin(thetaT);
    this->X[2] = thetaT + alpha;

    double **G = new double*[this->STATE_ENTRIES];
    double **GT = new double*[this->STATE_ENTRIES];
    for (int i = 0; i < this->STATE_ENTRIES; i++) {
        G[i] = new double[this->STATE_ENTRIES];
        GT[i] = new double[this->STATE_ENTRIES];
    }
    G[0][0] = 1;
    G[0][1] = 0;
    G[0][2] = -d * sin(thetaT);
    G[1][0] = 0;
    G[1][1] = 1;
    G[1][2] = d * cos(thetaT);
    G[2][0] = 0;
    G[2][1] = 0;
    G[2][2] = 1;

    matrixTranspose(G, GT, this->STATE_ENTRIES, this->STATE_ENTRIES);

    double **L = new double*[this->STATE_ENTRIES];
    double **LT = new double*[this->STATE_ENTRIES];
    for (int i = 0; i < this->STATE_ENTRIES; i++) {
        L[i] = new double[this->STATE_ENTRIES];
        LT[i] = new double[this->STATE_ENTRIES];
    }
    L[0][0] = cos(thetaT);
    L[0][1] = -sin(thetaT);
    L[0][2] = 0;
    L[1][0] = sin(thetaT);
    L[1][1] = cos(thetaT);
    L[1][2] = 0;
    L[2][0] = 0;
    L[2][1] = 0;
    L[2][2] = 1;

    matrixTranspose(L, LT, this->STATE_ENTRIES, this->STATE_ENTRIES);

    double **temp1 = new double*[this->STATE_ENTRIES];
    double **temp2 = new double*[this->STATE_ENTRIES];
    double **temp3 = new double*[this->STATE_ENTRIES];
    double **temp4 = new double*[this->STATE_ENTRIES];
    for (int i = 0; i < this->STATE_ENTRIES; i++) {
        temp1[i] = new double[this->STATE_ENTRIES];
        temp2[i] = new double[this->STATE_ENTRIES];
        temp3[i] = new double[this->STATE_ENTRIES];
        temp4[i] = new double[this->STATE_ENTRIES];
    }

    matrixMultiplication(G, this->P, temp1, this->STATE_ENTRIES, \
            this->STATE_ENTRIES, this->STATE_ENTRIES);
    matrixMultiplication(temp1, GT, temp2, this->STATE_ENTRIES, \
            this->STATE_ENTRIES, this->STATE_ENTRIES);

    matrixMultiplication(L, this->controlCov, temp3, this->STATE_ENTRIES, \
            this->STATE_ENTRIES, this->STATE_ENTRIES);
    matrixMultiplication(temp3, LT, temp4, this->STATE_ENTRIES, \
            this->STATE_ENTRIES, this->STATE_ENTRIES);

    matrixAddition(temp2, temp4, this->P, this->STATE_ENTRIES, \
            this->STATE_ENTRIES);

    for (int i = 0; i < this->STATE_ENTRIES; i++) delete[] G[i];
    delete[] G;
    for (int i = 0; i < this->STATE_ENTRIES; i++) delete[] GT[i];
    delete[] GT;
    for (int i = 0; i < this->STATE_ENTRIES; i++) delete[] L[i];
    delete[] L;
    for (int i = 0; i < this->STATE_ENTRIES; i++) delete[] LT[i];
    delete[] LT;
    for (int i = 0; i < this->STATE_ENTRIES; i++) delete[] temp1[i];
    delete[] temp1;
    for (int i = 0; i < this->STATE_ENTRIES; i++) delete[] temp2[i];
    delete[] temp2;
    for (int i = 0; i < this->STATE_ENTRIES; i++) delete[] temp3[i];
    delete[] temp3;
    for (int i = 0; i < this->STATE_ENTRIES; i++) delete[] temp4[i];
    delete[] temp4;
}

void ExtKalmanFillter::update(const std::vector<double> &measure) {
    assert(this->numLandmarks * 2 == static_cast<int>(measure.size()));

    for (int l = 0; l < this->numLandmarks; l++) {
        double xT = this->X[0], yT = this->X[1], thetaT = this->X[2];
        double lX = this->X[this->STATE_ENTRIES + 2*l];
        double lY = this->X[this->STATE_ENTRIES + 2*l + 1];

        double q = (lX-xT)*(lX-xT) + (lY-yT)*(lY-yT);
        double qSqrt = sqrt(q);

        double **H = new double*[this->NUM_MEAS_TYPE];
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) {
            H[i] = new double[this->numEntries]();
        }
        H[0][0] = (lY - yT) / q;
        H[0][1] = (-lX + xT) / q;
        H[0][2] = -1.0;
        H[1][0] = (-lX + xT) / qSqrt;
        H[1][1] = (-lY + yT) / qSqrt;
        H[1][2] = 0.0;

        H[0][this->STATE_ENTRIES + 2*l] = (-lY + yT) / q;
        H[0][this->STATE_ENTRIES + 2*l + 1] = (lX - xT) / q;
        H[1][this->STATE_ENTRIES + 2*l] = (lX - xT) / qSqrt;
        H[1][this->STATE_ENTRIES + 2*l + 1] = (lY - yT) / qSqrt;

        double **HT = new double*[this->numEntries];
        for (int i = 0; i < this->numEntries; i++) {
            HT[i] = new double[this->NUM_MEAS_TYPE];
        }
        matrixTranspose(H, HT, this->NUM_MEAS_TYPE, this->numEntries);

        double expectedBeta = wrapToPi(atan2(lY-yT, lX-xT) - thetaT);
        double expectedRange = qSqrt;

        double lBeta = measure[2*l];
        double lRange = measure[2*l + 1];

        double **diff = new double*[this->NUM_MEAS_TYPE];
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) diff[i] = new double[1];
        diff[0][0] = lBeta - expectedBeta;
        diff[1][0] = lRange - expectedRange;

        double **temp1 = new double*[this->NUM_MEAS_TYPE];
        double **temp2 = new double*[this->NUM_MEAS_TYPE];
        double **temp3 = new double*[this->NUM_MEAS_TYPE];
        double **temp4 = new double*[this->NUM_MEAS_TYPE];
        double **temp5 = new double*[this->numEntries];
        double **kalmanGain = new double*[this->numEntries];
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) {
            temp1[i] = new double[this->numEntries];
            temp2[i] = new double[this->NUM_MEAS_TYPE];
            temp3[i] = new double[this->NUM_MEAS_TYPE];
            temp4[i] = new double[this->NUM_MEAS_TYPE];
        }
        for (int i = 0; i < this->numEntries; i++) {
            temp5[i] = new double[this->NUM_MEAS_TYPE];
            kalmanGain[i] = new double[this->NUM_MEAS_TYPE];
        }

        matrixMultiplication(H, this->P, temp1, this->NUM_MEAS_TYPE, \
                this->numEntries, this->numEntries);
        matrixMultiplication(temp1, HT, temp2, this->NUM_MEAS_TYPE, \
                this->numEntries, this->NUM_MEAS_TYPE);
        matrixAddition(temp2, this->measurementCov, temp3, \
                this->NUM_MEAS_TYPE, this->NUM_MEAS_TYPE);
        matrix2dInverse(temp3, temp4);
        matrixMultiplication(this->P, HT, temp5, this->numEntries, \
                this->numEntries, this->NUM_MEAS_TYPE);
        matrixMultiplication(temp5, temp4, kalmanGain, this->numEntries, \
                this->NUM_MEAS_TYPE, this->NUM_MEAS_TYPE);

        double **stateUpdate = new double*[this->numEntries];
        for (int i = 0; i < this->numEntries; i++) {
            stateUpdate[i] = new double[1];
        }
        matrixMultiplication(kalmanGain, diff, stateUpdate, this->numEntries, \
                this->NUM_MEAS_TYPE, 1);
        for (int i = 0; i < this->numEntries; i++) {
            this->X[i] += stateUpdate[i][0];
        }

        double **covUpdate = new double*[this->numEntries];
        double **temp6 = new double*[this->numEntries];
        double **temp7 = new double*[this->numEntries];
        for (int i = 0; i < this->numEntries; i++) {
            covUpdate[i] = new double[this->numEntries];
            temp6[i] = new double[this->numEntries];
            temp7[i] = new double[this->numEntries];
        }

        matrixMultiplication(kalmanGain, H, temp6, this->numEntries, \
                this->NUM_MEAS_TYPE, this->numEntries);

        matrixSubtraction(this->eye, temp6, covUpdate, this->numEntries, \
                this->numEntries);

        matrixMultiplication(covUpdate, this->P, temp7, this->numEntries, \
                this->numEntries, this->numEntries);

        for (int i = 0; i < this->numEntries; i++) {
            for (int j = 0; j < this->numEntries; j++) {
                this->P[i][j] = temp7[i][j];
            }
        }

        // -O3 merges most of the following loops.
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) delete[] H[i];
        delete[] H;
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) delete[] diff[i];
        delete[] diff;
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) delete[] temp1[i];
        delete[] temp1;
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) delete[] temp2[i];
        delete[] temp2;
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) delete[] temp3[i];
        delete[] temp3;
        for (int i = 0; i < this->NUM_MEAS_TYPE; i++) delete[] temp4[i];
        delete[] temp4;
        for (int i = 0; i < this->numEntries; i++) delete[] temp5[i];
        delete[] temp5;
        for (int i = 0; i < this->numEntries; i++) delete[] temp6[i];
        delete[] temp6;
        for (int i = 0; i < this->numEntries; i++) delete[] temp7[i];
        delete[] temp7;
        for (int i = 0; i < this->numEntries; i++) delete[] HT[i];
        delete[] HT;
        for (int i = 0; i < this->numEntries; i++) delete[] kalmanGain[i];
        delete[] kalmanGain;
        for (int i = 0; i < this->numEntries; i++) delete[] stateUpdate[i];
        delete[] stateUpdate;
        for (int i = 0; i < this->numEntries; i++) delete[] covUpdate[i];
        delete[] covUpdate;
    }
}

std::vector<double> ExtKalmanFillter::getState() const {
    return std::vector<double>(this->X, this->X + this->numEntries);
}
