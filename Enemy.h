#pragma once

class Enemy : public Object
{
public:
	Enemy() {}
	virtual ~Enemy()
	{
		if (wave)
			wave->Destroy();
		if (buffer)
			buffer->Stop();
	}

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void CollisionEvent() = 0;

	Image* image = NULL;

	Instant* wave = NULL;

	V2 pos = V2(0, 0);
	V2 direction = V2(0, 0);
	COLOR color = COLOR(1, 1, 1, 1);
	float rotate = 0;
	float scale = 0;
	float speed = 0;

	float radius = 0;
	float crashTime = 0;

	float hp = 20;

	bool isAppeared = false;

	LPDIRECTSOUNDBUFFER buffer = NULL;

	void Movement(float pRotate);
	void IsDelete();
	void IsDamaged();
	void ImageRotate(float pSpeed);
	void ImageDirection(V2 pDirection);
	void CreateEffect();
	void DestroyEffect();
};

