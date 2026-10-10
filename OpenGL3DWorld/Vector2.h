#pragma once

#include <cmath>

struct Vector2
{
    double x;
    double z;

    double magnitude() const
    {
        return sqrt(pow(x, 2.0) + pow(z, 2.0));
	}

    void normalize()
    {
		double mag = magnitude();
        
        if (mag <= 0)
        {
            return;
        }

		x /= mag;
		z /= mag;
	}
};
