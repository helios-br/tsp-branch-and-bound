#include "BranchBoundSolver.h"

#include <iostream>
#include <vector>
#include "Problem.h"
#include "HungarianSolver.h"
#include "InitialSolver.h"
#include "util/OutputUtil.h"

using namespace std;

Solution *BranchBoundSolver::solve()
{
	nlogn("---- Solver: Branch and Bound");

	Problem *originalProblem = this->problem;

	/** Creates initial solution for the problem */

	InitialSolver *initialSolver = new InitialSolver(originalProblem);
	Solution *initialSolution = initialSolver->solve();
	delete initialSolver;
	nlogn("Initial solution found...");	
	initialSolution->printSolution(false);

	// Sets upper bound

	this->bounds.upperBound = initialSolution->cost;
	this->bestSolution = initialSolution;

	/** Branch and bound algorithm */

	logDashes();

	solveProblem(originalProblem);

	return this->bestSolution;
};

void BranchBoundSolver::solveProblem(Problem *problem)
{
	HungarianSolver *hungarianSolver = new HungarianSolver(problem);
	Solution *solution = hungarianSolver->solve();
	delete hungarianSolver;

	if (!solution)
	{
		nlogn("[ERROR] HUNGARIAN ALGORITHM RETURNED AN EMPTY SOLUTION");
		exit(0);
	}

	// Lowest possible bound

	if (this->bounds.lowestBound == -1)
	{
		this->bounds.lowestBound = solution->cost;
	}

	vector<vector<int>> tours = solution->extractTours();

	// Check whether a feasible solution was found

	if (tours.size() == 1)
	{
		// logn("# Solution is feasible!");

		if (solution->cost == this->bounds.lowestBound)
		{
			nlogn("@ Amazing! Best solution found using lower bound!");
			solution->printSolution(true);
			exit(0);
		}

		if (solution->cost < this->bestSolution->cost)
		{
			logn("@ Wow! New best solution: " + to_string(solution->cost) + " < " + to_string(this->bestSolution->cost));
			delete this->bestSolution;
			this->bestSolution = new Solution(solution);
			this->bounds.upperBound = solution->cost;
		}

		delete solution;

		// Bound

		return;
	}
	else
	{
		// Verify upper bound

		if (solution->cost > this->bounds.upperBound)
		{
			delete solution;
			return;
		}
	}

	// Finds the smaller subtour from the solution

	int smallestSubtourIndex = -1;
	int smallestSubtourSize = -1;

	for (unsigned i = 0; i < tours.size(); i++)
	{
		int subtourSize = tours[i].size();

		if (smallestSubtourSize == -1 || subtourSize < smallestSubtourSize)
		{
			smallestSubtourIndex = i;
			smallestSubtourSize = subtourSize;
		}
		else if (subtourSize == smallestSubtourSize && tours[i][0] < tours[smallestSubtourIndex][0])
		{
			smallestSubtourIndex = i;
		}
	}

	delete solution;

	for (unsigned j = 0; j < tours[smallestSubtourIndex].size() - 1; j++)
	{
		Problem *subProblem = copyProblem(problem);
		subProblem->blockMove(tours[smallestSubtourIndex][j], tours[smallestSubtourIndex][j + 1]);;
		solveProblem(subProblem);
		delete subProblem;
	}
}