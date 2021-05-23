#include "DXUT.h"
#include "ImageManager.h"

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
    for (auto i : images)
    {
        SAFE_RELEASE(i.second->texture);
        SAFE_DELETE(i.second);
    }
    images.clear();
}

Image* ImageManager::Load(wstring pPath)
{
    auto& find = images[pPath];

    if (!find)
    {
        LPDIRECT3DTEXTURE9 texture = NULL;
        D3DXIMAGE_INFO info;

        if (D3DXCreateTextureFromFileEx(g_device, (L"images/" + pPath + L".png").c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, NULL, NULL,
            D3DFMT_A8B8G8R8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, &info, NULL, &texture) == S_OK)
        {
            Image* image = new Image();
            image->texture = texture;
            image->info = info;

            find = image;
        }
    }

    return find;
}
