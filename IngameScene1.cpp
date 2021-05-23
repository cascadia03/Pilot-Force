#include "DXUT.h"
#include "IngameScene1.h"

void IngameScene1::Init()
{
	CAMERA->pos = InGrid(V2(160, 160));
	
	GAME->life = 100;
	GAME->maxLife = 100;
	GAME->move = 100;
	GAME->maxMove = 100;
	GAME->score = 0;
	GAME->timer = 0;
	GAME->cured_percent = 0;

	ShowCursor(false);

	SOUND->Play(L"Game Music", 500, true);
	SOUND->Play(L"SecurityRoomAmbient", 1000, true);

	tilemap = OBJECT->Add(new Tilemap(1));
	
	CreatePlayer();

	OBJECT->Add(new Instant(5)
	)->Add(new Instant(
		[=](LPINST a)
		{
			spawner = OBJECT->Add(new Instant(5, true,
				[=](LPINST a)
				{
					if (OBJECT->SearchAll<Enemy>().size() < 5)
					{
					REROLL1:
						switch (random(1, 2))
						{
						case 1:
							if (OBJECT->SearchAll<SizeEnemy>().size() >= 3)
								goto REROLL1;
							else
								OBJECT->Add(new SizeEnemy());
							break;
						case 2:
							if (OBJECT->SearchAll<SpeedEnemy>().size() >= 2)
								goto REROLL1;
							else	
								OBJECT->Add(new SpeedEnemy());
							break;
						}
					}
				},
				[=](LPINST a) {})
			);
		})
	);
}

void IngameScene1::Release()
{
	if (spawner)
		spawner->Destroy();

	OBJECT->Clear();
	SOUND->StopAll();

	ShowCursor(true);
}

void IngameScene1::Update()
{
	if (OBJECT->Search<Player>())
		GAME->UpdateTimer();

	if ((INPUT->KeyDown(VK_F9) ||
		GAME->IsClear()) &&
		!isCleared)
	{
		//	스테이지 강제 클리어
		ClearEvent();
	}

	auto player = OBJECT->Search<Player>();

	if (player && (GAME->life == 0 || GAME->timer >= 180) &&
		!isDefeat)
	{
		isDefeat = true;
		player->status = DamagedStatus;
	}
}

void IngameScene1::Render()
{
	GAME->RenderUI();
	GAME->RenderHP();
	GAME->RenderTimer();
}

void IngameScene1::CreatePlayer()
{
	V2 playerPos = V2(0, 0);
	
	if (random(false, true))
	{
		playerPos.x = random(false, true) ? 0 : 319;
		playerPos.y = random(0, 319);
	}
	else
	{
		playerPos.x = random(0, 319);
		playerPos.y = random(false, true) ? 0 : 319;
	}

	GAME->CreatePlayer(playerPos);
}


void IngameScene1::ClearEvent()
{
	if (!isCleared)
	{
		isCleared = true;

		if (spawner)
			spawner->Destroy();

		auto player = OBJECT->Search<Player>();

		if (player)
			player->Destroy();

		for (auto i : OBJECT->SearchAll<Enemy>())
		{
			i->Destroy();
		}

		for (auto i : OBJECT->SearchAll<Item>())
		{
			i->Destroy();
		}

		SOUND->Stop(L"Game Music");
		SOUND->Stop(L"SecurityRoomAmbient");
		SOUND->Play(L"TD Victory", 100, false);

		OBJECT->Add(new Instant(1)
		)->Add(new Instant(1,
			[=](LPINST a) {
				CAMERA->pos = lerp(CAMERA->pos, V2(0, 320), 0.5);
				CAMERA->scale = lerp(CAMERA->scale, 2.f, 0.5);
			})
		)->Add(new Instant(3,
			[=](LPINST a) {
				auto tilemap = OBJECT->Search<Tilemap>();

				tilemap->depth = 90;
				int ix = (320 * a->percent);

				for (int y = 0; y < 320; y++)
					for (int x = ix - 2; x <= ix + 2; x++)
					{
						if (x == 0 || x == 319 || y == 0 || y == 319 || x == ix + 2)
							tilemap->SetCell(x, y, LinedCell);
						else
							tilemap->SetCell(x, y, FilledCell);
					}
			})
		)->Add(new Instant(1,
			[=](LPINST a) {
				CAMERA->pos = lerp(CAMERA->pos, V2(320, 320), 0.5);
				CAMERA->scale = lerp(CAMERA->scale, 1.f, 0.5);
			})
		)->Add(new Instant(
			[=](LPINST a) {
				SCENE->Load<IngameScene2>({ 160, 160 }, 1);
			})
		);

	}
}
