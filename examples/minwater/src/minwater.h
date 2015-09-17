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
// --------------------------------------------

#ifndef DEMO_WATERMIN_H
#define DEMO_WATERMIN_H

#include <assert.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tuple>
#include <vector>

#include "kutils.h"
#include "prng.h"
#include "kmatrix.h"
#include "gaopt.h"
#include "hcsearch.h"
#include "kmodel.h"

namespace DemoWaterMin {
  // namespace to which KBase has no access

  using std::string;
  using std::tuple;
  using std::vector;

  using KBase::KMatrix;
  using KBase::PRNG;

  using KBase::Actor;
  using KBase::Position;
  using KBase::State;
  using KBase::Model;
  using KBase::VotingRule;
  using KBase::ReportingLevel;

  /// Define a hard-coded scenario
  const unsigned int numA = 4;
  const unsigned int numP = 4;

  const double uArray[] = { // numA x numP
    0.0000, 0.8383, 0.7532, 0.9660,
    1.0000, 1.0000, 1.0000, 0.7500,
    1.0000, 0.0000, 0.7500, 0.0000,
    0.0000, 0.6107, 0.3407, 0.9027
  };

  const double pArray[] = { 0.10, 0.80, 0.05, 0.05 }; // numP

  const KMatrix uInit = KMatrix::arrayInit(uArray, numA, numP);
  const KMatrix pInit = KMatrix::arrayInit(pArray, numP, 1);

}; // end of namespace

// -------------------------------------------------
#endif

// --------------------------------------------
// Copyright KAPSARC. Open source MIT License.
// --------------------------------------------
