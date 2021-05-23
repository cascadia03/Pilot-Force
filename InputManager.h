#pragma once

class InputManager : public Singleton<InputManager>
{
public:
	InputManager();
	virtual ~InputManager();

	void Update();

private:
	bool current[256];
	bool prev[256];

public:
	bool KeyPress(DWORD pKey);
	bool KeyUp(DWORD pKey);
	bool KeyDown(DWORD pKey);
	DWORD GetKey();
	
};

#define INPUT InputManager::GetInstance()
