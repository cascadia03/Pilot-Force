#include "DXUT.h"
#include "Missile.h"

void Missile::Init()
{
	image = IMAGE->Load(L"effect/circle");
}

void Missile::Release()
{
	for (int i = 0; i < 12; i++)
	{
		V2 beginPos = pos;
		V2 endPos = pos + toDirection(random(0.f, 360.f)) * random(16.f, 32.f);

		OBJECT->Add(new Instant(random(0.8f, 1.2f),
			[=](LPINST a) {
				a->image = IMAGE->Load(L"effect/square");
				a->scale = 0.025;
				a->pos = beginPos;
				a->color = COLOR(0, 0, 1, 1);
				a->depth = 45;
			},
			[=](LPINST a) {
				a->pos = InGrid(lerp(beginPos, endPos, a->percent));
				a->color.a = lerp(1.f, 0.f, a->percent);
			})
		);
	}
}

void Missile::Update()
{
	if (homingTimer > 0)
	{
		homingTimer -= DT;

		if (!target)
		{
			for (auto i : OBJECT->SearchAll<Enemy>())
			{
				if (toLength(i->pos - pos) <= 120 &&
					i->isAppeared)
					target = i;
			}
		}
		else
		{
			direction *= 240;
			V2 toward = target->pos - pos;
			direction += toNoramlize(toward) * 45;
			direction = toNoramlize(direction);
		}
	}

	OBJECT->Add(new Instant(0.5,
		[=](LPINST a) {
			a->image = IMAGE->Load(L"effect/circle");
			a->color = COLOR(0, 0, 1, 1);
			a->depth = 44;
			a->pos = InGrid(pos);
		},
		[=](LPINST a) {
			a->color.a = lerp(0.5f, 0.f, a->percent);
			a->scale = lerp(0.1f, 0.f, a->percent);
		})
	);

	pos += direction * 240 * DT;

	Tilemap* tilemap = OBJECT->Search<Tilemap>();

	if (tilemap->GetCell(pos) != EmptyCell)
	{
		Destroy();
	}

	for (auto i : OBJECT->SearchAll<Enemy>())
	{
		if (toLength(i->pos - pos) < i->radius + image->info.Width / 4 * 0.1 &&
			i->isAppeared)
		{
			i->hp--;
			OBJECT->Add(new Instant(0.5,
				[=](LPINST a) {
					i->color = lerp(COLOR(0.5, 0.5, 0.5, 1), COLOR(1, 1, 1, 1), a->percent);
				})
			);
			Destroy();
		}
	}
}

void Missile::Render()
{
	RENDER->Render(image, InGrid(pos), V2(0.5, 0.5), COLOR(0, 0, 1, 1), 0.1, 0, 60, false);
	RENDER->Render(image, InGrid(pos), V2(0.5, 0.5), COLOR(1, 1, 1, 1), 0.08, 0, 60, false);
}
