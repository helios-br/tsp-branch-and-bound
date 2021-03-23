#ifndef COLLECTION_UTIL_H
#define COLLECTION_UTIL_H

#include <stdio.h>
#include <stdlib.h>

/** Return true case all items > 1 */
inline bool allItemsProcessed(int nodesProcessed[], int dimension)
{
    for (int i = 0; i < dimension; i++)
    {
        if (nodesProcessed[i] == 0)
        {
            return false;
        }
    }
    return true;
}

inline int **createIntMatrix(int dimension)
{
  int **matrix = new int *[dimension];
  for (int i = 0; i < dimension; i++)
  {
    matrix[i] = new int[dimension];
    for (int j = 0; j < dimension; j++)
    {
      matrix[i][j] = 0;
    }
  }
  return matrix;
}

inline double **copyDoubleMatrix(double **matrix, int dimension)
{
  double **newMatrix = new double *[dimension];
  for (int i = 0; i < dimension; i++)
  {
    newMatrix[i] = new double[dimension];
    for (int j = 0; j < dimension; j++)
    {
      newMatrix[i][j] = matrix[i][j];
    }
  }
  return newMatrix;
};

inline int **copyIntMatrix(int **matrix, int dimension)
{
  int **newMatrix = new int *[dimension];
  for (int i = 0; i < dimension; i++)
  {
    newMatrix[i] = new int[dimension];
    for (int j = 0; j < dimension; j++)
    {
      newMatrix[i][j] = matrix[i][j];
    }
  }
  return newMatrix;
};

inline int **convertToIntMatrix(double **matrix, int dimension)
{
  int **newMatrix = new int *[dimension];

  for (int i = 0; i < dimension; i++)
  {
    newMatrix[i] = new int[dimension];
    for (int j = 0; j < dimension; j++)
    {
      newMatrix[i][j] = matrix[i][j];
    }
  }

  return newMatrix;
};

inline unsigned int **convertToUnsignedIntMatrix(double **matrix, int dimension)
{
  unsigned int **newMatrix = new unsigned int *[dimension];

  for (int i = 0; i < dimension; i++)
  {
    newMatrix[i] = new unsigned int[dimension];
    for (int j = 0; j < dimension; j++)
    {
      newMatrix[i][j] = matrix[i][j];
    }
  }

  return newMatrix;
};

inline void freeDoubleMatrix(double **matrix, int dimension)
{
  for (int i = 0; i < dimension; i++)
  {
    delete[] matrix[i];
  }
  delete[] matrix;
}

inline void freeIntMatrix(int **matrix, int dimension)
{
  for (int i = 0; i < dimension; i++)
  {
    delete[] matrix[i];
  }
  delete[] matrix;
}

inline void freeUnsignedIntMatrix(unsigned int **matrix, int dimension)
{
  for (int i = 0; i < dimension; i++)
  {
    delete[] matrix[i];
  }
  delete[] matrix;
}

inline void printMatrix(int **matrix, int dimension)
{
  fprintf(stderr, "\n");
  for (int i = 0; i < dimension; i++)
  {
    fprintf(stderr, " [");
    for (int j = 0; j < dimension; j++)
    {
      fprintf(stderr, "%5d ", matrix[i][j]);
    }
    fprintf(stderr, "]\n");
  }
  fprintf(stderr, "\n");
}

inline void printDoubleMatrix(double **matrix, int dimension)
{
  fprintf(stderr, "\n");
  for (int i = 0; i < dimension; i++)
  {
    fprintf(stderr, " [");
    for (int j = 0; j < dimension; j++)
    {
      fprintf(stderr, "%5f ", matrix[i][j]);
    }
    fprintf(stderr, "]\n");
  }
  fprintf(stderr, "\n");
}

#endif