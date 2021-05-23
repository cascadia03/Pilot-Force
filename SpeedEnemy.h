#pragma once

class SpeedEnemy : public Enemy
{
public:
	SpeedEnemy() {}
	virtual ~SpeedEnemy() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void CollisionEvent() override;
};
