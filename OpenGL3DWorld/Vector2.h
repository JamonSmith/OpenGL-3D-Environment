#pragma once

#include <cmath>

struct Vector2
{
    double x;
    double z;

    Vector2() : x(5.0), z(5.0) {}

    Vector2(double xval, double zval) : x(xval), z(zval) {}

    double magnitude2() const
    {
        return sqrt(pow(x, 2.0) + pow(z, 2.0));
	}

    void normalize2()
    {
		double mag = magnitude2();
        
        if (mag <= 0)
        {
            return;
        }

		x /= mag;
		z /= mag;
	}
};
