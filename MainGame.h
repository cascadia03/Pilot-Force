#pragma once

class MainGame
{
public:
	MainGame();
	~MainGame();

	void Update();
	void Render();

	void OnLostDevice();
	void OnResetDevice();
};
