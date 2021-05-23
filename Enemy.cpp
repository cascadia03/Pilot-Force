#include "DXUT.h"
#include "Enemy.h"

void Enemy::Movement(float pRotate)
{
	Tilemap* tilemap = OBJECT->Search<Tilemap>();

	direction = toDirection(toRotate(direction) + pRotate * 0.1 * DT);

	V2 moved = pos + direction * speed * DT;

	for (float i = toRotate(direction) - 90; i <= toRotate(direction) + 90; i += 15)
	{
		V2 toward = toDirection(i) * radius;

		if (tilemap->GetCell(moved + toward) == LinedCell)
		{
			bool isH = abs(toward.x) > radius * 0.5;
			bool isV = abs(toward.y) > radius * 0.5;

			if (isH) direction.x = abs(direction.x) * -getSign(toward.x);
			if (isV) direction.y = abs(direction.y) * -getSign(toward.y);
			if (isH || isV)
			{
				for (int i = 0; i < 36; i++)
				{
					V2 beginPos = moved + toward;
					V2 endPos = moved + toward + toDirection(random(0.f, 360.f)) * random(8.f, 32.f);
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
						})
					);
				}

				GAME->PanPlaySound(L"SE_Hit", 100, false, &(V2(pos)));

				CollisionEvent();
				goto SKIP_MOVE;
			}
		}
	}
SKIP_MOVE:;

	pos += direction * speed * DT;

}

void Enemy::IsDelete()
{
	Tilemap* tilemap = OBJECT->Search<Tilemap>();

	if (tilemap->GetCell(pos) == FilledCell ||
		tilemap->GetCell(pos) == OutOfCell ||
		hp <= 0)
	{
		Destroy();

		GAME->life += 10;
		GAME->maxLife += 10;
		GAME->life += 10;
		GAME->maxMove += 10;

		GAME->score += 100000;
	}
}

void Enemy::IsDamaged()
{
	if (crashTime <= 0)
	{
		Player* player = OBJECT->Search<Player>();

		if (player &&
			player->status == DrawStatus &&
			player->isInvicible == false &&
			GAME->isOverpowered == false)
		{
			for (auto i : player->pathes)
			{
				if (toLength(i.first - pos) <= radius)
				{
					SOUND->Play(L"ui_sound_back", 100, false);

					player->status = RetraceStatus;

					CAMERA->Shake(12, 0.25);
					GAME->life -= 20;
					crashTime = 1.5;
					break;
				}
			}
		}
	}
	else crashTime -= DT;
}

void Enemy::ImageRotate(float pSpeed)
{
	rotate += pSpeed * DT;
}

void Enemy::ImageDirection(V2 pDirection)
{
	rotate = toRotate(pDirection);
}

void Enemy::CreateEffect()
{
	OBJECT->Add(new Instant(1.5,
		[=](LPINST a) {

			V2 beginPos = pos + toDirection(random(0.f, 360.f)) * random(8.f, 32.f);

			if (random(false, true))
			{
				float endScale = random(0.05f, 0.1f);

				OBJECT->Add(new Instant(1,
					[=](LPINST b) {
						b->image = IMAGE->Load(L"effect/circle");
						b->color = COLOR(1, 1, 1, 1);
						b->rotate = random(0.f, 360.f);
						b->depth = random(44, 46);
					},
					[=](LPINST b) {
						b->pos = InGrid(lerp(beginPos, pos, b->percent));
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
						b->color = COLOR(1, 0, 0, 1);
						b->rotate = random(0.f, 360.f);
						b->scale = random(0.3f, 0.6f);
						b->depth = random(44, 46);
					},
					[=](LPINST b) {
						b->pos = InGrid(lerp(beginPos, pos, b->percent));
						b->color.a = lerp(0.f, 1.f, b->percent);
					})
				);
			}

		})
	)->Add(new Instant(1,
		[=](LPINST a) {
			scale = lerp(0.f, 1.f, a->percent);
		})
	)->Add(new Instant(0.5)
	)->Add(new Instant(1,
		[=](LPINST a) {
			a->image = IMAGE->Load(L"effect/empty circle");
			a->pos = InGrid(pos);
			a->color = COLOR(1, 0, 0, 1);
			a->depth = 48;

			isAppeared = true;
		},
		[=](LPINST a) {
			a->scale = lerp(0.f, 1.f, a->percent);
			a->color.a = lerp(1.f, 0.f, a->percent);
		})
	)->Add(new Instant(
		[=](LPINST a) {
			wave = OBJECT->Add(new Instant(2, true,
				[=](LPINST a) {
					a->image = IMAGE->Load(L"effect/empty circle");
					a->pos = InGrid(pos);
					a->color = COLOR(1, 0, 0, 1);
					a->depth = 48;
					a->scale = 0.f;

					isAppeared = true;
				},
				[=](LPINST a) {
					a->pos = InGrid(pos);
					a->scale = lerp(0.f, 1.f, a->percent * 2);
					a->color.a = lerp(1.f, 0.f, a->percent * 2);
				})
			);

			buffer = GAME->PanPlaySound(L"TD EMP Idle", 10, true, &pos);
		})
	);
}

void Enemy::DestroyEffect()
{
	for (int i = 0; i < 36; i++)
	{
		V2 beginPos = pos;
		V2 endPos = pos + toDirection(random(0.f, 360.f)) * random(8.f, 32.f);

		if (random(false, true))
		{
			float endScale = random(0.05f, 0.1f);

			OBJECT->Add(new Instant(random(0.8f, 1.2f),
				[=](LPINST b) {
					b->image = IMAGE->Load(L"effect/circle");
					b->color = COLOR(1, 1, 1, 1);
					b->rotate = random(0.f, 360.f);
					b->depth = random(44, 46);
				},
				[=](LPINST b) {
					b->pos = InGrid(lerp(beginPos, pos, b->percent));
					b->color.a = lerp(0.f, 1.f, b->percent);
					b->scale = lerp(0.f, endScale, b->percent);
				})
			);
		}
		else
		{
			OBJECT->Add(new Instant(random(0.4f, 0.6f),
				[=](LPINST b) {
					b->image = IMAGE->Load(L"debris/(" + to_wstring(random(1, 6)) + L")");
					b->color = COLOR(1, 0, 0, 1);
					b->rotate = random(0.f, 360.f);
					b->scale = random(0.3f, 0.6f);
					b->depth = random(44, 46);
				},
				[=](LPINST b) {
					b->pos = InGrid(lerp(beginPos, endPos, b->percent));
					b->color.a = lerp(1.f, 0.f, b->percent);
				})
			);
		}
	}

	OBJECT->Add(new Instant(1,
		[=](LPINST a) {
			a->image = IMAGE->Load(L"effect/empty circle");
			a->pos = InGrid(pos);
			a->color = COLOR(1, 0, 0, 1);
			a->depth = 48;
		},
		[=](LPINST a) {
			a->scale = lerp(0.f, 1.f, a->percent);
			a->color.a = lerp(1.f, 0.f, a->percent);
		})
	)->Add(new Instant(
		[=](LPINST a) {
			OBJECT->Add(new Item());
		})
	);
}
