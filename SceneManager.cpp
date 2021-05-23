#include "DXUT.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
	image = IMAGE->Load(L"black");
}

SceneManager::~SceneManager()
{
	SAFE_RELEASE(current);
	SAFE_DELETE(current);

	SAFE_RELEASE(next);
	SAFE_DELETE(next);
}

void SceneManager::Update()
{
	if (next)
	{
		if (!phase)
		{
			slide += DT;

			if (slide >= 1)
			{
				SAFE_RELEASE(current);
				SAFE_DELETE(current);
				current = next;

				CAMERA->pos = nextPos;
				CAMERA->scale = nextScale;

				phase = true;
			}
		}
		if (phase)
		{
			slide -= DT;

			if (slide <= 0)
			{
				next = NULL;
				current->Init();

				phase = false;
			}
		}
	}

	if (current && !next)
	{
		current->Update();
	}
}

void SceneManager::Render()
{
	if (image)
		RENDER->Render(image, V2(0, 0), V2(0, 0), COLOR(1, 1, 1, slide), 1, 0, 500, true);

	if (current)
	{
		current->Render();
	}
}
