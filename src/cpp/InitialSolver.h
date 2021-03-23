#ifndef SORTABLE_H
#define SORTABLE_H

#include <vector>
#include <string>
#include "Solver.h"

using namespace std;

class InitialSolver : public Solver
{
public:
    InitialSolver(Problem *problem) : Solver(problem) {}
    ~InitialSolver() {}
    Solution *solve();
};

#endif