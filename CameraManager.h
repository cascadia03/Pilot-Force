#pragma once

class CameraManager : public Singleton<CameraManager>
{
public:
	CameraManager();
	virtual ~CameraManager();

	void Update();
	void SetTransform();
	MAT GetTransform();

private:
	MAT viewMat;

	struct SHAKE
	{
	public:
		V2 direction = V2(0, 0);
		float force = 0;
		float time = 0;
	}shake;

public:
	V2 pos = V2(0, 0);
	float scale = 1;

	void Shake(float pForce, float pTime);

};

#define CAMERA CameraManager::GetInstance()
