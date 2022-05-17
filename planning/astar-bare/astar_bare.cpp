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

#include "astar_bare.h"
#include <chrono>
#include <limits.h>
#include <vector>
#include "parallel_hashmap/phmap.h"
#include "rtrbench_utils.h"

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;

AStar::AStar(STATE initial, STATE goal, double hWeight) {
    this->initState = initial;
    this->goalState = goal;
    this->heuristicWeight = hWeight;

    assert(initial.size() == goal.size());
}

PATH AStar::run(uint64_t maxExpansions) {
    MIN_HEAP openList;
    BaseEPS<STATE> *visited = nullptr;
    if (this->initState.size() <= 3) {
        visited = new ShortEPS<STATE>(this->MAX_STATE_COORDINATE_BITS);
    } else {
        visited = new LongEPS<STATE>();
    }
    BaseEPM<STATE, double> *gVals = nullptr;
    if (this->initState.size() > 3) {
        gVals = new LongEPM<STATE, double>();
    } else {
        gVals = new ShortEPM<STATE, double>(this->MAX_STATE_COORDINATE_BITS);
    }

    Node *startNode = new Node(this->initState, 0, this->heuristicWeight *\
            this->getH(this->initState), NULL, -1);
    openList.push(startNode);

    auto baseTime = high_resolution_clock::now();
    uint64_t numExpansions = 0;
    while (!openList.empty()) {
        Node *expNode = openList.top();
        openList.pop();

        for (auto e : expNode->s) {
            assert(e < (1 << this->MAX_STATE_COORDINATE_BITS));
        }

        if (visited->contains(expNode->s)) continue;
        visited->insert(expNode->s);

        if (++numExpansions >= maxExpansions) break;

        if (this->isGoal(expNode->s)) {
            assert(numExpansions <= maxExpansions);
            auto t = high_resolution_clock::now();
            this->execTime = duration_cast<nanoseconds>\
                             (t - baseTime).count() * 1e-9;
            return this->getPath(expNode);
        }

        std::vector<STATE> neighbors = this->getNeighbors(expNode->s);
        // NOTE: If expansion is "slow" (e.g., costly collision detection),
        // uncomment the OMP primitives to effectively parallelize the loop.
        // The data structures are race-free.

        // #pragma omp parallel for ordered
        for (int dir = 0; dir < static_cast<int>(neighbors.size()); dir++) {
            STATE neighborState = neighbors[dir];
            assert(neighborState.size() == this->initState.size());
            if (visited->contains(neighborState)) continue;

            if (this->isValid(neighborState)) {
                double g = this->getG(expNode->s, neighborState, \
                        expNode->g, dir);
                double f = g + this->heuristicWeight * \
                           this->getH(neighborState);

                // #pragma omp ordered
                if (!gVals->contains(neighborState) || \
                        g < gVals->getValue(neighborState)) {
                    gVals->insert(neighborState, g);
                    openList.push(new Node(neighborState, g, f, expNode, dir));
                }
            }
        }
    }

    assert(numExpansions <= maxExpansions);
    auto t = high_resolution_clock::now();
    this->execTime = duration_cast<nanoseconds>(t - baseTime).count() * 1e-9;

    delete visited;
    delete gVals;
    delete startNode;

    return PATH();
}

double AStar::getExecTime() const {
    return this->execTime;
}

PATH AStar::getPath(Node *n) const {
    PATH path;

    while (n) {
        path.push_back(n->dir);
        n = n->parent;
    }

    path.pop_back();    // dir is meaningless for the start node
    std::reverse(path.begin(), path.end());

    return path;
}

bool AStar::isGoal(const STATE &s) const {
    return s == this->goalState;
}
