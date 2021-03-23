#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include "Problem.h"

using namespace std;

class Solver
{
public:
    Solver(Problem *problem) : problem(problem) {}
    virtual ~Solver() {}
    virtual Solution *solve() = 0;
    Problem *problem;
};

#endif