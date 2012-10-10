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
Vector calcNormal(triangle& t, vector<vertex>& vertexTable)
{
	Vector U((vertexTable.at(t.v2).x - vertexTable.at(t.v1).x),
		(vertexTable.at(t.v2).y - vertexTable.at(t.v1).y),
		(vertexTable.at(t.v2).z - vertexTable.at(t.v1).z));

	Vector V((vertexTable.at(t.v3).x - vertexTable.at(t.v1).x),
		(vertexTable.at(t.v3).y - vertexTable.at(t.v1).y),
		(vertexTable.at(t.v3).z - vertexTable.at(t.v1).z));

	return Vector(((U.y * V.z) - (U.z * V.y)),
		((U.z * V.x) - (U.x * V.z)),
		((U.x * V.y) - (U.y * V.x)));
}