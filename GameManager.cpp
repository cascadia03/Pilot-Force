#include "DXUT.h"
#include "GameManager.h"

GameManager::GameManager()
{
	SCENE->Load<IntroScene>({ 960, 540 }, 1);
}

GameManager::~GameManager()
{
	for (auto i : buffers)
	{
		SAFE_RELEASE(i.second);
	}
	buffers.clear();
}

void GameManager::Update()
{
	if (INPUT->KeyDown(VK_F1))
	{
		//	무적
		isOverpowered = !isOverpowered;
	}

	if (INPUT->KeyDown(VK_F2))
	{
		//	아이템 랜덤 사용
		ItemUse((ItemType)random(0, 6));
	}

	if (INPUT->KeyDown(VK_F3))
	{
		//	HP 증가
		life = min(maxLife, life + 35);
	}

	if (INPUT->KeyDown(VK_F4))
	{
		// HP 감소
		life = max(0, life - 35);
	}

	if (INPUT->KeyDown(VK_F5))
	{
		//	이동속도 증가
		Player* player = OBJECT->Search<Player>();

		if (player)
		{
			player->speed = min(5, player->speed + 1);
		}
	}

	if (INPUT->KeyDown(VK_F6))
	{
		//	이동속도 감소
		Player* player = OBJECT->Search<Player>();

		if (player)
		{
			player->speed = max(1, player->speed - 1);
		}
	}

	if (INPUT->KeyDown(VK_F10))
	{
		//	메뉴화면으로 전환
		SCENE->Load<TitleScene>({ 960, 540 }, 1);
	}

	prevlife = lerp(prevlife, life, 0.03);
	prevMove = lerp(prevMove, move, 0.03);

	DWORD status;

	for (auto i = buffers.begin(); i != buffers.end();)
	{
		(*i).second->GetStatus(&status);

		if (!status & !DSBSTATUS_PLAYING)
		{
			SAFE_RELEASE((*i).second);
			i = buffers.erase(i);
		}
		else
		{
			Player* player = OBJECT->Search<Player>();
			
			if (player)
			{
				(*i).second->SetPan(((*i).first.first->x) - (player->pos.x) * (*i).first.second);
				(*i).second->SetVolume(-(toLength(player->pos  - *(*i).first.first)) * (*i).first.second);
			}
				
			i++;
		}
	}

	timer = min(180, timer);
}

void GameManager::Render()
{
	RENDER->Write(text, V2(960, 935), V2(0.5, 0.5), textColor, 0.5, 0, 100, true);
}

void GameManager::RenderUI()
{
	WCHAR percentStr[256];
	swprintf(percentStr, L"%.2f", cured_percent);
	RENDER->Write(percentStr, V2(960, 90), V2(0.5, 0.5), uiColor, 1.5 * uiScale, 0, 100, true);

	WCHAR scoreStr[256];
	swprintf(scoreStr, L"%08d", score);
	RENDER->Write(scoreStr, V2(960, 180), V2(0.5, 0.5), uiColor, 0.5 * uiScale, 0, 100, true);
}

