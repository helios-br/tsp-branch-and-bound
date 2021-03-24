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
	if (p.problem)
	{
		hungarian_free(&p);
		return NULL;
	}
	double cost = hungarian_solve(&p);
	if (p.problem)
	{
		hungarian_free(&p);
		return NULL;
	}
	Solution *solution = new Solution(problem, p.assignment, cost);
	if (cost != solution->cost) {
		cout << "++++++++++++++++++++ VIGE";
		cout << "cost: " << cost << ", solution-cost: " << solution->cost << endl;
		exit(0);
	}
	custom_hungarian_free(&p);
	return solution;
};