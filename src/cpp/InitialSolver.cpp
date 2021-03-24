#include "InitialSolver.h"

#include <iostream>
#include <vector>

using namespace std;

Solution *InitialSolver::solve()
{
	Problem *problem = this->problem;
	int dimension = problem->dimension;
	int **assignment = createIntMatrix(dimension);
	int assignedNodes[dimension] = {};

	// Node "0" is the first element of the tour
	int node = 0;

	while (!allItemsProcessed(assignedNodes, dimension))
	{
		int lowestCost = GLOBAL_INFINITE_COST;
		int nodeLowestCost = -1;

		for (int nextNode = 0; nextNode < problem->dimension; nextNode++)
		{
			if (node == nextNode || assignedNodes[nextNode] > 0)
				continue;

			if (problem->cost[node][nextNode] < lowestCost)
			{
				lowestCost = problem->cost[node][nextNode];
				nodeLowestCost = nextNode;
			}
		}

		assignedNodes[node] = 1;
		assignedNodes[nodeLowestCost] = 1;
		assignment[node][nodeLowestCost] = 1;
		node = nodeLowestCost;
	}

	assignment[node][0] = 1;

	return new Solution(problem, assignment, calculateCost(assignment, problem->cost, dimension));
};