#pragma once

enum Status { DefStatus, DrawStatus, RetraceStatus, DamagedStatus, NoneStatus };
enum Weapon { NoneWeapon, Weapon1, Weapon2, Weapon3, Weapon4 };

class Player : public Object
{
public:
	Player(V2 pPos) { pos = pPos; }
	virtual ~Player() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	bool isAppeared = false;

	V2 pos = V2(0, 0);
	V2 direction = V2(0, 0);
	COLOR color = COLOR(1, 1, 1, 1);
	float scale = 1;
	float rotate = 0;
	int speed = 1;

	Status status = DefStatus;

	Tilemap* tilemap = NULL;

	vector<pair<V2, float>> pathes;

	bool isInvicible = false;

	Weapon weapon = NoneWeapon;
	float weaponTime = 0;

	bool isLaser = false;

	void SetDirection();
	void DefStatusEvent();
	void DrawStatusEvent();
	void RetraceStatusEvent();
	void DamagedStatusEvent();

	void Weapon1Event();
	void Weapon2Event();
	void Weapon3Event();
	void Weapon4Event();
};
