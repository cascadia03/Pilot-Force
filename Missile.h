#pragma once

class Missile : public Object
{
public:
	Missile(V2 pPos, float pRotate)
	{
		pos = pPos;
		direction = toDirection(pRotate);
	}
	virtual ~Missile() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	Image* image = NULL;

	Enemy* target = NULL;

	V2 pos = V2(0, 0);
	V2 direction = V2(0, 0);

	float homingTimer = 1;

};
