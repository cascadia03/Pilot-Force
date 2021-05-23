#include "DXUT.h"
#include "math.h"

int math::getSign(int pValue)
{
    if (pValue >= 0)
        return 1;
    else
        return -1;
}

float math::clamp(float pValue, float pMin, float pMax)
{
    return max(pMin, min(pMax, pValue));
}

int math::random(int pMin, int pMax)
{
    random_device device;
    mt19937 gen(device());
    uniform_int_distribution<> dis(pMin, pMax);

    return dis(gen);
}

float math::random(float pMin, float pMax)
{
    random_device device;
    mt19937 gen(device());
    uniform_real_distribution<> dis(pMin, pMax);

    return dis(gen);
}

float math::toRotate(V2 pDirection)
{
    return D3DXToDegree(atan2f(pDirection.y, pDirection.x));
}

float math::toLength(V2 pDirection)
{
    return D3DXVec2Length(&pDirection);
}

V2 math::toDirection(float pRotate)
{
    while (pRotate > 360)
    {
        pRotate -= 360;
    }

    while (pRotate <= 0)
    {
        pRotate += 360;
    }

    return V2(cosf(D3DXToRadian(pRotate)), sinf(D3DXToRadian(pRotate)));
}

V2 math::toNoramlize(V2 pDirection)
{
    V2 normalized;
    D3DXVec2Normalize(&normalized, &pDirection);
    return normalized;
}
