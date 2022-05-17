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

#include <fstream>
#include <string>
#include "args.h"
#include "sdcar.h"
#include "zsim_hooks.h"

int main(int argc, const char **argv) {
    using args::Parser;
    using args::KVArg;

    Parser parser(argv[0], argc, argv, false);
    KVArg<std::string> inputMapArg(parser, "map", "", \
            "Input map file");
    KVArg<int> gridConnectionArg(parser, "grid", "", \
            "Grid connectivity [4, 8]");
    KVArg<double> heuristicWeightArg(parser, "weight", "", \
            "Heuristic weight of A*");
    KVArg<std::string> aStarHeuristicArg(parser, "heuristic", "", \
            "A* heuristic");
    KVArg<int> scaleMapArg(parser, "scale-map", "", \
            "Map scale factor");
    KVArg<int> scaleRobotArg(parser, "scale-robot", "", \
            "Robot scale factor");
    KVArg<uint64_t> maxExpansionsArg(parser, "max-exps", "", \
            "Maximum expansions");
    KVArg<std::string> outputPathArg(parser, "output", "", \
            "Output path file");

    if (!parser.parse()) assert(false);

    assert_msg(inputMapArg.found(), \
            "Input map file is not provided");

    std::string inputFile = inputMapArg.value();
    int numDirections = gridConnectionArg.found() ? \
                        gridConnectionArg.value() : 8;
    double heuristicWeight = heuristicWeightArg.found() ? \
                             heuristicWeightArg.value() : 1.0;
    std::string aStarHeuristicStr = aStarHeuristicArg.found() ? \
                                    aStarHeuristicArg.value() : "Euclidean";
    int scaleMap = scaleMapArg.found() ? \
                   scaleMapArg.value() : 1;
    int scaleRobot = scaleRobotArg.found() ? \
                     scaleRobotArg.value() : 1;
    uint64_t maxExpansions = maxExpansionsArg.found() ? \
                             maxExpansionsArg.value() : 10'000'000;
    std::string outputFile = outputPathArg.found() ? \
                             outputPathArg.value() : "/dev/null";

    STATE startPos = {0 /*X*/, 0 /*Y*/};
    STATE goalPos = {965 /*X*/, 980 /*Y*/};

    int robotLength = 10, robotWidth = 4;   // Numbers in resolution unit

    // Scaling the robot size
    robotLength *= scaleRobot;
    robotWidth *= scaleRobot;

    SelfDrivingCar sdCar(inputFile, scaleMap, numDirections, startPos, \
            goalPos, robotLength, robotWidth, aStarHeuristicStr, \
            heuristicWeight);

    // ROI begins
    zsim_roi_begin();

    auto path = sdCar.run(maxExpansions);

    zsim_roi_end();
    // ROI ends

    // Write the output path
    std::ofstream pathFile;
    pathFile.open(outputFile);
    for (auto dir : path) {
        pathFile << dir << std::endl;
    }
    pathFile.close();

    return 0;
}
