#pragma once

class IngameScene2 : public Scene
{
public:
	IngameScene2() {}
	virtual ~IngameScene2() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	Tilemap* tilemap = NULL;

	Instant* spawner = NULL;

	bool isCleared = false;
	bool isDefeat = false;

	void CreatePlayer();
	void ClearEvent();
};
