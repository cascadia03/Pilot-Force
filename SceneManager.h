#pragma once

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

};

class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	virtual ~SceneManager();

	void Update();
	void Render();

private:
	Scene* current = NULL;
	Scene* next = NULL;

	V2 nextPos = V2(0, 0);
	float nextScale = 1;

	Image* image = NULL;
	float slide = 0;
	bool phase = false;
public:
	template<typename Type>
	void Load(V2 pPos, float pScale)
	{
		nextPos = pPos;
		nextScale = pScale;

		next = new Type();
	}

};

#define SCENE SceneManager::GetInstance()
