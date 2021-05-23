#include "DXUT.h"
#include "IntroScene.h"

void IntroScene::Init()
{
	GAME->life = 100;
	GAME->maxLife = 100;
	GAME->move = 100;
	GAME->maxMove = 100;
	GAME->score = 0;
	GAME->timer = 0;
	GAME->cured_percent = 0;

	CAMERA->pos = V2(960, 540);
	CAMERA->scale = 1.f;

	OBJECT->Add(ShowText(L"In 2021 years...")
	)->Add(new Instant(0.5)
	)->Add(ShowText(L"Mutant Viruses attack our planet...")
	)->Add(new Instant(0.5)
	)->Add(ShowText(L"Cure to save our planet!")
	)->Add(new Instant(0.5)
	)->Add(ShowText(L"Go on, Pilot!")
	)->Add(new Instant(1,
		[=](LPINST a){
			CAMERA->pos = lerp(CAMERA->pos, V2(960, 480), 0.05);
			CAMERA->scale = lerp(CAMERA->scale, 1.2f, 0.05);
			blurColor.a = lerp(1.f, 0.f, a->percent);
		})
	)->Add(new Instant(5,
		[=](LPINST a) {
			for (int i = 0; i < 128; i++)
				CreatePilot();
		},
		[=](LPINST a) {})
	)->Add(new Instant(3,
		[=](LPINST a) {
			a->image = IMAGE->Load(L"pilot");
			a->pos = V2(960, 1280);
			a->color = COLOR(1, 1, 1, 1);
			a->scale = 4;
			a->rotate = 270;
			a->depth = 40;

			OBJECT->Add(new Instant(3, 
				[=](LPINST b) {
					b->text = L"<-YOU";
					b->color = COLOR(1, 1, 1, 1);
					b->scale = 1;
					b->rotate = 0;
				},
				[=](LPINST b) {
					b->pos.x = a->pos.x + 180.f;
					b->pos.y = lerp(1280.f, -200.f, a->percent);
				})
			);
		},
		[=](LPINST a) {
			a->pos.y = lerp(1280.f, -200.f, a->percent);

			float beginScale = random(0.1f, 0.4f);

			OBJECT->Add(new Instant(0.5,
				[=](LPINST b) {
					b->image = IMAGE->Load(L"effect/circle");
					b->pos = a->pos + V2(random(-4.f, 4.f), random(-4.f, 4.f));
					b->color = COLOR(1, 1, 1, 1);
					b->rotate = 270;
					b->depth = 25;
				},
				[=](LPINST b) {
					b->color.a = lerp(0.5f, 0.f, b->percent);
					b->scale = lerp(beginScale, 0.f, b->percent);
				})
			);
		})
	)->Add(new Instant(1.5)
	)->Add(new Instant(
		[=](LPINST a) {
			SCENE->Load<TitleScene>( { 960, 540 }, 1 );
		})
	);
}

void IntroScene::Release()
{
	OBJECT->Clear();
	SOUND->StopAll();

	ShowCursor(true);
}

void IntroScene::Update()
{
}

void IntroScene::Render()
{
	RENDER->Render(IMAGE->Load(L"title"), V2(0, 0), V2(0, 0), COLOR(1, 1, 1, 1), 1, 0, 0, false);
	RENDER->Render(IMAGE->Load(L"blur"), V2(0, 0), V2(0, 0), blurColor, 1, 0, 1, false);

	RENDER->Write(text, V2(960, 540), V2(0.5, 0.5), textColor, 1, 0, 100, true);
}

Instant* IntroScene::ShowText(wstring pText)
{
	return new Instant(3,
		[=](LPINST a) {
			SOUND->Play(L"ui_sound_forward", 100, false);
			text = pText;
			textColor = COLOR(1, 1, 1, 1);
		},
		[=](LPINST a) {
			textColor.a = lerp(2.f, 0.f, a->percent);
		});
}

Instant* IntroScene::CreatePilot()
{
	float randIdx = random(1.f, 6.f);

	return OBJECT->Add(new Instant(randIdx * 0.5)
	)->Add(new Instant(0.5,
		[=](LPINST a) {
			a->image = IMAGE->Load(L"pilot");
			a->pos = V2(random(240.f, 1680.f), 1280);
			a->color = COLOR(randIdx / 6, randIdx / 6, randIdx / 6, 1);
			a->scale = randIdx;
			a->rotate = 270;
			a->depth = 30;
		},
		[&](LPINST a) {
			a->pos.y -= (2400 + a->percent * 4800) * DT;

			float beginScale = a->scale * 0.1f;

			OBJECT->Add(new Instant(0.5,
				[=](LPINST b) {
					b->image = IMAGE->Load(L"effect/circle");
					b->pos = a->pos;
					b->color = COLOR(1, 1, 1, 1);
					b->rotate = 270;
					b->depth = 25;
				},
				[=](LPINST b) {
					b->color.a = lerp(0.5f, 0.f, b->percent);
					b->scale = lerp(beginScale, 0.f, b->percent);
				})
			);
		})
	)->Add(new Instant(
		[=](LPINST a) {
			CAMERA->Shake(8, 0.1);
		})
	);
}
