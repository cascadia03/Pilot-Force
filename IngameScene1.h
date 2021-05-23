#pragma once

class IngameScene1 : public Scene
{
public:
	IngameScene1() {}
	virtual ~IngameScene1() {}

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
