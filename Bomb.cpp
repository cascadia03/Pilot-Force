#include "DXUT.h"
#include "Bomb.h"

void Bomb::Init()
{
	image = IMAGE->Load(L"bomb");
}

void Bomb::Release()
{
	if (blinkInst)
	{
		blinkInst->Destroy();
	}
}

void Bomb::Update()
{
	Tilemap* tilemap = OBJECT->Search<Tilemap>();

	V2 moved = pos + direction * (3 - timer) * 100 * DT;

	for (float i = toRotate(direction) - 90; i < toRotate(direction) + 90; i += 15)
	{
		V2 toward = toDirection(i) * image->info.Width/6;

		if (tilemap->GetCell(moved + toward) == FilledCell ||
			tilemap->GetCell(moved + toward) == LinedCell ||
			tilemap->GetCell(moved + toward) == OutOfCell)
		{
			bool isH = abs(toward.x) > (image->info.Width / 6) * 0.66;
			bool isV = abs(toward.y) > (image->info.Width / 6) * 0.66;

			if (isH) direction.x = abs(direction.x) * -getSign(toward.x);
			if (isV) direction.y = abs(direction.y) * -getSign(toward.y);
			if (isH || isV)
			{
				GAME->PanPlaySound(L"SE_Hit", 100, false, &(V2(pos)));

				break;
			}
		}
	}

	pos += direction * (3 - timer) * 25 * DT;
	rotate += (3 - timer) * 25 * DT;

	timer += DT;
	if (timer >= 3)
	{
		Image* circleImage = IMAGE->Load(L"effect/circle");

		OBJECT->Add(new Instant(3, false,
			[=](LPINST a) {
				a->image = circleImage;
				a->pos = InGrid(pos);
				a->color = COLOR(1, 1, 1, 1);
				a->depth = 90;
				a->scale = 0;
			},
			[=](LPINST a) {
				a->scale = lerp(a->scale, 4.f, 0.01);
				a->color.a = lerp(1.f, 0.f, a->percent);
			})
		);

		CAMERA->Shake(16, 0.5);
		for (auto i : OBJECT->SearchAll<Enemy>())
		{
			if (toLength(i->pos - pos) < i->radius + circleImage->info.Width &&
				i->isAppeared)
			{
				i->hp -= 20;
				OBJECT->Add(new Instant(0.5,
					[=](LPINST a) {
						i->color = lerp(COLOR(0.5, 0.5, 0.5, 1), COLOR(1, 1, 1, 1), a->percent);
					})
				);
				Destroy();
			}
		}

		Destroy();
	}
}

void Bomb::Render()
{
	RENDER->Render(image, InGrid(pos), V2(0.5, 0.5), COLOR(0, 1, 0, 1), 1, rotate, 60, false);
}