void GameManager::RenderHP()
{
	Image* hpImage = IMAGE->Load(L"ui/hp");
	Image* hpBack = IMAGE->Load(L"ui/hp back");
	Image* hpLine = IMAGE->Load(L"ui/hp line");
	Image* hpPrev = IMAGE->Load(L"ui/hp prev");
	Image* hpMove = IMAGE->Load(L"ui/speed");

	RENDER->Render(hpLine, V2(90, 90), V2(0, 0.5), COLOR(0.5, 0.5, 0.5, 1), 1.5, 0, 95, true);
	RENDER->Render(hpBack, V2(90, 90), V2(0, 0.5), COLOR(0.5, 0.5, 0.5, 1), 1.5, 0, 80, true);
	RENDER->RenderRect(hpImage, V2(90, 90), V2(0, 0.5), COLOR(1, 1, 1, 1), 1.5, 0, RECT{0, 0, (long)(hpImage->info.Width * (clamp(life, 0, 100) / maxMove)), (long)hpImage->info.Height}, 90, true);
	RENDER->RenderRect(hpPrev, V2(90, 90), V2(0, 0.5), COLOR(1, 1, 1, 1), 1.5, 0, RECT{ 0, 0, (long)(hpImage->info.Width * (clamp(prevlife, 0, 100) / maxMove)), (long)hpImage->info.Height }, 88, true);

	RENDER->Render(hpLine, V2(45, 170), V2(0, 0.5), COLOR(0.5, 0.5, 0.5, 1), 1.5, 0, 95, true);
	RENDER->Render(hpBack, V2(45, 170), V2(0, 0.5), COLOR(0.5, 0.5, 0.5, 1), 1.5, 0, 80, true);
	RENDER->RenderRect(hpMove, V2(45, 170), V2(0, 0.5), COLOR(1, 1, 1, 1), 1.5, 0, RECT{ 0, 0, (long)(hpImage->info.Width * (clamp(move, 0, 100) / maxMove)), (long)hpImage->info.Height }, 90, true);
	RENDER->RenderRect(hpPrev, V2(45, 170), V2(0, 0.5), COLOR(1, 1, 1, 1), 1.5, 0, RECT{ 0, 0, (long)(hpImage->info.Width * (clamp(prevMove, 0, 100) / maxMove)), (long)hpImage->info.Height }, 88, true);
}

void GameManager::UpdateTimer()
{
	timer += DT;
}

void GameManager::RenderTimer()
{
	WCHAR timerStr[256];
	swprintf(timerStr, L"%d : %02d", (int)timer / 60, (int)timer % 60);
	RENDER->Write(timerStr, V2(1750, 90), V2(0.5, 0.5), COLOR(1, 1, 1, 1), 1, 0, 100, true);
}

