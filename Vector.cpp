#include "Vector.h"

Vector Vector::plus(Vector& v2)
{
	return Vector((x + v2.x),
		(y + v2.y),
		(z + v2.z));
}

Vector Vector::minus(Vector& v2)
{
	return Vector((x - v2.x),
		(y - v2.y),
		(z - v2.z));
}

double Vector::dotWith(Vector& v2)
{
	return ((x * v2.x)
		+ (y * v2.y)
		+ (z * v2.z));
}

Vector Vector::crossWith(Vector& v2)
{
	double xhat = ((y * v2.z) - (z * v2.y));
	double yhat = ((z * v2.x) - (x * v2.z));
	double zhat = ((x * v2.y) - (y * v2.x));
	return Vector(xhat, yhat, zhat);
}