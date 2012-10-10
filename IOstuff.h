#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>

#include "triangle.h"
#include "vertex.h"

#pragma once

using namespace std;

void readInputFile(string& filename, int numTriangles, int numVertices, vector<triangle>& triangleTable, vector<vertex>& vertexTable);