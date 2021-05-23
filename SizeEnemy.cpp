#include "DXUT.h"
#include "SizeEnemy.h"

void SizeEnemy::Init()
{
	image = IMAGE->Load(L"enemy/size");

	Tilemap* tilemap = OBJECT->Search<Tilemap>();
	do
	{
		pos.x = random(20, 300);
		pos.y = random(20, 300);
	} while (tilemap->GetCell(pos) != EmptyCell);

	rotate = random(0.f, 360.f);
	direction = toDirection(random(0.f, 360.f));
	speed = 30;

	radius = image->info.Width/4;
	crashTime = 0;

	CreateEffect();
}

void SizeEnemy::Release()
{
	Enemy::DestroyEffect();
}

void SizeEnemy::Update()
{
	if (isAppeared)
	{
		Enemy::Movement(30);
		Enemy::IsDamaged();
		Enemy::IsDelete();
		Enemy::ImageRotate(30);
	}
}

void SizeEnemy::Render()
{
	RENDER->Render(image, InGrid(pos), V2(0.5, 0.5), color, scale, rotate, 80, false);
}

void SizeEnemy::CollisionEvent()
{
}
