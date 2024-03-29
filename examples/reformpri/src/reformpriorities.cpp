// --------------------------------------------
// Copyright KAPSARC. Open source MIT License.
// --------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2015 King Abdullah Petroleum Studies and Research Center
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// -------------------------------------------------
// Define the functions and methods unique to the Priority case.
// -------------------------------------------------

#include "reformpriorities.h"
#include <tuple>

namespace RfrmPri {
// namespace to hold everything related to the
// "priority of reforms" CDMP. Note that KBase has no access.

using namespace std;

using KBase::KMatrix;
using KBase::PRNG;
using KBase::VUI;

// -------------------------------------------------
// function definitions


vector<VUI> scanPositions(const RPModel * rpm) {
    unsigned int numA = rpm->numAct;
    unsigned int numRefItem = rpm->numItm;
    assert(numRefItem == rpm->numCat);

    printf("There are %u actors and %u reform items \n", numA, numRefItem);

    cout << "Computing positions ... " << endl;
    vector<VUI> positions; // list of all positions
    VUI pstn;
    // build the first permutation: 1,2,3,...
    for (unsigned int i = 0; i < numRefItem; i++) {
        pstn.push_back(i);
    }
    positions.push_back(pstn);
    while (next_permutation(pstn.begin(), pstn.end())) {
        positions.push_back(pstn);
    }
    const unsigned int numPos = positions.size();
    cout << "For " << numRefItem << " reform items there are " << numPos << " positions" << endl;

    cout << "Computing utilities of positions ... " << endl;
    cout << "Effective gov costs:" << endl;
    (rpm->govCost).mPrintf("%.3f ");
    cout << endl;
    KMatrix uij = KMatrix(numA, numPos);
    for (unsigned int ai = 0; ai < numA; ai++) {
        for (unsigned int pj = 0; pj < numPos; pj++) {
            auto pstn = positions[pj];
            double uip = rpm->utilActorPos(ai, pstn);
            assert(0 < uip);
            uij(ai, pj) = uip;
        }
    }

    cout << "Computing best position for each actor"<<endl;
    vector<VUI> bestAP; // list of each actor's best position (followed by CP)
    for (unsigned int ai = 0; ai < numA; ai++) {
        unsigned int bestJ = 0;
        double bestV = 0;
        for (unsigned int pj = 0; pj < numPos; pj++) {
            if (bestV < uij(ai,pj)) {
                bestJ = pj;
                bestV = uij(ai,pj);
            }
        }
        printf("Best for %02u is ", ai);
        printPerm(positions[bestJ]);
        cout << endl;
        bestAP.push_back(positions[bestJ]);
    }

    cout << "Computing zeta ... " << endl;
    KMatrix aCap = KMatrix(1, numA);
    for (unsigned int i = 0; i < numA; i++) {
        auto ri = ((const RPActor *)(rpm->actrs[i]));
        aCap(0, i) = ri->sCap;
    }
    KMatrix zeta = aCap * uij;
    assert((1 == zeta.numR()) && (numPos == zeta.numC()));


    cout << "Sorting positions from most to least net support ..." << endl << flush;

    auto betterPR = [](tuple<unsigned int, double, VUI> pr1,
    tuple<unsigned int, double, VUI> pr2) {
        double v1 = get<1>(pr1);
        double v2 = get<1>(pr2);
        bool better = (v1 > v2);
        return better;
    };

    auto pairs = vector<tuple<unsigned int, double, VUI>>();
    for (unsigned int i = 0; i < numPos; i++) {
        auto pri = tuple<unsigned int, double, VUI>(i, zeta(0, i), positions[i]);
        pairs.push_back(pri);
    }

    sort(pairs.begin(), pairs.end(), betterPR);

    const unsigned int maxDisplayed = 25;
    unsigned int numPr = pairs.size();
    if (maxDisplayed < numPr) {
        numPr = maxDisplayed;
    }
    cout << "Displaying highest " << numPr << endl << flush;
    for (unsigned int i = 0; i < numPr; i++) {
        auto pri = pairs[i];
        unsigned int ni = get<0>(pri);
        double zi = get<1>(pri);
        VUI pi = get<2>(pri);

        printf("%3u: %4u  %.2f  ", i, ni, zi);
        printPerm(pi);
        cout << endl << flush;
    }

    VUI bestPerm = get<2>(pairs[0]);

    bestAP.push_back(bestPerm);
    return bestAP;
}

} // end of namespace

