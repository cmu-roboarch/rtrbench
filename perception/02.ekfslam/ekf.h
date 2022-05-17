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

#include <vector>
#include "log.h"

class ExtKalmanFillter {
    public:
        ExtKalmanFillter(double sigX2, double sigY2, double sigAlpha2, \
                double sigBeta2, double sigR2, \
                std::vector<double> initialMeasurement);
        ~ExtKalmanFillter();

        void predict(double d, double alpha);
        void update(const std::vector<double> &measure);
        std::vector<double> getState() const;

    private:
        double *X;
        double **P;
        double **controlCov, **measurementCov;
        double **eye;
        int numLandmarks;

        const int STATE_ENTRIES = 3;    // X, Y, Theta
        const int NUM_MEAS_TYPE = 2;    // Range and bearing angle
        int numEntries;     // Total entries representing robot and landmarks
};
