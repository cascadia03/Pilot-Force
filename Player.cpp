#include "DXUT.h"
#include "Player.h"

void Player::Init()
{
	OBJECT->Add(new Instant(1,
		[=](LPINST a) {
			scale = lerp(0.f, 1.f, a->percent);
		})
	)->Add(new Instant(0.5)
	)->Add(new Instant(1,
		[=](LPINST a) {
			a->image = IMAGE->Load(L"effect/empty circle");
			a->pos = InGrid(pos);
			a->color = COLOR(0, 1, 0, 1);
			a->depth = 48;

			isAppeared = true;
			SOUND->Play(L"TD Tower Upgrade", 300, false);
		},
		[=](LPINST a) {
			a->scale = lerp(0.f, 2.f, a->percent);
			a->color.a = lerp(1.f, 0.f, a->percent);
		})
	);
}

void Player::Release()
{
}

void Player::Update()
{
	if (!tilemap)
		tilemap = OBJECT->Search<Tilemap>();

	if (isAppeared)
	{
		SetDirection();

		if (status != DamagedStatus)
		{
			for (int i = 0; i < speed; i++)
			{
				switch (status)
				{
				case DefStatus:
					DefStatusEvent();
					break;
				case DrawStatus:
					DrawStatusEvent();
					break;
				case RetraceStatus:
					RetraceStatusEvent();
				}
			}
		}
		else
		{
			DamagedStatusEvent();
		}
	}

	if (GAME->life <= 0)
	{
		status = DamagedStatus;
	}

	if (isInvicible)
	{
		color = COLOR(1, 1, 0, 1);
	}
	else
	{
		color = COLOR(1, 1, 1, 1);
	}

	if (status == DrawStatus)
	for (auto i : pathes)
	{
		if (random(1, 100) == 1)
		{
			V2 beginPos = i.first;
			V2 endPos = i.first + toDirection(random(0.f, 360.f)) * random(16.f, 32.f);

			OBJECT->Add(new Instant(1,
				[=](LPINST a) {
					a->image = IMAGE->Load(L"effect/square");
					a->scale = 0.025;
					a->pos = beginPos;
					a->color = COLOR(0, 1, 0, 1);
					a->depth = 45;
				},
				[=](LPINST a) {
					a->pos = InGrid(lerp(beginPos, endPos, a->percent));
					a->color.a = lerp(1.f, 0.f, a->percent);
				})
			);
		}
	}

	if (INPUT->KeyPress(VK_SPACE) &&
		weapon != NoneWeapon &&
		weaponTime <= 0)
	{
		switch (weapon)
		{
		case Weapon1:
			Weapon1Event();
			weaponTime = 0.15;
			break;
		case Weapon2:
			Weapon2Event();
			weaponTime = 0.2;
			break;
		case Weapon3:
			Weapon3Event();
			break;
		case Weapon4:
			Weapon4Event();
			weapon = NoneWeapon;
			break;
		}
	}
	else
	{
		weaponTime -= DT;
		isLaser = false;
	}
}

void Player::Render()
{
	auto playerImage = IMAGE->Load(L"player");

	RENDER->Render(playerImage, InGrid(pos), V2(0.5, 0.5), color, scale, rotate, 50, false);

	if (isLaser)
	{
		auto laserImage = IMAGE->Load(L"laser");

		RENDER->Render(laserImage, InGrid(pos), V2(0, 0.5), color, scale, rotate, 49, false);
	}
}

void Player::SetDirection()
{
	direction = V2(0, 0);

	if (INPUT->KeyPress(VK_LEFT))
		direction = V2(-1, 0);
	if (INPUT->KeyPress(VK_RIGHT))
		direction = V2(1, 0);
	if (INPUT->KeyPress(VK_UP))
		direction = V2(0, -1);
	if (INPUT->KeyPress(VK_DOWN))
		direction = V2(0, 1);

	if (direction != V2(0, 0))
		rotate = toRotate(direction);
}

