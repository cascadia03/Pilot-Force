#pragma once

enum ItemType { SpeedItem, InvicibleItem, LifeItem,
	Shoot1Item, Shoot2Item, Shoot3Item, Shoot4Item };

class Item : public Object
{
public:
	Item() {}
	virtual ~Item() {}
	
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	Image* image = NULL;

	Instant* wave = NULL;

	ItemType type = SpeedItem;

	V2 pos = V2(0, 0);
	float rotate = 0;
	float scale = 0;
	bool isAppeared = false;

	void CreateEffect();
};
