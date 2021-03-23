#ifndef DATA_H
#define DATA_H

#define INFINITE 2147483647

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

class Data{
public:
	Data(char *);
	~Data();

	void readData();
	void printMatrixDist();
	inline int getDimension() { return dimension; };
	inline double getDistance(int i, int j) { return distMatrix[i][j]; };
	inline double **getMatrixCost() { return distMatrix; }
	inline double getXCoord(int i) { return xCoord[i]; }
	inline double getYCoord(int i) { return yCoord[i]; }
	inline bool getExplicitCoord() { return explicitCoord; };

	string getInstanceName();

private:
	string instaceName;
	int dimension;
	double **distMatrix = NULL;
	double *xCoord = NULL;
	double *yCoord = NULL;
	bool explicitCoord;

	static double CalcDistEuc ( double *, double *, int , int );
	static double CalcDistAtt ( double *, double *, int , int );
	static double CalcDistGeo ( double *, double *, int , int );
	static void CalcLatLong ( double *, double *, int , double *, double* );
};

#endif