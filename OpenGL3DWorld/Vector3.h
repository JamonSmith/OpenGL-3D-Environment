#pragma once

#include "Vector2.h"

struct Vector3 : public Vector2
{
	double y;

	Vector3() : Vector2(), y(5.0) {}

	Vector3(double xval, double yval, double zval) : Vector2(xval, zval), y(yval) {}

	double magnitude3() const
	{
		return sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
	}

	void normalize3()
	{
		double mag = magnitude3();

		if (mag <= 0)
		{
			return;
		}

		x /= mag;
		y /= mag;
		z /= mag;
	}
};