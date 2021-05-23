#include "DXUT.h"
#include "RenderManager.h"

RenderManager::RenderManager()
{
	D3DXCreateSprite(g_device, &sprite);
	D3DXCreateFont(g_device, 128, NULL, NULL, D3DVBF_1WEIGHTS, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, L"System", &font);
}

RenderManager::~RenderManager()
{
	SAFE_RELEASE(sprite);
	SAFE_RELEASE(font);
}

void RenderManager::Begin()
{
	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK);
	g_device->SetSamplerState(NULL, D3DSAMP_MIPFILTER, D3DX_FILTER_NONE);
	g_device->SetSamplerState(NULL, D3DSAMP_MINFILTER, D3DX_FILTER_NONE);
	g_device->SetSamplerState(NULL, D3DSAMP_MAGFILTER, D3DX_FILTER_NONE);
}

void RenderManager::End()
{
	sprite->End();
}

void RenderManager::OnLostDevice()
{
	sprite->OnLostDevice();
	font->OnLostDevice();
}

void RenderManager::OnResetDevice()
{
	sprite->OnResetDevice();
	font->OnResetDevice();
}

void RenderManager::Render(Image* pImage, V2 pPos, V2 pPivot, COLOR pColor, float pScale, float pRotate, UINT pDepth, bool pIsView)
{
	pPivot.x *= pImage->info.Width;
	pPivot.y *= pImage->info.Height;

	MAT transMat, posMat, pivotMat, scaleMat, rotateMat;

	D3DXMatrixTranslation(&posMat, pPos.x, pPos.y, 0);
	D3DXMatrixTranslation(&pivotMat, -pPivot.x, -pPivot.y, 0);
	D3DXMatrixScaling(&scaleMat, pScale, pScale, 0);
	D3DXMatrixRotationZ(&rotateMat, D3DXToRadian(pRotate));

	transMat = pivotMat * scaleMat * rotateMat * posMat;
	transMat._43 = 1 - pDepth * 0.001;

	if (pIsView) transMat *= CAMERA->GetTransform();

	sprite->SetTransform(&transMat);
	sprite->Draw(pImage->texture, NULL, NULL, NULL, pColor);

}

void RenderManager::Write(wstring pText, V2 pPos, V2 pPivot, COLOR pColor, float pScale, float pRotate, UINT pDepth, bool pIsView)
{
	RECT rt = RECT{ 0, 0, 0, 0 };

	font->DrawText(sprite, pText.c_str(), pText.size(), &rt, DT_NOCLIP | DT_CALCRECT, pColor);

	pPivot.x *= rt.right;
	pPivot.y *= rt.bottom;

	MAT transMat, posMat, pivotMat, scaleMat, rotateMat;

	D3DXMatrixTranslation(&posMat, pPos.x, pPos.y, 0);
	D3DXMatrixTranslation(&pivotMat, -pPivot.x, -pPivot.y, 0);
	D3DXMatrixScaling(&scaleMat, pScale, pScale, 0);
	D3DXMatrixRotationZ(&rotateMat, D3DXToRadian(pRotate));

	transMat = pivotMat * scaleMat * rotateMat * posMat;
	transMat._43 = 1 - pDepth * 0.001;

	if (pIsView) transMat *= CAMERA->GetTransform();

	sprite->SetTransform(&transMat);
	font->DrawText(sprite, pText.c_str(), pText.size(), NULL, DT_NOCLIP | DT_CENTER | DT_VCENTER, pColor);
}

void RenderManager::RenderRect(Image* pImage, V2 pPos, V2 pPivot, COLOR pColor, float pScale, float pRotate, RECT pRect, UINT pDepth, bool pIsView)
{
	pPivot.x *= pImage->info.Width;
	pPivot.y *= pImage->info.Height;

	MAT transMat, posMat, pivotMat, scaleMat, rotateMat;

	D3DXMatrixTranslation(&posMat, pPos.x, pPos.y, 0);
	D3DXMatrixTranslation(&pivotMat, -pPivot.x, -pPivot.y, 0);
	D3DXMatrixScaling(&scaleMat, pScale, pScale, 0);
	D3DXMatrixRotationZ(&rotateMat, D3DXToRadian(pRotate));

	transMat = pivotMat * scaleMat * rotateMat * posMat;
	transMat._43 = 1 - pDepth * 0.001;

	if (pIsView) transMat *= CAMERA->GetTransform();

	sprite->SetTransform(&transMat);
	sprite->Draw(pImage->texture, &pRect, NULL, NULL, pColor);
}
