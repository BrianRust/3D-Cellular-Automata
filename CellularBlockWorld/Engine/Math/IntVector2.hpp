#ifndef included_IntVector2
#define included_IntVector2
#pragma once

//-------------------------------------------------------------
class IntVector2
{
public:
	IntVector2(){};
	IntVector2(int InitialX, int InitialY) {
		x = InitialX;
		y = InitialY;
	};
	int x;
	int y;
};

inline bool operator == (const IntVector2& lhs, const IntVector2& rhs) {
	if (lhs.y == rhs.y) {
		if (lhs.x == rhs.x) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

inline IntVector2 operator - (const IntVector2& first, const IntVector2& second) {
	IntVector2 result;
	result.x = first.x - second.x;
	result.y = first.y - second.y;
	return result;
};

inline bool operator < (const IntVector2& lhs, const IntVector2& rhs) {
	if (lhs.y == rhs.y) {
		if (lhs.x < rhs.x) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (lhs.y < rhs.y) {
			return true;
		}
		else {
			return false;
		}
	}
};

#endif //included_IntVector2