int main(int ac, char **av) {
    using KBase::ReportingLevel;
    using KBase::PRNG;
    using KBase::MtchPstn;

    using RfrmPri::RPModel;
    using RfrmPri::RPState;
    using RfrmPri::RPActor;
    using RfrmPri::printPerm;

    auto sTime = KBase::displayProgramStart();
    uint64_t dSeed = 0xD67CC16FE69C185C; // arbitrary
    uint64_t seed = dSeed;
    bool siP = false;
    bool cpP = false;
    bool runP = true;
    unsigned int sNum = 1;

    auto showHelp = [dSeed, sNum]() {
        printf("\n");
        printf("Usage: specify one or more of these options\n");
        printf("\n");
        printf("--cp              start all actors from the central position \n");
        printf("--si              start each actor from their most self-interested position \n");
        printf("                  Note: either CP or SI must be indicated. \n");
        printf("--help            print this message and exit \n");
        printf("--seed <n>        set a 64bit seed \n");
        printf("                  0 means truly random \n");
        printf("                  default: %020llu \n", dSeed);
        printf("--sNum <n>        choose a scenario nnumber \n");
        printf("                  default: %u \n", sNum);
    };



    // a list of <keyword, description, lambda-fn>
    // might be enough to do this - except for the arguments to options.
    if (ac > 1) {
        for (int i = 1; i < ac; i++) {
            if (strcmp(av[i], "--seed") == 0) {
                i++;
                seed = std::stoull(av[i]);
            }
            else if (strcmp(av[i], "--sNum") == 0) {
                i++;
                sNum = atoi(av[i]);
            }
            else if (strcmp(av[i], "--cp") == 0) {
                cpP = true;
            }
            else if (strcmp(av[i], "--si") == 0) {
                siP = true;
            }
            else if (strcmp(av[i], "--help") == 0) {
                runP = false;
            }
            else {
                runP = false;
                printf("Unrecognized argument: %s\n", av[i]);
            }
        }
    }

    if (!((siP && !cpP) || (cpP && !siP))) {
        runP = false;
        printf("Exactly one of either --siP or --cpP must be specified\n");
    }

    if (!runP) {
        showHelp();
        return 0;
    }


    PRNG * rng = new PRNG();
    seed = rng->setSeed(seed); // 0 == get a random number
    printf("Using PRNG seed:  %020llu \n", seed);
    printf("Same seed in hex:   0x%016llX \n", seed);
    // Unix correctly prints all digits with lu, lX, llu, and llX.
    // Windows only prints part, with lu, lX, llu, and llX.


    auto rpm = new RPModel(rng);
    switch (sNum) {
    case 0:
        rpm->initScen(0);
        break;
    case 1:
    default:
        rpm->initScen(1);
        break;
    }

    unsigned int numA = rpm->numAct; // the actors are as yet unnamed
    unsigned int numR = rpm->numItm;

    auto rps0 = new RPState(rpm);
    rpm->addState(rps0);

    auto pstns = RfrmPri::scanPositions(rpm);
    KBase::VUI bestPerm = pstns[numA];
    assert(numR == bestPerm.size());

    // Either start them all at the CP or have each to choose an initial position which
    // maximizes their direct utility, regardless of expected utility.
    for (unsigned int i = 0; i < numA; i++) {
        auto pi = new  MtchPstn();
        pi->numCat = numR;
        pi->numItm = numR;
        if (cpP) {
            pi->match =  bestPerm;
        }
        if (siP) {
            pi->match =  pstns[i];
        }
        rps0->addPstn(pi);
    }
    assert(numA == rps0->pstns.size());

    rps0->step = [rps0]() {
        return rps0->stepSUSN();
    };
    unsigned int maxIter = 100;
    rpm->stop = [maxIter,rpm](unsigned int iter, const KBase::State * s) {
        bool doneP = iter > maxIter;
        if (doneP) {
            printf("Max iteration limit of %u exceeded \n", maxIter);
        }
        auto s2 = ((const RPState *)(rpm->history[iter]));
        for (unsigned int i = 0; i < iter; i++) {
            auto s1 = ((const RPState *)(rpm->history[i]));
            if (RPModel::equivStates(s1, s2)) {
                doneP = true;
                printf("State number %u matched state number %u \n", iter, i);
            }
        }

        return doneP;
    };

    rps0->setUENdx();
    rpm->run();

    // we already displayed each state as it was processed,
    // so there is no need to show it again
    //rpm->showHist();


    delete rpm; // and actors, and states
    rpm = nullptr;
    rps0 = nullptr;

    delete rng;
    rng = nullptr;

    KBase::displayProgramEnd(sTime);

    return 0;
}


// --------------------------------------------
// Copyright KAPSARC. Open source MIT License.
// --------------------------------------------
