#pragma once

class IntroScene : public Scene
{
public:
	IntroScene() {}
	virtual ~IntroScene() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	COLOR blurColor = COLOR(1, 1, 1, 1);

	wstring text = L"\0";
	COLOR textColor = COLOR(1, 1, 1, 1);
	Instant* ShowText(wstring pText);
	Instant* CreatePilot();
};
