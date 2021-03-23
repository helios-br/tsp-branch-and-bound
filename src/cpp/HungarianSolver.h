#ifndef HUNGARIAN_SOLVER_H
#define HUNGARIAN_SOLVER_H

#include "Solver.h"
#include "Problem.h"

class HungarianSolver : public Solver
{
public:
    HungarianSolver(Problem *problem) : Solver(problem) {}
    ~HungarianSolver() {}
    Solution *solve();
};

#endif