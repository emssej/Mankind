#pragma once

#define CUBE_SIZE 0.5f

struct Cube {
	char typeID;

	bool IsSolid()
	{
		return typeID != 0;
	}
};

