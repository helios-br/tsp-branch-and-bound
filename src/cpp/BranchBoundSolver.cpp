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
		cout << "%%%% ENTROU ONDE NAO DEVIA" << endl;
		return;
	}

	vector<vector<int>> tours = solution->extractTours();

	// cout << endl
	// 	 << "----" << endl;
	// cout << "---- Problem [" << problem->problemId << "]: new hungarian solution found " << endl;
	// cout << "----" << endl
	// 	 << endl;

	// cout << "# Best value so far: " << this->bestSolution->cost << endl;

	//solution->printSolution(false);

	// Solution feasible found

	if (tours.size() == 1)
	{
		//cout << "# Solution is feasible!" << endl;

		if (solution->cost < this->bestSolution->cost)
		{
			cout << "@ Wow! New upper bound: " << solution->cost << " < " << this->bestSolution->cost << endl;
			delete this->bestSolution;
			this->bestSolution = new Solution(solution);
			this->bounds.upperBound = solution->cost;
		}

		// Bound

		return;
	}
	else
	{
		// if (solution->cost < this->bounds.lowerBound)
		// {
		// 	return;
		// }

		if (solution->cost > this->bounds.upperBound)
		{
			return;
		}

		if (solution->cost > this->bounds.lowerBound && solution->cost < this->bounds.upperBound)
		{
			//cout << "New lower bound: " << solution->cost << " > " << this->bounds.lowerBound << endl;
			this->bounds.lowerBound = solution->cost;
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

	// Creates a new node for each arch of the subtour (blocking)

	//solution->printTours();
	//cout << "Tour index chosen: " << smallestSubtourIndex << endl;

	//cout << "***** blocking moves: ";
	// for (unsigned j = 0; j < tours[smallestSubtourIndex].size() - 1; j++)
	// {
	// 	cout << tours[smallestSubtourIndex][j] << " -> " << tours[smallestSubtourIndex][j + 1] << " | ";
	// }
	// cout << endl;

	delete solution;

	for (unsigned j = 0; j < tours[smallestSubtourIndex].size() - 1; j++)
	{
		Problem *subProblem = copyProblem(problem);
		subProblem->blockMove(tours[smallestSubtourIndex][j], tours[smallestSubtourIndex][j + 1]);
		//printProblem(subProblem, false);
		solveProblem(subProblem);
		delete subProblem;
	}	
}