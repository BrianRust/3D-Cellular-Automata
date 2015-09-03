#ifndef included_IntVector3
#define included_IntVector3
#pragma once

//-------------------------------------------------------------
class IntVector3
{
public:
	IntVector3(){};
	IntVector3(int InitialX, int InitialY, int InitialZ) {
		x = InitialX;
		y = InitialY;
		z = InitialZ;
	};
	int x;
	int y;
	int z;
};

#endif //included_IntVector3