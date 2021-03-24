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

int main(int argc, char **argv)
{
    // gr17, gr21, gr24, ulysses16
    try
    {
        // Validate parameters
        verify(argc);

        // Read instance data from filesystem
        string fileName = argv[1];
        Data *data = new Data(convertToChar(fileName + ".tsp"));
        data->readData();

        // Solve problem

        Problem *problem = createProblem(data);
        //solveProblem(new InitialSolver(problem));
        //solveProblem(new HungarianSolver(problem));
        solveProblem(new BranchBoundSolver(problem));
        printProblem(problem, true);

        //BranchBoundSolver *solver = new BranchBoundSolver(&problem);
        //Solution solution = solver->solve();

        // Free memory

        delete problem;
        delete data;
    }
    catch (invalid_argument &e)
    {
        cout << "Unhandled exception thrown: " << e.what() << endl;
        return 1;
    }

    return 0;
}