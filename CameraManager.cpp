#include "DXUT.h"
#include "CameraManager.h"

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::Update()
{
	if (shake.time > 0)
	{
		shake.time -= DT;
		shake.direction.x = random(-shake.force, shake.force);
		shake.direction.y = random(-shake.force, shake.force);
	}
	else
	{
		shake.direction = V2(0, 0);
	}

	D3DXMatrixTransformation2D(
		&viewMat,
		&pos,
		NULL,
		&V2(scale, scale),
		NULL,
		NULL,
		&(-pos + V2(960, 540) + shake.direction)
	);
}

void CameraManager::SetTransform()
{
	g_device->SetTransform(D3DTS_VIEW, &viewMat);
}

MAT CameraManager::GetTransform()
{
	MAT invMat;
	D3DXMatrixInverse(&invMat, NULL, &viewMat);
	return invMat;
}

void CameraManager::Shake(float pForce, float pTime)
{
	shake.force = pForce;
	shake.time = pTime;
}
