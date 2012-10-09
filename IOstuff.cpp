#include "IOstuff.h"

void readInputFile(char* filename, int numTriangles, int numVertices, vector<int>& triangleTable, vector<double>& vertexTable)
{
	ifstream inFile(filename);
	if(!inFile)
	{
		cout << "Error reading from " << filename << endl;
		exit(1);
	}
	inFile >> numTriangles >> numVertices;
	cout << "Triangles: " << numTriangles << endl;
	cout << "Vertices: " << numVertices << endl;
	exit(0);
}