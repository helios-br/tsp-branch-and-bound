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
	cout << endl
		 << "---- Solver: Branch and Bound" << endl;

	Problem *originalProblem = this->problem;

	/** Creates initial solution for the problem */

	InitialSolver *initialSolver = new InitialSolver(originalProblem);
	Solution *initialSolution = initialSolver->solve();
	delete initialSolver;
	cout << endl
		 << "Initial solution found..." << endl;
	initialSolution->printSolution(false);

	// Sets upper bound

	this->bounds.upperBound = initialSolution->cost;
	this->bestSolution = initialSolution;

	/** Branch and bound algorithm */

	solveProblem(originalProblem);

	return this->bestSolution;
};

void BranchBoundSolver::solveProblem(Problem *problem)
{
	HungarianSolver *hungarianSolver = new HungarianSolver(problem);
	logn("1");
	Solution *solution = hungarianSolver->solve();
	logn("2");
	vector<vector<int>> tours = solution->extractTours();
	logn("3");

	cout << endl
		 << "-- Problem [" << problem->problemId << "]: new hungarian solution found " << endl;
	cout << "# Best value so far: " << this->bestSolution->cost << endl;

	solution->printSolution(false);

	// Validates new solution against bounds

	// if (solution->cost > this->bounds.upperBound)
	// {
	// 	cout << solution->cost << " > " << this->bounds.upperBound << endl;
	// 	return;
	// }

	// if (solution->cost < this->bounds.lowerBound)
	// {
	// 	cout << "LowerBound validation failure: " << solution->cost << " < " << this->bounds.lowerBound << endl;
	// 	return;
	// }

	// Solution feasible

	if (tours.size() == 1)
	{
		cout << "Solution is feasible" << endl;
		if (solution->cost < this->bestSolution->cost)
		{
			cout << "New upper bound: " << solution->cost << " < " << this->bestSolution->cost << endl;
			delete this->bestSolution;
			this->bestSolution = new Solution(solution);
			this->bounds.upperBound = this->bestSolution->cost;
		}
		//return;
	}
	// else
	// {
	// 	if (solution->cost > this->bounds.lowerBound && solution->cost < this->bestSolution->cost)
	// 	{
	// 		cout << "New lower bound: " << solution->cost << " > " << this->bounds.lowerBound << endl;
	// 		this->bounds.lowerBound = solution->cost;
	// 	}
	// }

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

	// Creates a new node for each arch of the subtour (blocking)

	cout << "Tour index chosen: " << smallestSubtourIndex << endl;

	for (unsigned j = 0; j < tours[smallestSubtourIndex].size() - 2; j++)
	{
		Problem *subProblem = copyProblem(solution->problem);
		cout << "a" << endl;
		subProblem->blockMove(tours[smallestSubtourIndex][j], tours[smallestSubtourIndex][j + 1]);
		cout << "b" << endl;
		solveProblem(subProblem);
		cout << "c" << endl;
		delete subProblem;
		cout << "d" << endl;
	}

	delete solution;
}