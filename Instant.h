#pragma once

class Instant : public Object
{
public:
	Instant(float pTime)
	{
		duration = time = pTime;
		isLoop = false;
		init = [=](Instant* a) {};
		update = [=](Instant* a) {};
	}
	Instant(function<void(Instant*)> pInit)
	{
		duration = time = 0;
		isLoop = false;
		init = pInit;
		update = [=](Instant* a) {};
	}
	Instant(float pTime,
		function<void(Instant*)> pUpdate)
	{
		duration = time = pTime;
		isLoop = false;
		init = [=](Instant* a) {};
		update = pUpdate;
	}
	Instant(float pTime,
		function<void(Instant*)> pInit,
		function<void(Instant*)> pUpdate)
	{
		duration = time = pTime;
		isLoop = false;
		init = pInit;
		update = pUpdate;
	}
	Instant(float pTime, bool pIsLoop,
		function<void(Instant*)> pInit,
		function<void(Instant*)> pUpdate)
	{
		duration = time = pTime;
		isLoop = pIsLoop;
		init = pInit;
		update = pUpdate;
	}
	virtual ~Instant()
	{
		SAFE_DELETE(next);
	}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	Image* image = NULL;
	wstring text = L"\0";

	V2 pos = V2(0, 0);
	V2 pivot = V2(0.5, 0.5);
	COLOR color = COLOR(1, 1, 1, 1);
	float rotate = 0;
	float scale = 1;
	UINT depth = 0;
	bool isView = false;

	float time = 0;
	float duration = 0;
	float percent = 0;
	bool isLoop = false;

	function<void(Instant*)> init;
	function<void(Instant*)> update;

	Instant* next = NULL;

	Instant* Add(Instant* pInstant);
};

typedef Instant* LPINST;
