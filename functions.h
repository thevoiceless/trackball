#pragma once

// https://www.opengl.org/wiki/Calculating_a_Surface_Normal
// Begin Function CalculateSurfaceNormal (Input Triangle) Returns Vector
// 	Set Vector U to (Triangle.v2 minus Triangle.v1)
// 	Set Vector V to (Triangle.v3 minus Triangle.v1)
// 	Set Normal.x to (multiply U.y by V.z) minus (multiply U.z by V.y)
// 	Set Normal.y to (multiply U.z by V.x) minus (multiply U.x by V.z)
// 	Set Normal.z to (multiply U.x by V.y) minus (multiply U.y by V.x)
// 	Return Normal
// End Function
Vector calcSurfaceNormal(triangle& t, vector<vertex>& vertexTable)
{
	Vector U((vertexTable.at(t.v2).x - vertexTable.at(t.v1).x),
		(vertexTable.at(t.v2).y - vertexTable.at(t.v1).y),
		(vertexTable.at(t.v2).z - vertexTable.at(t.v1).z));

	Vector V((vertexTable.at(t.v3).x - vertexTable.at(t.v1).x),
		(vertexTable.at(t.v3).y - vertexTable.at(t.v1).y),
		(vertexTable.at(t.v3).z - vertexTable.at(t.v1).z));

	return Vector(-((U.y * V.z) - (U.z * V.y)),
		-((U.z * V.x) - (U.x * V.z)),
		-((U.x * V.y) - (U.y * V.x)));
}

void calcNormals(vector<triangle>& triangleTable, vector<vertex>& vertexTable, vector<Vector>& triangleNormals, vector<Vector>& vertexNormals)
{
	// Calculate normal for each triangle
	for (int i = 0; i < triangleTable.size(); ++i)
	{
		triangleNormals.push_back(calcSurfaceNormal(triangleTable.at(i), vertexTable));
	}

	// Calculate normal for each vertex
	// Normal of a vertex v = Sum of cross products computed for all incident triangles
	// Linear-time algorithm:
	// 		Allocate a vector N[] of V normals (V = number of vertices), initialize all entries to zero
	// 		For each triangle ∆(a, b, c) do
	// 		Below, a,b and c are considered integer IDs of vertices or their coordinates depending on context
	// 			Compute normal of ∆, N∆ = ab × ac
	// 			N[a]+ = N∆
	// 			N[b]+ = N∆
	// 			N[c]+ = N∆
	// Since triangles are oriented consistently, all cross products will point in a consistent direction
	// Want to use outward normals
	vector<Vector> normals (vertexTable.size(), Vector(0,0,0));
	for (int i = 0; i < triangleTable.size(); ++i)
	{
		normals.at(triangleTable.at(i).v1) = normals.at(triangleTable.at(i).v1).plus(triangleNormals.at(i));
		normals.at(triangleTable.at(i).v2) = normals.at(triangleTable.at(i).v2).plus(triangleNormals.at(i));
		normals.at(triangleTable.at(i).v3) = normals.at(triangleTable.at(i).v3).plus(triangleNormals.at(i));
	}
	vertexNormals = normals;
	cout << "Number of triangle normals: " << triangleNormals.size() << endl;
	cout << "Number of vertex normals: " << vertexNormals.size() << endl;
}

void calcBoundingBox(vector<vertex>& vertexTable, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, double maxdim)
{
	xmin = xmax = vertexTable.at(0).x;
	ymin = ymax = vertexTable.at(0).y;
	zmin = zmax = vertexTable.at(0).z;

	for (int i = 1; i < vertexTable.size(); ++i)
	{
		if (vertexTable.at(i).x < xmin) xmin = vertexTable.at(i).x;
		if (vertexTable.at(i).x > xmax) xmax = vertexTable.at(i).x;

		if (vertexTable.at(i).y < ymin) ymin = vertexTable.at(i).y;
		if (vertexTable.at(i).y > ymax) ymax = vertexTable.at(i).y;

		if (vertexTable.at(i).z < zmin) zmin = vertexTable.at(i).z;
		if (vertexTable.at(i).z > zmax) zmax = vertexTable.at(i).z;
	}

	double tempmax = max(abs(xmax - xmin), abs(ymax - ymin));
	maxdim = max(tempmax, abs(zmax - zmin));

	cout << "Min x: " << xmin << endl;
	cout << "Max x: " << xmax << endl;
	cout << "Min y: " << ymin << endl;
	cout << "Max y: " << ymax << endl;
	cout << "Min z: " << zmin << endl;
	cout << "Max z: " << zmax << endl;
	cout << "Max dim: " << maxdim << endl;
}

double toRadians(double d)
{
	return (d * (M_PI / 180.0));
}
