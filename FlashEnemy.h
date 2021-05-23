#pragma once

class FlashEnemy : public Enemy
{
public:
	FlashEnemy() {}
	virtual ~FlashEnemy() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void CollisionEvent() override;

	int side = -1;

	Instant* flash = NULL;
};
