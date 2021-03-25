#ifndef BRANCH_BOUND_SOLVER_H
#define BRANCH_BOUND_SOLVER_H

#include <vector>
#include <string>
#include "Solver.h"
#include "util/CollectionUtil.h"
#include "Problem.h"

using namespace std;

class BranchBoundSolver : public Solver
{
public:
	BranchBoundSolver(Problem *problem) : Solver(problem) {}
	~BranchBoundSolver() {}
	Solution *solve();
	Bounds bounds;
	Solution *bestSolution = NULL;

private:
	void solveProblem(Problem *problem);
};

#endif