#include "BestBranchBoundSolver.h"

#include <iostream>
#include <vector>
#include "Problem.h"
#include "HungarianSolver.h"
#include "InitialSolver.h"
#include "util/OutputUtil.h"

using namespace std;

Solution *BestBranchBoundSolver::solve()
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

	//this->bounds.upperBound = initialSolution->cost;
	this->bounds.upperBound = GLOBAL_INFINITE_COST;
	this->bestSolution = initialSolution;

	/** Branch and bound algorithm */

	logDashes();

	solveProblem(originalProblem);

	return this->bestSolution;
};

void BestBranchBoundSolver::solveProblem(Problem *problem)
{
	int nodeIdSequence = 0;
	NodeBest firstNode;
	firstNode.id = ++nodeIdSequence;
	firstNode.solution = solveNodeProblem(&firstNode);
	list<NodeBest> queue{firstNode};

	while (queue.size() > 0)
	{
		//cout << "----------------- NEW ITERATION -----------------" << endl;
		//cout << "Queue size: " << queue.size() << ". Best cost: " << this->bounds.upperBound << endl;
		auto bestIterator = queue.begin();
		for (auto iterator = queue.begin(); iterator != queue.end(); iterator++)
		{
			NodeBest item = *iterator;
			NodeBest bestItem = *bestIterator;
			//cout << "[" << item.id << ": " << item.solution->cost << "] ";
			if (item.solution->cost < bestItem.solution->cost)
			{
				bestIterator = iterator;
			}
		}

		NodeBest node = *bestIterator;

		//cout << endl << "Best node chosen: " << node.id << endl;

		Solution *solution = node.solution;
		vector<vector<int>> tours = solution->extractTours();

		// Finds the smaller subtour from the solution

		int smallestSubtourIndex = -1;
		int smallestSubtourSize = -1;

		//solution->printTours();

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
			NodeBest newNode;
			newNode.id = ++nodeIdSequence;
			newNode.blockedArches = node.blockedArches;
			pair<int, int> arch(tours[smallestSubtourIndex][j], tours[smallestSubtourIndex][j + 1]);
			newNode.blockedArches.push_back(arch);

			Solution *newSolution = solveNodeProblem(&node);
			if (!node.bounded)
			{
				//cout << "New node id:  " << newNode.id << endl;
				newNode.solution = newSolution;
				queue.push_back(newNode);
			}
			else
			{
				cout << "Deleted node id:  " << newNode.id << endl;
				delete newSolution;
			}
		}

		delete solution;
		queue.erase(bestIterator);
	}
}

Solution *BestBranchBoundSolver::solveNodeProblem(NodeBest *node)
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

	vector<vector<int>> tours = solution->extractTours();

	// Check whether a feasible solution was found

	if (tours.size() == 1)
	{
		cout << "upper bound: " << this->bounds.upperBound << ", solution cost: " << solution->cost << endl;
		if (solution->cost < this->bounds.upperBound)
		{
			logn("@ Wow! New best solution: " + to_string(solution->cost) + " < " + to_string(this->bounds.upperBound));
			double infinite = GLOBAL_INFINITE_COST;
			if (this->bounds.upperBound != infinite)
			{
				delete this->bestSolution;
			}
			this->bestSolution = new Solution(solution);
			this->bounds.upperBound = solution->cost;
		}

		node->bounded = true;
	}
	else
	{
		// Verify upper bound

		if (solution->cost > this->bounds.upperBound)
		{
			node->bounded = true;
			//cout << "Not feasible. Higher than upper bound!" << endl;
		}
	}

	return solution;
}