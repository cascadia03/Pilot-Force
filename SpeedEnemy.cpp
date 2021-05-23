#include "DXUT.h"
#include "SpeedEnemy.h"

void SpeedEnemy::Init()
{
	image = IMAGE->Load(L"enemy/speed");

	Tilemap* tilemap = OBJECT->Search<Tilemap>();
	do
	{
		pos.x = random(20, 300);
		pos.y = random(20, 300);
	} while (tilemap->GetCell(pos) != EmptyCell);

	direction = toDirection(random(0.f, 360.f));
	rotate = toRotate(direction);
	speed = 90;

	radius = image->info.Width / 6;
	crashTime = 0;

	CreateEffect();
}

void SpeedEnemy::Release()
{
	Enemy::DestroyEffect();
}

void SpeedEnemy::Update()
{
	if (isAppeared)
	{
		Enemy::Movement(120);
		Enemy::IsDamaged();
		Enemy::IsDelete();
		Enemy::ImageDirection(direction);
	}
}

void SpeedEnemy::Render()
{
	RENDER->Render(image, InGrid(pos), V2(0.5, 0.5), color, scale, rotate, 80, false);
}

void SpeedEnemy::CollisionEvent()
{
	OBJECT->Add(new Instant(1.5,
		[=](LPINST a) {
			speed = lerp(150, 90, a->percent);
			color = lerp(COLOR(1, 0, 0, 1), COLOR(1, 1, 1, 1), a->percent);
			scale = lerp(1.2f, 1.f, a->percent);
		})
	);
}
