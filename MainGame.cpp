#include "DXUT.h"
#include "MainGame.h"

MainGame::MainGame()
{
	ImageManager::InitInstance();
	CameraManager::InitInstance();
	RenderManager::InitInstance();
	SoundManager::InitInstance();
	InputManager::InitInstance();
	SceneManager::InitInstance();
	ObjectManager::InitInstance();
	GameManager::InitInstance();
}

MainGame::~MainGame()
{
	GameManager::ReleaseInstance();
	SceneManager::ReleaseInstance();
	ObjectManager::ReleaseInstance();
	ImageManager::ReleaseInstance();
	RenderManager::ReleaseInstance();
	CameraManager::ReleaseInstance();
	SoundManager::ReleaseInstance();
	InputManager::ReleaseInstance();
}

void MainGame::Update()
{
	CAMERA->Update();
	INPUT->Update();
	OBJECT->Update();
	SCENE->Update();
	SOUND->Update();
	GAME->Update();
}

void MainGame::Render()
{
	RENDER->Begin();

	CAMERA->SetTransform();
	SCENE->Render();
	OBJECT->Render();
	GAME->Render();

	RENDER->End();
}

void MainGame::OnLostDevice()
{
	RENDER->OnLostDevice();
}

void MainGame::OnResetDevice()
{
	RENDER->OnResetDevice();
}
