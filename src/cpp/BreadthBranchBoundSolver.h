#ifndef BREADTH_BRANCH_BOUND_SOLVER_H
#define BREADTH_BRANCH_BOUND_SOLVER_H

#include <vector>
#include <string>
#include <list>
#include "Solver.h"
#include "util/CollectionUtil.h"
#include "Problem.h"

using namespace std;

struct Node
{
	vector<pair<int, int>> blockedArches;
	bool bounded = false;
};

class BreadthBranchBoundSolver : public Solver
{
public:
	BreadthBranchBoundSolver(Problem *problem) : Solver(problem) {}
	~BreadthBranchBoundSolver() {}
	Solution *solve();
	Bounds bounds;
	Solution *bestSolution = NULL;

private:
	void solveProblem(Problem *problem);
	Solution *solveNodeProblem(Node *node);
};

#endif