#pragma once
 
#include "Vector2.h"

struct AABB
{
	Vector2 min;
	Vector2 max;

	bool intersects(const AABB& other) const
	{
		return (min.x <= other.max.x && max.x >= other.min.x) &&
			   (min.z <= other.max.z && max.z >= other.min.z);
	}
};