bool GameManager::IsClear()
{
	if (cured_percent >= 80)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GameManager::PrintText(wstring pText)
{
	text = pText;
	textColor = COLOR(1, 1, 1, 1);

	OBJECT->Add(new Instant(2.5,
		[=](LPINST a) {
			textColor.a = lerp(1.f, 0.f, a->percent);
		})
	);
}

void GameManager::CreatePlayer(V2 pPos)
{
	OBJECT->Add(new Instant(1)
	)->Add(new Instant(0.5, 
		[=](LPINST a) {
			CAMERA->pos = lerp(CAMERA->pos, InGrid(pPos), DT * 10);
			CAMERA->scale = lerp(CAMERA->scale, 4.f, 0.1);
		})
	)->Add(new Instant(1.5,
		[=](LPINST a) {

			V2 beginPos = pPos + toDirection(random(0.f, 360.f)) * random(8.f, 32.f);

			if (random(false, true))
			{
				float endScale = random(0.05f, 0.1f);

				OBJECT->Add(new Instant(1,
					[=](LPINST b) {
						b->image = IMAGE->Load(L"effect/circle");
						b->color = COLOR(1, 1, 1, 1);
						b->rotate = random(0.f, 360.f);
						b->scale = random(0.05f, 0.1f);
						b->depth = random(44, 46);
					},
					[=](LPINST b) {
						b->pos = InGrid(lerp(beginPos, pPos, b->percent));
						b->color.a = lerp(0.f, 1.f, b->percent);
						b->scale = lerp(0.f, endScale, b->percent);
					})
				);
			}
			else
			{
				OBJECT->Add(new Instant(0.5,
					[=](LPINST b) {
						b->image = IMAGE->Load(L"debris/(" + to_wstring(random(1, 6)) + L")");
						b->color = COLOR(0, 1, 0, 1);
						b->rotate = random(0.f, 360.f);
						b->scale = random(0.3f, 0.6f);
						b->depth = random(44, 46);
					},
					[=](LPINST b) {
						b->pos = InGrid(lerp(beginPos, pPos, b->percent));
						b->color.a = lerp(0.f, 1.f, b->percent);
					})
				);
			}

		})
	)->Add(new Instant(
		[=](LPINST a) {
			OBJECT->Add(new Player(pPos));
		})
	)->Add(new Instant(1.5)
	)->Add(new Instant(1,
		[=](LPINST a) {
			CAMERA->pos = lerp(CAMERA->pos, GRIDV2(160, 160), DT * 5);
			CAMERA->scale = lerp(CAMERA->scale, 1.f, 0.1);
		})
	);
}

LPDIRECTSOUNDBUFFER GameManager::PanPlaySound(wstring pPath, UINT pVolume, bool pIsLoop, V2* pX)
{
	auto& find = SOUND->sounds[pPath];

	if (!find)
	{
		CSound** sound = new CSound * ();

		WCHAR path[256];
		swprintf(path, L"sounds/%s.wav", pPath.c_str());

		if (SOUND->csmanager.Create(sound, path, DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN) == S_OK)
		{
			find = *sound;
		}

		SAFE_DELETE(sound);
	}

	LPDIRECTSOUNDBUFFER buffer = NULL;

	if (SOUND->csmanager.GetDirectSound()->DuplicateSoundBuffer(SOUND->sounds[pPath]->GetBuffer(NULL), &buffer) == S_OK)
	{
		buffer->Play(NULL, NULL, pIsLoop ? DSBPLAY_LOOPING : NULL);
		buffer->SetVolume(-2000 + pVolume);

		buffers.insert({ {pX, pVolume}, buffer });
	}
	else SAFE_DELETE(buffer);

	return buffer;
}

void GameManager::ItemUse(ItemType pType)
{
	Player* player = OBJECT->Search<Player>();

	if (player)
	{
		SOUND->Play(L"ui_sound_forward", 100, false);

		player->isInvicible = false;
		player->weapon = NoneWeapon;

		if (itemInst)
			itemInst->Destroy();

		switch (pType)
		{
		case SpeedItem:
			PrintText(L"Used Speed Item");

			player->speed = min(5, player->speed + 1);
			
			break;
		case InvicibleItem:
			PrintText(L"Used Invincible Item");
			player->isInvicible = true;

			OBJECT->Add(new Instant(3,
				[=](LPINST a) {
					if (player)
						player->isInvicible = false;
				})
			);
			break;
		case LifeItem:
			PrintText(L"Used Life Item");
			GAME->life += 35;

			if (GAME->life > GAME->maxLife)
			{
				GAME->life = GAME->maxLife;
				GAME->score += 100000;
			}
			break;
		case Shoot1Item:
			player->weapon = Weapon1;
			itemInst = OBJECT->Add(new Instant(8)
			)->Add(new Instant(
				[=](LPINST a) {
					if (player)
						player->weapon = NoneWeapon;
				})
			);
			PrintText(L"Used Shoot 1 Item");
			break;
		case Shoot2Item:
			player->weapon = Weapon2;
			itemInst = OBJECT->Add(new Instant(8)
			)->Add(new Instant(
				[=](LPINST a) {
					if (player)
						player->weapon = NoneWeapon;
				})
			);
			PrintText(L"Used Shoot 2 Item");
			break;
		case Shoot3Item:
			player->weapon = Weapon3;
			itemInst = OBJECT->Add(new Instant(8)
			)->Add(new Instant(
				[=](LPINST a) {
					if (player)
						player->weapon = NoneWeapon;
				})
			);
			PrintText(L"Used Shoot 3 Item");
			break;
		case Shoot4Item:
			player->weapon = Weapon4;
			itemInst = OBJECT->Add(new Instant(8)
			)->Add(new Instant(
				[=](LPINST a) {
					if (player)
						player->weapon = NoneWeapon;
				})
			);
			PrintText(L"Used Shoot 4 Item");
			break;
		}
	}
}
