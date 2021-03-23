#include <iostream>
#include "external/Data.h"
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
    cout << endl
         << "Best solution found:" << endl;
    solution->printSolution(true);
    delete solver;
    delete solution;
}

int main(int argc, char **argv)
{
    try
    {
        // Validate parameters
        verify(argc);

        // Read instance data from filesystem
        Data *data = new Data(argv[1]);
        data->readData();

        // Solve problem

        Problem *problem = createProblem(data);
        //solveProblem(new InitialSolver(problem));
        //solveProblem(new HungarianSolver(problem));
        solveProblem(new BranchBoundSolver(problem));

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