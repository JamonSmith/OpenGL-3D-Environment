#pragma once
 
#include "Vector2.h"
#include "Vector3.h"

struct AABB
{
	Vector3 min;
	Vector3 max;

	bool intersects(const AABB& other) const
	{
		return (min.x < other.max.x && max.x > other.min.x) &&
			   (min.y < other.max.y && max.y > other.min.y) &&
			   (min.z < other.max.z && max.z > other.min.z);
	}
};