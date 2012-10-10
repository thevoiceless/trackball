#pragma once

class Vector
{
public:
	double x, y, z;

	Vector(double x, double y, double z) : x(x), y(y), z(z) {}

	Vector minus(Vector& v2);
};