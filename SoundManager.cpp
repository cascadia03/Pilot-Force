#include "DXUT.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	csmanager.Initialize(DXUTGetHWND(), DSSCL_PRIORITY);
}

SoundManager::~SoundManager()
{
	for (auto i : sounds)
	{
		SAFE_DELETE(i.second);
	}
	sounds.clear();

	for (auto i : buffers)
	{
		SAFE_RELEASE(i.second);
	}
	buffers.clear();
}

void SoundManager::Update()
{
	DWORD status;

	for (auto i = buffers.begin(); i != buffers.end();)
	{
		(*i).second->GetStatus(&status);

		if (!status & !DSBSTATUS_PLAYING)
		{
			SAFE_RELEASE((*i).second);
			i = buffers.erase(i);
		}
		else i++;
	}
}

LPDIRECTSOUNDBUFFER SoundManager::Play(wstring pPath, UINT pVolume, bool pIsLoop)
{
	auto& find = sounds[pPath];

	if (!find)
	{
		CSound** sound = new CSound*();

		WCHAR path[256];
		swprintf(path, L"sounds/%s.wav", pPath.c_str());

		if (csmanager.Create(sound, path, DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN) == S_OK)
		{
			find = *sound;
		}

		SAFE_DELETE(sound);
	}

	LPDIRECTSOUNDBUFFER buffer = NULL;

	if (csmanager.GetDirectSound()->DuplicateSoundBuffer(find->GetBuffer(NULL), &buffer) == S_OK)
	{
		buffer->Play(NULL, NULL, pIsLoop ? DSBPLAY_LOOPING : NULL);
		buffer->SetVolume(-2000 + pVolume);
		buffers.insert({ pPath, buffer });
	}
	else SAFE_DELETE(buffer);

	return buffer;
}

void SoundManager::Stop(wstring pPath)
{
	for (auto i = buffers.begin(); i != buffers.end();)
	{
		if ((*i).first == pPath)
		{
			SAFE_RELEASE((*i).second);
			i = buffers.erase(i);
		}
		else i++;
	}
}

void SoundManager::StopAll()
{
	for (auto i : buffers)
	{
		i.second->Stop();
	}
	buffers.clear();
}
