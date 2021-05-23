#pragma once

//	Objects
#include "Instant.h"
#include "Tilemap.h"

#include "Enemy.h"
#include "SizeEnemy.h"
#include "SpeedEnemy.h"
#include "FlashEnemy.h"

#include "Item.h"
#include "Bullet.h"
#include "Missile.h"
#include "Bomb.h"

#include "Player.h"

//	Scenes
#include "IntroScene.h"
#include "TtileScene.h"
#include "IngameScene1.h"
#include "IngameScene2.h"
#include "ScoreScene.h"

class GameManager : public Singleton<GameManager>
{
public:
	GameManager();
	virtual ~GameManager();

	void Update();
	void Render();

	void RenderUI();
	void RenderHP();

	void UpdateTimer();
	void RenderTimer();

	wstring gameResult = L"\0";

	bool IsClear();

	wstring text = L"\0";
	COLOR textColor = COLOR(1, 1, 1, 0);

	void PrintText(wstring pText);

	void CreatePlayer(V2 pPos);

	bool isOverpowered = false;

	COLOR uiColor = COLOR(1, 1, 1, 1);
	float uiScale = 1;

	int score = 0;
	float life = 100;
	float maxLife = 100;
	float prevlife = 100;
	float move = 100;
	float prevMove = 100;
	float maxMove = 100;
	float cured_percent = 0;
	float toxiced_percent = 0;
	float timer = 0;

	vector<pair<int, wstring>> ranks =
	{
		{ 0, L"AAA" },
		{ 0, L"BBB" },
		{ 0, L"CCC" },
		{ 0, L"DDD" },
		{ 0, L"EEE" }
	};
	multimap<pair<V2*, UINT>, LPDIRECTSOUNDBUFFER> buffers;

	LPDIRECTSOUNDBUFFER PanPlaySound(wstring pPath, UINT pVolume, bool pIsLoop, V2* pX);

	Instant* itemInst = NULL;

	void ItemUse(ItemType pType);

};

#define GAME GameManager::GetInstance()
