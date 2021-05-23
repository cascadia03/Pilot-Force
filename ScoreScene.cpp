#include "DXUT.h"
#include "ScoreScene.h"

void ScoreScene::Init()
{
	GAME->life = 100;
	GAME->maxLife = 100;
	GAME->move = 100;
	GAME->maxMove = 100;
	GAME->score = 0;
	GAME->timer = 0;
	GAME->cured_percent = 0;

	SOUND->Play(L"BackgroundMusic", 1000, true); 
}

void ScoreScene::Release()
{
	OBJECT->Clear();
	SOUND->StopAll();
}

void ScoreScene::Update()
{
	if (INPUT->GetKey() != 0)
	{
		SOUND->Play(L"ui_sound_back", 100, false);

		initial.push_back(INPUT->GetKey());
	}

	if (initial.size() >= 3)
	{
		GAME->ranks.push_back({GAME->score, initial});

		sort(GAME->ranks.begin(), GAME->ranks.end(),
			[=](pair<int, wstring> a, pair<int, wstring> b) {
				if (a.first > b.first)
					return true;
				else
					return false;
			});
		
		GAME->ranks.pop_back();

		SCENE->Load<TitleScene>({ 960, 540 }, 1);
	}
}

void ScoreScene::Render()
{
	RENDER->Render(IMAGE->Load(L"blur"), V2(0, 0), V2(0, 0), COLOR(1, 1, 1, 1), 1, 0, 1, true);

	RENDER->Write(initial, V2(900, 160), V2(0, 0.5), COLOR(1, 1, 1, 1), 1, 0, 100, true);

	for (int i = 0; i < 5; i++)
	{
		WCHAR scoreStr[256];
		swprintf(scoreStr, L"%08d", GAME->ranks[i].first);
		RENDER->Write(scoreStr, V2(960, 360 + 135 * i), V2(1, 0.5), COLOR(1, 1, 1, 1), 1, 0, 100, true);
		RENDER->Write(GAME->ranks[i].second, V2(1080, 360 + 135 * i), V2(0, 0.5), COLOR(1, 1, 1, 1), 1, 0, 100, true);
	}
}
