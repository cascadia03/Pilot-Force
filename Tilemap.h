#pragma once

#define InGrid(pos) (pos * 2 + V2(1, 1))
#define GRIDV2(x, y) V2(x * 2 + 1, y * 2 + 1)

enum Cell { EmptyCell = 1, DrawedCell = 2, LinedCell = 3, FilledCell = 4, ToxicedCell = 5, ToxicLinedCell = 6, OutOfCell = 7 };

class Tilemap : public Object
{
public:
	Tilemap(UINT pStage)
	{
		switch (pStage)
		{
		case 1:
			cured = IMAGE->Load(L"tilemap/cured (1)");
			break;
		case 2:
			cured = IMAGE->Load(L"tilemap/cured (2)");
			break;
		default:
			cured = NULL;
			break;
		}
		masked = IMAGE->Load(L"tilemap/masked");
	}
	virtual ~Tilemap() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	Image* cured = NULL;
	Image* masked = NULL;
	int depth = 1;

	Cell grid[320][320];

	void SetCell(int pX, int pY, Cell pCell);
	void SetCell(V2 pPos, Cell pCell);
	Cell GetCell(int pX, int pY);
	Cell GetCell(V2 pPos);

	int FloodFill(int pX, int pY);
	int FloodFill(V2 pPos);

};
