#include "HungarianSolver.h"

#include <iostream>
#include <vector>
#include "external/Hungarian.h"
#include "Problem.h"
#include "util/OutputUtil.h"

using namespace std;

Solution *HungarianSolver::solve()
{
	Problem *problem = this->problem;
	int dimension = problem->dimension;

	hungarian_problem_t p;
	logn("hung0");
	hungarian_init(&p, problem->cost, dimension, dimension, HUNGARIAN_MODE_MINIMIZE_COST);
	logn("hung1");
	hungarian_solve(&p);
	logn("hung2");
	Solution *solution = new Solution(problem, copyIntMatrix(p.assignment, dimension));
	logn("hung3");

	hungarian_free(&p);

	return solution;
};