#pragma once

struct Image
{
public:
	LPDIRECT3DTEXTURE9 texture = NULL;
	D3DXIMAGE_INFO info;

};

class ImageManager : public Singleton<ImageManager>
{
public:
	ImageManager();
	virtual ~ImageManager();

private:
	map<wstring, Image*> images;

public:
	Image* Load(wstring pPath);

};

#define IMAGE ImageManager::GetInstance()
