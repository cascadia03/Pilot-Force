#pragma once

class TitleScene : public Scene
{
public:
	TitleScene() {}
	virtual ~TitleScene() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	int selection = 1;
	bool showDetail = false;
	
	COLOR titleColor = COLOR(1, 1, 1, 1);
	COLOR infoColor = COLOR(1, 1, 1, 0);
	COLOR rankColor = COLOR(1, 1, 1, 0);

};
