#pragma once

class SoundManager : public Singleton<SoundManager>
{
public:
	SoundManager();
	virtual ~SoundManager();

	void Update();

private:
	friend class GameManager;

	CSoundManager csmanager;
	map<wstring, CSound*> sounds;
	multimap<wstring, LPDIRECTSOUNDBUFFER> buffers;

public:
	LPDIRECTSOUNDBUFFER Play(wstring pPath, UINT pVolume, bool pIsLoop);
	void Stop(wstring pPath);
	void StopAll();

};

#define SOUND SoundManager::GetInstance()
