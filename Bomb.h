#pragma once

class Bomb : public Object
{
public:
	Bomb(V2 pPos, float pRotate)
	{
		pos = pPos;
		direction = toDirection(pRotate);
		rotate = pRotate;
	}
	virtual ~Bomb() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	Image* image = NULL;

	Instant* blinkInst = NULL;

	V2 pos = V2(0, 0);
	V2 direction = V2(0, 0);
	float rotate = 0;

	float timer = 0;
};