void Player::DefStatusEvent()
{
	GAME->life = max(0, GAME->life - DT);

	GAME->move = min(100, GAME->move + DT * 10);

	if ((weapon == Weapon1 ||
		weapon == Weapon2 ||
		weapon == Weapon3 )&&
		tilemap->GetCell(pos + direction) == FilledCell)
	{
		pathes.push_back({ pos, rotate });
		pos += direction;
	}
	else
	{
		if (GAME->life > 0 && tilemap->GetCell(pos + direction) == LinedCell)
		{
			for (int y = -1; y <= 1; y++)
				for (int x = -1; x <= 1; x++)
				{
					if (tilemap->GetCell(pos + direction + V2(x, y)) == EmptyCell)
					{
						pos += direction;

						pathes.clear();

						goto OUT_OF_MOVE;
					}
				}

		OUT_OF_MOVE:;
		}
		else if (pathes.size() > 0)
		{
			pos = pathes.front().first;
			pathes.clear();
		}

		if (GAME->move > 0 && (
			(tilemap->GetCell(pos + direction) == EmptyCell ||
				tilemap->GetCell(pos + direction) == ToxicedCell) &&
			INPUT->KeyPress(VK_SHIFT)))
		{
			GAME->move = max(0, GAME->move - DT * 10);

			pathes.clear();
			pathes.push_back({ pos, rotate });

			pos += direction;
			pathes.push_back({ pos, rotate });
			tilemap->SetCell(pos, DrawedCell);

			status = DrawStatus;
		}
	}
}

void Player::DrawStatusEvent()
{
	V2 check = V2(0, 0);

	if (pathes.size() > 2 && (
		tilemap->GetCell(pos + (check = V2(-1, 0))) == LinedCell ||
		tilemap->GetCell(pos + (check = V2(1, 0))) == LinedCell ||
		tilemap->GetCell(pos + (check = V2(0, -1))) == LinedCell ||
		tilemap->GetCell(pos + (check = V2(0, 1))) == LinedCell))
	{
		Tilemap* iTilemap = new Tilemap(NULL);
		memcpy(iTilemap->grid, tilemap->grid, sizeof(iTilemap->grid));

		int result = 0;

		if (check.x == 0)
		{
		HORIZONTAL:
			int left = iTilemap->FloodFill(pos + V2(-1, 0));
			int right = iTilemap->FloodFill(pos + V2(1, 0));

			if (left < right)
			{
				result = tilemap->FloodFill(pos + V2(-1, 0));
				goto OUTOFCHECK;
			}
			else
			{
				result = tilemap->FloodFill(pos + V2(1, 0));
				goto OUTOFCHECK;
			}

			goto VERTICAL;
		}
		else
		{
		VERTICAL:
			int top = iTilemap->FloodFill(pos + V2(0, -1));
			int bottom = iTilemap->FloodFill(pos + V2(0, 1));

			if (top < bottom)
			{
				result = tilemap->FloodFill(pos + V2(0, -1));
				goto OUTOFCHECK;
			}
			else
			{
				result = tilemap->FloodFill(pos + V2(0, 1));
				goto OUTOFCHECK;
			}

			goto HORIZONTAL;
		}
	OUTOFCHECK:;

		GAME->cured_percent += (result / (320.f * 320.f)) * 100;
		GAME->score += (int)(result / (320.f * 320.f) * 10000) * 100;

		OBJECT->Add(new Instant(1,
			[=](LPINST a) {
				GAME->uiScale = lerp(1.2f, 1.f, a->percent);
				GAME->uiColor = lerp(COLOR(1, 1, 0, 1), COLOR(1, 1, 1, 1), a->percent);
			})
		);

		SAFE_DELETE(iTilemap);

		for (auto i : pathes)
		{
			tilemap->SetCell(i.first, LinedCell);

			if (random(false, true))
			{
				V2 beginPos = i.first;
				V2 endPos = i.first + toDirection(random(0.f, 360.f)) * random(16.f, 32.f);

				OBJECT->Add(new Instant(1,
					[=](LPINST a) {
						a->image = IMAGE->Load(L"effect/square");
						a->scale = 0.025;
						a->pos = beginPos;
						a->color = COLOR(1, 1, 1, 1);
						a->depth = 45;
					},
					[=](LPINST a) {
						a->pos = InGrid(lerp(beginPos, endPos, a->percent));
						a->color.a = lerp(1.f, 0.f, a->percent);
					})
				);
			}
		}
		pathes.clear();

		pos += check;
		
		SOUND->Play(L"ui_sound_forward", 100, false);

		status = DefStatus;
	}
	else
	{
		if (!INPUT->KeyPress(VK_SHIFT) || GAME->move <= 0)
		{
			status = RetraceStatus;
		}
		else
		if (GAME->move > 0 && (
			tilemap->GetCell(pos + direction) == EmptyCell &&
			tilemap->GetCell(pos + direction * 2) != DrawedCell &&
			tilemap->GetCell(pos + direction + toDirection(toRotate(direction) - 90)) != DrawedCell &&
			tilemap->GetCell(pos + direction + toDirection(toRotate(direction) + 90)) != DrawedCell))
		{
			GAME->move = max(0, GAME->move - DT * 10);

			pos += direction;
			pathes.push_back({ pos, rotate });
			tilemap->SetCell(pos, DrawedCell);
		}
	}
}

