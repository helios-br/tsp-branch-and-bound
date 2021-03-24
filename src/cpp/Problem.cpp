#include "Problem.h"

#include <iostream>
#include <vector>

int PROBLEM_ID_COUNTER = -1;

/** Create new problem instance to be solved */
Problem *createProblem(Data *data)
{
    int dimension = data->getDimension();
    int problemId = ++PROBLEM_ID_COUNTER;
    //cout << "New problem [problemId: " << problemId << ", dimension: " << dimension << "]" << endl;
    return new Problem(problemId, dimension, copyDoubleMatrix(data->getMatrixCost(), dimension));
}

Problem *copyProblem(Problem *superProblem)
{
    int problemId = ++PROBLEM_ID_COUNTER;
    Problem *newProblem = new Problem(superProblem, problemId);
    //cout << "[copy] New problem [problemId: " << problemId << ", superProblemId: " << superProblem->problemId << "]" << endl;
    return newProblem;
}

/** Calculate solution cost */
double calculateCost(int **assignment, double **cost, int dimension)
{
    double totalCost = 0;
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            if (assignment[i][j] > 0)
            {
                totalCost += cost[i][j];
            }
        }
    }
    return totalCost;
}

void printProblem(Problem *problem, bool showCost)
{
    cout << endl;
    cout << "Problem {" << endl;
    cout << "  problem-id: " << problem->problemId << endl;
    cout << "  super-problem-id: " << (problem->superProblem ? problem->superProblem->problemId : 0) << endl;
    cout << "  dimension: " << problem->dimension << endl;
    cout << "  blocked moves: ";
    for (unsigned i = 0; i < problem->blockedMoves.size(); i++) {
        pair<int, int> blockedMove = problem->blockedMoves[i];
        cout << blockedMove.first << "->" << blockedMove.second << " | ";
    }
    cout << endl;
    
    cout << "}" << endl;

    if (showCost)
    {
        cout << endl;
        cout << "Problem [" << problem->problemId << "] cost matrix:" << endl;
        printDoubleMatrix(problem->cost, problem->dimension);
    }
}