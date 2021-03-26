#ifndef BEST_BRANCH_BOUND_SOLVER_H
#define BEST_BRANCH_BOUND_SOLVER_H

#include <vector>
#include <string>
#include <list>
#include "Solver.h"
#include "util/CollectionUtil.h"
#include "Problem.h"

using namespace std;

struct NodeBest
{
	int id;
	vector<pair<int, int>> blockedArches;
	bool bounded = false;
	Solution *solution = NULL;
};

class BestBranchBoundSolver : public Solver
{
public:
	BestBranchBoundSolver(Problem *problem) : Solver(problem) {}
	~BestBranchBoundSolver() {}
	Solution *solve();
	Bounds bounds;
	Solution *bestSolution = NULL;

private:
	void solveProblem(Problem *problem);
	Solution *solveNodeProblem(NodeBest *node);
};

#endif