void Player::RetraceStatusEvent()
{
	for (int i = 0; i < 4; i++)
	{
		if (pathes.size() > 0)
		{
			auto back = pathes.back();

			pos = back.first;
			rotate = back.second;

			if (tilemap->GetCell(back.first) == DrawedCell)
				tilemap->SetCell(back.first, EmptyCell);

			pathes.pop_back();
		}
		else
		{
			status = DefStatus;

			break;
		}
	}
}

void Player::DamagedStatusEvent()
{
	SOUND->Stop(L"Game Music");
	SOUND->Stop(L"SecurityRoomAmbient");

	SOUND->Play(L"ui_sound_back", 100, false);

	for (int i = 0; i < 36; i++)
	{
		V2 beginPos = pos;
		V2 endPos = pos + toDirection(random(0.f, 360.f)) * random(8.f, 32.f);

		if (random(false, true))
		{
			float beginScale = random(0.05f, 0.1f);

			OBJECT->Add(new Instant(random(0.8f, 1.2f),
				[=](LPINST b) {
					b->image = IMAGE->Load(L"effect/circle");
					b->color = COLOR(1, 1, 1, 1);
					b->rotate = random(0.f, 360.f);
					b->depth = random(44, 46);
				},
				[=](LPINST b) {
					b->pos = InGrid(lerp(beginPos, endPos, b->percent));
					b->scale = lerp(beginScale, 0.f, b->percent);
					b->color.a = lerp(1.f, 0.f, b->percent);
				})
			);
		}
		else
		{
			OBJECT->Add(new Instant(random(0.4f, 0.6f),
				[=](LPINST b) {
					b->image = IMAGE->Load(L"debris/(" + to_wstring(random(1, 6)) + L")");
					b->color = COLOR(0, 1, 0, 1);
					b->rotate = random(0.f, 360.f);
					b->scale = random(0.3f, 0.6f);
					b->depth = random(44, 46);
				},
				[=](LPINST b) {
					b->pos = InGrid(lerp(beginPos, endPos, b->percent));
					b->color.a = lerp(0.5f, 0.f, b->percent);
				})
			);
		}
	}

	V2 playerPos = pos;
	V2 spawnPos = pathes.front().first;

	OBJECT->Add(new Instant(0.5,
		[=](LPINST a) {
			CAMERA->pos = lerp(CAMERA->pos, InGrid(playerPos), DT * 10);
			CAMERA->scale = lerp(CAMERA->scale, 4.f, 0.1);
		})
	);

	OBJECT->Add(new Instant(1,
		[=](LPINST a) {
			a->image = IMAGE->Load(L"effect/empty circle");
			a->pos = InGrid(pos);
			a->color = COLOR(0, 1, 0, 1);
			a->depth = 48;

			isAppeared = true;
		},
		[=](LPINST a) {
			a->scale = lerp(0.f, 2.f, a->percent);
			a->color.a = lerp(1.f, 0.f, a->percent);
		})
	);
	
	for (auto i : pathes)
	{
		if (tilemap->GetCell(i.first) == DrawedCell)
			tilemap->SetCell(i.first, EmptyCell);

		if (random(1, 10) == 1)
		{
			V2 beginPos = i.first;
			V2 endPos = i.first + toDirection(random(0.f, 360.f)) * random(16.f, 32.f);

			OBJECT->Add(new Instant(1,
				[=](LPINST a) {
					a->image = IMAGE->Load(L"effect/square");
					a->scale = 0.025;
					a->pos = beginPos;
					a->color = COLOR(1, 1, 1, 1);
					a->depth = 45;
				},
				[=](LPINST a) {
					a->pos = InGrid(lerp(beginPos, endPos, a->percent));
					a->color.a = lerp(1.f, 0.f, a->percent);
				})
			);
		}
	}
	pathes.clear();

	Destroy();

	OBJECT->Add(new Instant(2,
		[=](LPINST a) {
			SOUND->Play(L"TD Defeat", 100, false);

		},
		[=](LPINST a) {
		
		})
	)->Add(new Instant(
		[=](LPINST a) {

			SCENE->Load<ScoreScene>({960, 540}, 1);
		})
	);

	status = NoneStatus;
}

