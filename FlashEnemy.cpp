#include "DXUT.h"
#include "FlashEnemy.h"

void FlashEnemy::Init()
{
	image = IMAGE->Load(L"enemy/flash");

	Tilemap* tilemap = OBJECT->Search<Tilemap>();
	do
	{
		pos.x = random(20, 300);
		pos.y = random(20, 300);
	} while (tilemap->GetCell(pos) != EmptyCell);

	direction = toDirection(random(0.f, 360.f));
	rotate = toRotate(direction);
	speed = 60;

	radius = image->info.Width / 6;
	crashTime = 0;

	CreateEffect();

	OBJECT->Add(new Instant(9)
	)->Add(new Instant(
		[=](LPINST a) {
			flash = OBJECT->Add(new Instant(9, true,
				[=](LPINST a) {
					V2 playerPos = pos;

					isAppeared = false;

					OBJECT->Add(new Instant(1.5,
						[=](LPINST b) {
						},
						[=](LPINST b) {
							color.a = lerp(1.f, 0.f, b->percent);

							V2 beginPos = pos;
							V2 endPos = pos + toDirection(random(0.f, 360.f)) * random(8.f, 32.f);

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
										b->pos = InGrid(lerp(beginPos, endPos, b->percent));
										b->color.a = lerp(1.f, 0.f, b->percent);
										b->scale = lerp(endScale, 0.f, b->percent);
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
										b->pos = InGrid(lerp(beginPos, endPos, b->percent));
										b->color.a = lerp(1.f, 0.f, b->percent);
									})
								);
							}
						})
					)->Add(new Instant(1.5,
						[=](LPINST b) {
							Tilemap* tilemap = OBJECT->Search<Tilemap>();
							do
							{
								pos.x = random(20, 300);
								pos.y = random(20, 300);
							} while (tilemap->GetCell(pos) != EmptyCell);
						},
						[=](LPINST b) {
							for (int i = 0; i < 3; i++)
							{
								V2 beginPos = lerp(playerPos, pos, b->percent);
								V2 endPos = lerp(playerPos, pos, b->percent) + toDirection(random(0.f, 360.f)) * random(8.f, 32.f);

								float endScale = random(0.05f, 0.1f);

								OBJECT->Add(new Instant(1,
									[=](LPINST b) {
										b->image = IMAGE->Load(L"effect/circle");
										b->color = COLOR(1, 0, 0, 1);
										b->rotate = random(0.f, 360.f);
										b->depth = random(44, 46);
									},
									[=](LPINST b) {
										b->pos = InGrid(lerp(beginPos, endPos, b->percent));
										b->color.a = lerp(1.f, 0.f, b->percent);
										b->scale = lerp(endScale, 0.f, b->percent);
									})
								);
							}
						})
					)->Add(new Instant(1.5,
						[=](LPINST b) {},
						[=](LPINST b) {
							color.a = lerp(0.f, 1.f, b->percent);

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
					)->Add(new Instant(
						[=](LPINST b) {
							isAppeared = true;
						})
					);
				},
				[=](LPINST a) {})
			);
		})
	);
}

void FlashEnemy::Release()
{
	if (flash)
		flash->Destroy();

	Enemy::DestroyEffect();
}

void FlashEnemy::Update()
{
	if (isAppeared)
	{
		Enemy::Movement(540 * side);
		Enemy::IsDamaged();
		Enemy::IsDelete();
		Enemy::ImageDirection(direction);
	}
}

void FlashEnemy::Render()
{
	RENDER->Render(image, InGrid(pos), V2(0.5, 0.5), color, scale, rotate, 80, false);
}

void FlashEnemy::CollisionEvent()
{
}
