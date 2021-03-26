#include "BreadthBranchBoundSolver.h"

#include <iostream>
#include <vector>
#include "Problem.h"
#include "HungarianSolver.h"
#include "InitialSolver.h"
#include "util/OutputUtil.h"

using namespace std;

Solution *BreadthBranchBoundSolver::solve()
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

void BreadthBranchBoundSolver::solveProblem(Problem *problem)
{
	Node firstNode;
	list<Node> queue;
	queue.push_back(firstNode);

	while (queue.size() > 0)
	{
		Node node = queue.front();
		Solution *solution = solveNodeProblem(&node);

		if (!node.bounded)
		{
			vector<vector<int>> tours = solution->extractTours();

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

			for (unsigned j = 0; j < tours[smallestSubtourIndex].size() - 1; j++)
			{
				Node newNode;
				newNode.blockedArches = node.blockedArches;
				pair<int, int> arch(tours[smallestSubtourIndex][j], tours[smallestSubtourIndex][j + 1]);
				newNode.blockedArches.push_back(arch);
				queue.push_back(newNode);
			}
		}

		delete solution;
		queue.pop_front();
	}
}

Solution *BreadthBranchBoundSolver::solveNodeProblem(Node *node)
{
	Problem *subProblem = createProblemBlockingNodes(this->problem, node->blockedArches);
	HungarianSolver *hungarianSolver = new HungarianSolver(subProblem);
	Solution *solution = hungarianSolver->solve();
	delete hungarianSolver;
	delete subProblem;

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

			if (this->bounds.lowerBound > this->bounds.upperBound)
			{
				logn("# Reseting lowerbound!");
				this->bounds.lowerBound = this->bounds.lowestBound;
			}
		}

		node->bounded = true;
	}
	else
	{
		// Verify upper bound

		if (solution->cost > this->bounds.upperBound)
		{
			node->bounded = true;
		}
	}

	return solution;
}