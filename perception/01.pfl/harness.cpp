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

#include <string>
#include <vector>
#include "args.h"
#include "envmap.h"
#include "log.h"
#include "pfilter.h"
#include "rtrbench_utils.h"
#include "zsim_hooks.h"

#define NUM_ODOMETRY_MEASUR (3)     // X, Y, Theta
#define NUM_LASER_MEASUR (180)      // All angles

typedef std::vector<READING> MEASUREMENT_LOG;

void readMeasurements(std::string inputFile, MEASUREMENT_LOG *odometryLog, \
        MEASUREMENT_LOG *laserLog) {
    std::ifstream mFile;
    mFile.open(inputFile);
    assert(mFile.good());

    std::string line;
    while (std::getline(mFile, line)) {
        READING l;

        std::stringstream ss(line);
        double e;
        while (ss >> e) {
            l.push_back(e);
            if (static_cast<int>(l.size()) == NUM_ODOMETRY_MEASUR) {
                odometryLog->push_back(l);
                break;
            }
        }
        l.clear();

        while (ss >> e) {
            l.push_back(e);
        }
        assert(static_cast<int>(l.size()) == NUM_LASER_MEASUR);
        laserLog->push_back(l);
    }
    mFile.close();
}

int main(int argc, const char **argv) {
    using args::Parser;
    using args::KVArg;

    Parser parser(argv[0], argc, argv, false);
    KVArg<std::string> inputMapArg(parser, "map", "", \
            "Input map file");
    KVArg<std::string> measurementsArg(parser, "measurements", "", \
            "Input measurements file");
    KVArg<int> particlesArg(parser, "particles", "", \
            "Number of particles");
    KVArg<int> subsampleArg(parser, "subsample", "", \
            "Subsampling laser readings");
    KVArg<int> maxUpdatesArg(parser, "updates", "", \
            "Maximum number of updates");
    KVArg<std::string> outputArg(parser, "output", "", \
            "Output log file");

    if (!parser.parse()) assert(false);

    assert_msg(inputMapArg.found(), \
            "Input map file is not provided");
    assert_msg(measurementsArg.found(), \
            "Input measurements file is not provided");

    std::string inputMapFile = inputMapArg.value();
    std::string measurementsFile = measurementsArg.value();
    int numParticles = particlesArg.found() ? \
                       particlesArg.value() : 500;
    int subsample = subsampleArg.found() ? \
                    subsampleArg.value() : 1;
    int maxUpdates = maxUpdatesArg.found() ? \
                     maxUpdatesArg.value() : 10'000;
    std::string outputFile = outputArg.found() ? \
                             outputArg.value() : "/dev/null";

    assert(numParticles > 0 && subsample > 0 && maxUpdates >= 2);

    EnvMap *envMap = new EnvMap(inputMapFile);

    MEASUREMENT_LOG *odometryLog = new MEASUREMENT_LOG();
    MEASUREMENT_LOG *laserLog = new MEASUREMENT_LOG();
    readMeasurements(measurementsFile, odometryLog, laserLog);
    assert(odometryLog->size() == laserLog->size());

    ParticleFilter *pf = new ParticleFilter(envMap, numParticles, subsample);
    MEASUREMENT_LOG outputLog;
    outputLog.push_back(pf->getBelief());

    // ROI begins
    zsim_roi_begin();

    for (int i = 1 /*Skip the first reading*/; \
            i < static_cast<int>(odometryLog->size()); i++) {
        READING *prevOdometry = &odometryLog->at(i-1);
        READING *currOdometry = &odometryLog->at(i);
        assert(static_cast<int>(prevOdometry->size()) == NUM_ODOMETRY_MEASUR);
        assert(static_cast<int>(currOdometry->size()) == NUM_ODOMETRY_MEASUR);

        READING *laserReading = &laserLog->at(i);
        assert(static_cast<int>(laserReading->size()) == NUM_LASER_MEASUR);

        pf->updateMotion(prevOdometry, currOdometry);
        pf->updateSensor(laserReading);
        pf->resample();

        outputLog.push_back(pf->getBelief());
        if (i >= maxUpdates) break;
    }

    zsim_roi_end();
    // ROI ends

    // Write the output log
    std::ofstream outLogFile;
    outLogFile.open(outputFile);
    for (auto l : outputLog) {
        for (auto e : l) {
            outLogFile << std::setprecision(4) << e << " ";
        }
        outLogFile << std::endl;
    }
    outLogFile.close();

    delete envMap;
    delete pf;

    return 0;
}
