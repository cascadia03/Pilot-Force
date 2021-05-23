#pragma once

class Bullet : public Object
{
public:
	Bullet(V2 pPos, float pRotate) 
	{ 
		pos = pPos;
		direction = toDirection(pRotate);
	}
	virtual ~Bullet() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	Image* image = NULL;

	V2 pos = V2(0, 0);
	V2 direction = V2(0, 0);
};

