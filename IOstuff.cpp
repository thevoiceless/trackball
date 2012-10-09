#include "IOstuff.h"

void readInputFile(string& filename, int numTriangles, int numVertices, vector<int>& triangleTable, vector<double>& vertexTable)
{
	// Open the input file
	ifstream inFile(filename.c_str());
	if (!inFile.is_open())
	{
		cout << "Error reading from " << filename << endl;
		exit(1);
	}

	// Read the number of triangles and vertices
	inFile >> numTriangles >> numVertices;
	cout << "Triangles: " << numTriangles << endl;
	cout << "Vertices: " << numVertices << endl;

	// Read the vertex IDs for each triangle
	// Triangle soup: Each line contains 3 vertices that define a triangle
	for (int i = 0; i < numTriangles; ++i)
	{
		int v1, v2, v3;
		inFile >> v1 >> v2 >> v3;
		triangleTable.push_back(v1);
		triangleTable.push_back(v2);
		triangleTable.push_back(v3);
	}

	// Read vertex coordinates for all vertices
	for (int i = 0; i < numVertices; ++i)
	{
		double x, y, z;
		inFile >> x >> y >> z;
		vertexTable.push_back(x);
		vertexTable.push_back(y);
		vertexTable.push_back(z);
	}

	cout << "Number of vertex IDs: " << triangleTable.size() << endl;
	cout << "Number of vertices: " << vertexTable.size() << endl;
	inFile.close();
}