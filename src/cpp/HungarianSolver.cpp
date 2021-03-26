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
	hungarian_init(&p, problem->cost, dimension, dimension, HUNGARIAN_MODE_MINIMIZE_COST);
	double cost = hungarian_solve(&p);
	Solution *solution = new Solution(problem, p.assignment, cost);

	if (cost != solution->cost) {
		nlogn("[ERROR] Solution cost (" + to_string(cost) + ") differs from hungarian algorithm cost (" + to_string(solution->cost) + ")");
		exit(0);
	}

	custom_hungarian_free(&p);
	return solution;
};