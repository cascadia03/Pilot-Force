#include "DXUT.h"
#include "Tilemap.h"

void Tilemap::Init()
{
	for (int y = 0; y < 320; y++)
	for (int x = 0; x < 320; x++)
	{
		if (x == 0 || x == 319 || y == 0 || y == 319)
			SetCell(x, y, LinedCell);
		else
			SetCell(x, y, EmptyCell);
	}
}

void Tilemap::Release()
{
}

void Tilemap::Update()
{
	COLOR top[640];

	D3DLOCKED_RECT m_rt;

	cured->texture->LockRect(NULL, &m_rt, NULL, D3DLOCK_DISCARD);

	DWORD* m_bits = (DWORD*)m_rt.pBits;

	for (int y = 0; y < 640; y++)
	for (int x = 0; x < 640; x++)
	{
		int idx = x + y * 640;

		if (y == 0)
			top[x] = m_bits[idx];

		if (y != 639)
			m_bits[idx] = m_bits[idx + 640];
		else
			m_bits[idx] = top[x];
	}

	cured->texture->UnlockRect(NULL);
}

void Tilemap::Render()
{
	if (cured)
	{
		RENDER->Render(cured, V2(0, 0), V2(0, 0), COLOR(1, 1, 1, 1), 1, 0, 0, false);
		RENDER->Render(masked, V2(0, 0), V2(0, 0), COLOR(1, 1, 1, 1), 1, 0, depth, false);
	}
}

void Tilemap::SetCell(int pX, int pY, Cell pCell)
{
	if (pX >= 0 && pX < 320 && pY >= 0 && pY < 320)
	{
		D3DLOCKED_RECT m_rt;

		grid[pY][pX] = pCell;

		if (!cured) return;

		masked->texture->LockRect(NULL, &m_rt, NULL, D3DLOCK_DISCARD);
		
		DWORD* m_bits = (DWORD*)m_rt.pBits;

		for (int y = 0; y < 2; y++)
		for (int x = 0; x < 2; x++)
		{
			int idx = (pX * 2 + x) + (pY * 2 + y) * 640;

			COLOR m_pixel = m_bits[idx];

			switch (pCell)
			{
			case EmptyCell:
				m_pixel = COLOR(0, 0, 0, 0.75);
				break;
			case DrawedCell:
				m_pixel = COLOR(0, 1, 0, 1);
				break;
			case LinedCell:
				m_pixel = COLOR(1, 1, 1, 1);
				break;
			case FilledCell:
				m_pixel = COLOR(0, 0, 0, 0);
				break;
			case ToxicedCell:
				m_pixel = COLOR(1, 0, 0, 0.75);
			case ToxicLinedCell:
				m_pixel = COLOR(1, 0, 0, 1);
				break;
			}

			m_bits[idx] = m_pixel;
		}

		masked->texture->UnlockRect(NULL);
	}
}

void Tilemap::SetCell(V2 pPos, Cell pCell)
{
	SetCell(pPos.x, pPos.y, pCell);
}

Cell Tilemap::GetCell(int pX, int pY)
{
	if (pX >= 0 && pX < 320 && pY >= 0 && pY < 320)
		return grid[pY][pX];
	else
		return OutOfCell;
}

Cell Tilemap::GetCell(V2 pPos)
{
	Cell a = GetCell(pPos.x, pPos.y);
	return a;
}

int Tilemap::FloodFill(int pX, int pY)
{
	int result = 0;

	if (GetCell(pX, pY) == EmptyCell)
	{
		SetCell(pX, pY, FilledCell);
		result++;
		result += FloodFill(pX - 1, pY);
		result += FloodFill(pX + 1, pY);
		result += FloodFill(pX, pY - 1);
		result += FloodFill(pX, pY + 1);
	}

	if (GetCell(pX, pY) == DrawedCell)
	{
		SetCell(pX, pY, LinedCell);
		result++;
	}

	return result;
}

int Tilemap::FloodFill(V2 pPos)
{
	return FloodFill(pPos.x, pPos.y);
}
