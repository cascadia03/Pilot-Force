#pragma once

class ScoreScene : public Scene
{
public:
	ScoreScene() {}
	virtual ~ScoreScene() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	wstring initial = L"\0";
};

