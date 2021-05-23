#include "DXUT.h"
#include "TtileScene.h"

void TitleScene::Init()
{
	CAMERA->pos = V2(960, 540);
	CAMERA->scale = 1.f;

	SOUND->Play(L"BackgroundMusic", 1000, true);
}

void TitleScene::Release()
{
	OBJECT->Clear();
	SOUND->StopAll();

	ShowCursor(true);
}

void TitleScene::Update()
{
	if (INPUT->KeyDown(VK_LEFT))
	{
		SOUND->Play(L"ui_sound_back", 100, false);
		selection = max(1, selection - 1);
	}

	if (INPUT->KeyDown(VK_RIGHT))
	{
		SOUND->Play(L"ui_sound_back", 100, false);
		selection = min(4, selection + 1);
	}

	if (INPUT->KeyDown(VK_SPACE))
	{
		SOUND->Play(L"ui_sound_forward", 100, false);
		showDetail = !showDetail;
	}

	V2 toward = V2(0, 0);

	if (!showDetail)
	{
		titleColor = lerp(titleColor, COLOR(1, 1, 1, 1), 0.1);
		infoColor = lerp(infoColor, COLOR(1, 1, 1, 0), 0.1);
		rankColor = lerp(rankColor, COLOR(1, 1, 1, 0), 0.1);
	}
	else
	{
		titleColor = lerp(titleColor, COLOR(1, 1, 1, 0), 0.1);

		switch (selection)
		{
		case 1:
			SCENE->Load<IngameScene1>({ 320, 320 }, 1);
			break;
		case 2:
			infoColor = lerp(infoColor, COLOR(1, 1, 1, 1), 0.1);
			break;
		case 3:
			rankColor = lerp(rankColor, COLOR(1, 1, 1, 1), 0.1);
			break;
		case 4:
			PostQuitMessage(NULL);
			break;
		}
	}

	switch (selection)
	{
	case 1:
		toward = V2(960, 540);
		break;
	case 2:
		toward = V2(1260, 540 + (showDetail ? 1080 : 0));
		break;
	case 3:
		toward = V2(1560, 540 + (showDetail ? 1080 : 0));
		break;
	case 4:
		toward = V2(1860, 540);
		break;
	}

	CAMERA->pos = lerp(CAMERA->pos, toward, 0.1);
}

void TitleScene::Render()
{
	RENDER->Render(IMAGE->Load(L"blur"), V2(0, 0), V2(0, 0), COLOR(1, 1, 1, 1), 1, 0, 1, true);

	RENDER->Write(L"Pilot Force", V2(960, 540), V2(0.5, 0.5), titleColor, 2, 0, 100, true);

	RENDER->Write(L"Start", V2(960, 820), V2(0.5, 0.5), COLOR(1, 1, 1, 1), 1, 0, 100, false);
	RENDER->Write(L"Info", V2(1260, 820), V2(0.5, 0.5), COLOR(1, 1, 1, 1), 1, 0, 100, false);
	RENDER->Write(L"Rank", V2(1560, 820), V2(0.5, 0.5), COLOR(1, 1, 1, 1), 1, 0, 100, false);
	RENDER->Write(L"Exit", V2(1860, 820), V2(0.5, 0.5), COLOR(1, 1, 1, 1), 1, 0, 100, false);

	RENDER->Write(L"Cure the area please, mr. pilot", V2(960, 270), V2(0.5, 0.5), infoColor, 0.5, 0, 100, true);
	RENDER->Write(L"Super virus can infects the area, too", V2(960, 405), V2(0.5, 0.5), infoColor, 0.5, 0, 100, true);
	RENDER->Write(L"Other viruses are attack you, too", V2(960, 540), V2(0.5, 0.5), infoColor, 0.5, 0, 100, true);
	RENDER->Write(L"But, you can solve this problem by many items", V2(960, 675), V2(0.5, 0.5), infoColor, 0.5, 0, 100, true);
	RENDER->Write(L"Remember, you should cure the area before area is infected 60% by super virus", V2(960, 805), V2(0.5, 0.5), infoColor, 0.5, 0, 100, true);

	for (int i = 0; i < 5; i++)
	{
		WCHAR scoreStr[256];
		swprintf(scoreStr, L"%08d", GAME->ranks[i].first);
		RENDER->Write(scoreStr, V2(960, 270 + 135 * i), V2(1, 0.5), rankColor, 1, 0, 100, true);
		RENDER->Write(GAME->ranks[i].second, V2(1080, 270 + 135 * i), V2(0, 0.5), rankColor, 1, 0, 100, true);
	}
}
