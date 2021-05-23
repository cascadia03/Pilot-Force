#pragma once

namespace math
{
	template<typename Type>
	Type lerp(Type pBegin, Type pEnd, float pTime)
	{
		return pBegin + (pEnd - pBegin) * pTime;
	}

	int getSign(int pValue);

	float clamp(float pValue, float pMin, float pMax);

	int random(int pMin, int pMax);
	float random(float pMin, float pMax);

	float toRotate(V2 pDirection);
	float toLength(V2 pDirection);
	V2 toDirection(float pRotate);
	V2 toNoramlize(V2 pDirection);

};

using namespace math;
