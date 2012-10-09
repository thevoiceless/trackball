#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>

#pragma once

using namespace std;

void readInputFile(char* filename, int numTriangles, int numVertices, vector<int>& triangleTable, vector<double>& vertexTable);