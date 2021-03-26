#include <iostream>
#include <string>
#include "external/Data.h"
#include "util/StringUtil.h"
#include "util/OutputUtil.h"
#include "Problem.h"
#include "Solver.h"
#include "HungarianSolver.h"
#include "InitialSolver.h"
#include "BranchBoundSolver.h"
#include "BreadthBranchBoundSolver.h"
#include <bits/stdc++.h>

using namespace std;

void verify(int numberParameters)
{
    if (numberParameters < 2)
    {
        throw invalid_argument("Instance name?");
    }

    if (numberParameters > 2)
    {
        throw invalid_argument("Too many parameters");
    }
}

void solveProblem(Solver *solver)
{
    Solution *solution = solver->solve();
    logDashes();
    cout << "Best solution found:" << endl;
    solution->printSolution(true);
    delete solver;
    delete solution;
}

string normalizeFileName(char *parameter)
{
    string fileName = parameter;
    size_t foundDot = fileName.find(".");
    if (foundDot != string::npos)
    {
        return fileName;
    }
    return fileName + ".tsp";
}

int main(int argc, char **argv)
{
    // done: burma14 (3323), gr17 (2085), gr21 (2707), gr24 (1272), ulysses16 (6859), bays29 (2020), bayg29 (1610)

    time_t start, end;
    time(&start);

    try
    {
        // Validate parameters
        verify(argc);

        // Read instance data from filesystem
        string fileName = normalizeFileName(argv[1]);
        Data *data = new Data(convertToChar(fileName));
        data->readData();

        // Solve problem

        Problem *problem = createProblem(data);
        //solveProblem(new InitialSolver(problem));
        //solveProblem(new HungarianSolver(problem));
        //solveProblem(new BranchBoundSolver(problem));
        solveProblem(new BreadthBranchBoundSolver(problem));
        printProblem(problem, true);

        // Free memory

        delete problem;
        delete data;
    }
    catch (invalid_argument &e)
    {
        logn("Unhandled exception thrown: " + string(e.what()));
        return 1;
    }

    time(&end);
    double time_taken = double(end - start);
    cout << "@ Processing time : " << fixed << time_taken << setprecision(5) << " seconds" << endl;
    cout << endl;

    return 0;
}