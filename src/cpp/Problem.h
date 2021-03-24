#ifndef PROBLEM_H
#define PROBLEM_H

#define GLOBAL_INFINITE_COST 99999999;

#include <iostream>
#include <vector>
#include "external/Data.h"
#include "util/CollectionUtil.h"

using namespace std;

double calculateCost(int **assignment, double **cost, int dimension);

/**
 * Struct: Problem
 *
 * Represents the problem to be solved
 */
struct Problem
{
  Problem *superProblem = NULL;
  int problemId;
  int dimension;
  double **cost;
  vector<pair<int, int>> blockedMoves;

  Problem(int problemId, int dimension, double **cost)
  {
    this->problemId = problemId;
    this->dimension = dimension;
    this->cost = cost;
  }

   Problem(Problem *superProblem, int problemId)
   {
     this->problemId = problemId;
     this->dimension = superProblem->dimension;
     this->superProblem = superProblem;
     this->cost = copyDoubleMatrix(superProblem->cost, superProblem->dimension);
     this->blockedMoves = superProblem->blockedMoves;
   }

  Problem(Problem * problem) {
    this->problemId = problem->problemId;
    this->dimension = problem->dimension;
    this->cost = copyDoubleMatrix(problem->cost, problem->dimension);
    this->blockedMoves = problem->blockedMoves;
  }

  ~Problem()
  {
    freeDoubleMatrix(cost, dimension);
  }

  void blockMove(int i, int j)
  {
    blockedMoves.push_back({i, j});
    cost[i][j] = GLOBAL_INFINITE_COST;
    //cost[j][i] = GLOBAL_INFINITE_COST;
  }
};

/**
 * Struct: Solution
 *
 * Represents the solution for the given problem
 */
struct Solution
{
  Problem *problem;
  int dimension;
  int **assignment;
  double cost;

  Solution(Problem *problem, int **assignment)
  {
    this->problem = problem;
    this->dimension = problem->dimension;
    this->assignment = assignment;
    this->cost = calculateCost(assignment, problem->cost, problem->dimension);
  }

  Solution(Solution *solution)
  {
    this->problem = new Problem(solution->problem);
    this->dimension = solution->dimension;
    this->assignment = copyIntMatrix(solution->assignment, solution->dimension);
    this->cost = solution->cost;
  }

  ~Solution()
  {
    freeIntMatrix(assignment, dimension);
  }

  vector<vector<int>> extractTours()
  {
    vector<vector<int>> allTours;

    int nodesProcessed[dimension] = {};
    int node = 0;

    while (!allItemsProcessed(nodesProcessed, dimension))
    {
      vector<int> tour;
      int nextNode = 0;

      if (nodesProcessed[node] > 0)
      {
        node++;
        continue;
      }

      while (true)
      {
        if (assignment[node][nextNode] > 0)
        {
          if (nodesProcessed[node] == 0)
          {
            tour.push_back(node);
            nodesProcessed[node] = 1;
            node = nextNode;
            nextNode = 0;
          }
          else
          {
            tour.push_back(node);
            allTours.push_back(tour);
            node = 0;
            break;
          }
        }
        else
        {
          nextNode += 1;
        }
      }
    }

    return allTours;
  }

  bool isSolutionFeasible()
  {
    vector<vector<int>> tours = extractTours();
    return tours.size() > 1;
  }

  void printTours()
  {
    vector<vector<int>> allTours = extractTours();
    cout << "  tours: {";
    for (unsigned i = 0; i < allTours.size(); i++)
    {
      cout << "{";
      for (unsigned j = 0; j < allTours[i].size(); j++)
      {
        cout << (j == 0 ? "" : " ") << allTours[i][j];
      }
      cout << "}";
    }
    cout << "}" << endl;
  };

  void printSolution(bool showMatrix)
  {
    cout << endl
         << "Solution {" << endl;
    cout << "  problem-id: " << this->problem->problemId << "," << endl;
    cout << "  cost: " << this->cost << endl;
    printTours();
    cout << "}" << endl;    
    if (showMatrix)
      printMatrix(this->assignment, this->dimension);
  }
};

Problem *createProblem(Data *data);
Problem *copyProblem(Problem *superProblem);
void printProblem(Problem *problem, bool showCost);

#endif