#include "DXUT.h"
#include "Item.h"

void Item::Init()
{
	type = (ItemType)random(0, 6);
	image = IMAGE->Load(L"item/(" + to_wstring((int)type) + L")");

	Tilemap* tilemap = OBJECT->Search<Tilemap>();
	do
	{
		pos.x = random(0, 320);
		pos.y = random(0, 320);
	} while (tilemap->GetCell(pos) != EmptyCell);

	rotate = random(0.f, 360.f);

	CreateEffect();
}

void Item::Release()
{
	if (wave)
		wave->Destroy();
}

void Item::Update()
{
	Player* player = OBJECT->Search<Player>();

	if (player)
	{
		if (toLength(player->pos - pos) <= image->info.Width/6)
		{
			Destroy();
			GAME->ItemUse(type);
		}
	}

	Tilemap* tilemap = OBJECT->Search<Tilemap>();

	if (tilemap->GetCell(pos) == FilledCell ||
		tilemap->GetCell(pos) == OutOfCell)
	{
		Destroy();
		GAME->ItemUse(type);
	}

	rotate += 30 * DT;
}

void Item::Render()
{
	RENDER->Render(image, InGrid(pos), V2(0.5, 0.5), COLOR(1, 1, 1, 1), scale, rotate, 80, false);
}

void Item::CreateEffect()
{
	OBJECT->Add(new Instant(1,
		[=](LPINST a) {

			V2 beginPos = pos + toDirection(random(0.f, 360.f)) * random(8.f, 32.f);

			float endScale = random(0.05f, 0.1f);

			OBJECT->Add(new Instant(0.5,
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

		})
	)->Add(new Instant(1,
		[=](LPINST a) {
			scale = lerp(0.f, 0.5f, a->percent);
		})
	)->Add(new Instant(0.5)
	)->Add(new Instant(1,
		[=](LPINST a) {
			a->image = IMAGE->Load(L"effect/empty circle");
			a->pos = InGrid(pos);
			a->color = COLOR(1, 1, 1, 1);
			a->depth = 48;

			isAppeared = true;
		},
		[=](LPINST a) {
			a->scale = lerp(0.f, 0.5f, a->percent);
			a->color.a = lerp(1.f, 0.f, a->percent);
		})
	)->Add(new Instant(
		[=](LPINST a) {
			wave = OBJECT->Add(new Instant(2, true,
				[=](LPINST a) {
					a->image = IMAGE->Load(L"effect/empty circle");
					a->pos = InGrid(pos);
					a->color = COLOR(1, 1, 1, 1);
					a->depth = 48;
					a->scale = 0.f;

					isAppeared = true;
				},
				[=](LPINST a) {
					a->scale = lerp(0.f, 0.5f, a->percent);
					a->color.a = lerp(1.f, 0.f, a->percent); 
				})
			);
		})
	);
}


