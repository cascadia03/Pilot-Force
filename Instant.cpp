#include "DXUT.h"
#include "Instant.h"

void Instant::Init()
{
	init(this);
}

void Instant::Release()
{
	if (next)
	{
		OBJECT->Add(next);
		next = NULL;
	}
}

void Instant::Update()
{
	update(this);

	time -= DT;

	if (time <= 0)
	{
		if (isLoop)
		{
			time = duration;
			init(this);
		}
		else
		{
			Destroy();
		}
	}

	percent = lerp(0.f, 1.f, 1 - (time / duration));
}

void Instant::Render()
{
	if (image)
	{
		RENDER->Render(image, pos, pivot, color, scale, rotate, depth, isView);
	}

	if (text != L"\0")
	{
		RENDER->Write(text, pos, pivot, color, scale, rotate, depth, isView);
	}
}

Instant* Instant::Add(Instant* pInstant)
{
	return next = pInstant;
}