void Player::Weapon1Event()
{
	if (tilemap->GetCell(pos + toDirection(rotate)) == EmptyCell)
		OBJECT->Add(new Bullet(pos + toDirection(rotate) * 3, rotate));
}

void Player::Weapon2Event()
{
	if (tilemap->GetCell(pos + toDirection(rotate)) == EmptyCell)
		OBJECT->Add(new Missile(pos + toDirection(rotate) * 3, rotate));
}

void Player::Weapon3Event()
{
	isLaser = INPUT->KeyPress(VK_SPACE);

	for (int i = 0; i < 8; i++)
	{
		V2 beginPos = pos + toDirection(rotate) * random(0.f, 2400.f);
		V2 endPos = beginPos + toDirection(random(0.f, 360.f)) * random(16.f, 32.f);

		OBJECT->Add(new Instant(1,
			[=](LPINST a) {
				a->image = IMAGE->Load(L"effect/square");
				a->scale = 0.05;
				a->pos = beginPos;
				a->color = COLOR(1, 0, 0, 1);
				a->depth = 45;
			},
			[=](LPINST a) {
				a->pos = InGrid(lerp(beginPos, endPos, a->percent));
				a->color.a = lerp(1.f, 0.f, a->percent);
			})
		);
	}

	for (auto i : OBJECT->SearchAll<Enemy>())
	{
		float distance = toLength(i->pos - pos);
		V2 foot = pos + toDirection(rotate) * distance;

		if (toLength(i->pos - foot) <= i->radius)
		{
			i->hp -= DT;
			OBJECT->Add(new Instant(0.5,
				[=](LPINST a) {
					i->color = lerp(COLOR(0.5, 0.5, 0.5, 1), COLOR(1, 1, 1, 1), a->percent);
				})
			);

			for (int j = 0; j < 3; j++)
			{
				V2 beginPos = lerp(foot, i->pos, 0.5);
				V2 endPos = beginPos + toDirection(random(0.f, 360.f)) * random(16.f, 32.f);

				OBJECT->Add(new Instant(random(0.8f, 1.2f),
					[=](LPINST a) {
						a->image = IMAGE->Load(L"effect/square");
						a->scale = 0.05;
						a->pos = beginPos;
						a->color = COLOR(1, 0, 0, 1);
						a->depth = 45;
					},
					[=](LPINST a) {
						a->pos = InGrid(lerp(beginPos, endPos, a->percent));
						a->color.a = lerp(1.f, 0.f, a->percent);
					})
				);
			}
		}
	}
}

void Player::Weapon4Event()
{
	if (tilemap->GetCell(pos + toDirection(rotate)) == EmptyCell)
		OBJECT->Add(new Bomb(pos + toDirection(rotate) * 3, rotate));
}
