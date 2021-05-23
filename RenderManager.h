#pragma once

class RenderManager : public Singleton<RenderManager>
{
public:
	RenderManager();
	virtual ~RenderManager();

	void Begin();
	void End();
	void OnLostDevice();
	void OnResetDevice();

private:
	LPD3DXSPRITE sprite = NULL;
	LPD3DXFONT font = NULL;

public:
	void Render(Image* pImage, V2 pPos, V2 pPivot, COLOR pColor, float pScale, float pRotate, UINT pDepth, bool pIsView);
	void Write(wstring pText, V2 pPos, V2 pPivot, COLOR pColor, float pScale, float pRotate, UINT pDepth, bool pIsView);

	void RenderRect(Image* pImage, V2 pPos, V2 pPivot, COLOR pColor, float pScale, float pRotate, RECT pRect, UINT pDepth, bool pIsView);

};

#define RENDER RenderManager::GetInstance()
