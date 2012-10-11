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