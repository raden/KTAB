//---------------------------------------------
// Copyright Ben Paul Wise. All Rights Reserved.
//---------------------------------------------
// start of a simple Tetris clone
//---------------------------------------------
#ifndef TETRIS_MAIN_H
#define TETRIS_MAIN_H

#include "tutils.h"
#include "tcanvas.h"
#include "board.h"

//---------------------------------------------

using std::cout;
using std::endl;
using std::flush;

using KBase::PRNG;
using KGraph::Canvas;

//---------------------------------------------
namespace Tetris {

char* newChar (unsigned int buffLen);
enum SchemeShapes {SS_GameBoy, SS_Gerasimov, SS_Sega, SS_SovietMG, SS_TetrisCo };
enum SchemeWindows {SW_Black, SW_White, SW_Beige};


class ControlState {
public:
    ControlState() { // factory-defined defaults
        bg = 1;
        pc = 2;
        gt = 1;
        rt = 0;
    };
    ControlState(unsigned int bv, unsigned int pv, unsigned int gv, unsigned int rv) {
        bg = bv;
        pc = pv;
        gt = gv;
        rt = rv;
    }; 

    unsigned int bg = 1; // factory-defined default
    unsigned int pc = 2; // factory-defined default
    unsigned int gt = 1; // factory-defined default
    unsigned int rt = 0; // factory-defined default
};


class TApp {
public:
    explicit TApp(uint64_t s);
    void run();
    virtual ~TApp();

    void newGame(); // w/o resetting any parameters
    void stepGame();
    void pause(); // w/o resetting any parameters
    void resume(double delay); // w/o resetting any parameters

    static TApp* theApp;

    Fl_Color color(unsigned int i) const;
    unsigned int level = 3; // 1 is slowest, 10 is fastest
    // The timings are set so that at level 1,
    // it takes 10-15 seconds to fall to the bottom,
    // while at level 10 it takes 1 second.
    // Same percentage speed up with each step.

    void setRC(unsigned int r, unsigned int c);
    void setLevel(unsigned int lvl);
    void setRandom(bool rp);

    double dt = 0.1;
    PRNG* rng = nullptr;
    Board* board = nullptr;
    unsigned int rows = 0;
    unsigned int clms = 0;
    bool randomPlacement = false;
    double setDt(); // compute time step for current levelfile:///home/bwise/GitBox/personal/ftetris/src/tmain.h
    void applyControlState(ControlState cs);
    void applyColorScheme(ControlState cs);

    void processKey(int x, int y, int k); // a fairly important function
    void quit();

    double playTime = 5*60;
    double maxPlayTime = 10*60;

    bool paused = true;
protected:
    unsigned int lineCount = 0;
    unsigned int score = 0;
    const unsigned int defaultLevel = 3;
    const unsigned int defaultRows = 24;
    const unsigned int defaultClms = 12;
    const bool defaultRP = false;

    vector<Fl_Color> colors = {}; // these 10 colors define the current color scheme
    unsigned int scoreFn(unsigned int clc);

    void setColorScheme();
    
private:
};


}; // end of namespace

//---------------------------------------------
#endif
//---------------------------------------------
// Copyright Ben Paul Wise. All Rights Reserved.
//---------------------------------------------
