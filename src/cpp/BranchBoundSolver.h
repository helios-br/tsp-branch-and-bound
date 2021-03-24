#ifndef BRANCH_BOUND_SOLVER_H
#define BRANCH_BOUND_SOLVER_H

#include <vector>
#include <string>
#include "Solver.h"
#include "util/CollectionUtil.h"
#include "Problem.h"

using namespace std;

struct Node
{
	int id;
	Problem *problem;
	Solution *solution;

	Node(Solution *solution)
	{
		this->id = solution->problem->problemId;
		this->solution = solution;
		this->problem = solution->problem;
	}

	~Node()
	{
		delete solution;
		delete problem;
	}
};

struct Bounds
{
	double lowerBound = -1;
	double upperBound = GLOBAL_INFINITE_COST;
};

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