﻿// --------------------------------------------
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
// Define a few generally useful functions.
// -------------------------------------------------

#include <assert.h>
#include <iostream>
#include <tuple>

#include "kutils.h"
#include "prng.h"

namespace KBase {

  using std::cout;
  using std::endl;
  using std::flush;


  // -------------------------------------------------

  // for historical reasons, the second, quadratic power is called "square"
  double sqr(const double& x) {
    return (x*x);
  }

  // the fourth power is the quartic
  double qrtc(const double& x) {
    return (x*x*x*x);
  }
  
  VUI uiSeq(const unsigned int n1, const unsigned int n2, const unsigned int ns) {
    VUI uis = {};
    assert (n1 <= n2);
    assert (0 < ns);
    for (unsigned int i = n1; i <= n2; i=i+ns) 
      uis.push_back(i);
    return uis;
  }
  
  // -------------------------------------------------

  std::chrono::time_point<std::chrono::system_clock>  displayProgramStart() {
    std::chrono::time_point<std::chrono::system_clock> st;
    st = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(st);
    cout << "  Start time: " << std::ctime(&start_time) << endl << flush;
    return st;
  }

  void displayProgramEnd(std::chrono::time_point<std::chrono::system_clock> st) {
    std::chrono::time_point<std::chrono::system_clock> ft;
    ft = std::chrono::system_clock::now();
    std::chrono::duration<double> eTime = ft - st;
    std::time_t fTime = std::chrono::system_clock::to_time_t(ft);
    cout << "  Finish time: " << std::ctime(&fTime) << endl << flush;
    printf("  Elapsed time: %.4f seconds \n", eTime.count());
    return;
  }


  char* newChars(unsigned int len) {
    auto rslt = new char[len];
    for (unsigned int i = 0; i < len; i++) {
      rslt[i] = ((char)0);
    }
    return rslt;
  };

  double rescale(double x, double x0, double x1, double y0, double y1) {
    assert((x0 < x1) || (x1 < x0));
    const double f = (x - x0) / (x1 - x0);
    return y0 + (y1 - y0)*f;
  }


  // -------------------------------------------------
  KException::KException(string m) {
    msg = m;
  }

  KException::~KException() { 
    msg = "";
  }


}; // namespace


// --------------------------------------------
// Copyright KAPSARC. Open source MIT License.
// --------------------------------------------
