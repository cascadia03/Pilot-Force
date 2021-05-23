#pragma once

class SizeEnemy : public Enemy
{
public:
	SizeEnemy() {}
	virtual ~SizeEnemy() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void CollisionEvent() override;
};
