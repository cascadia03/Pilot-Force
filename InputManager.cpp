#include "DXUT.h"
#include "InputManager.h"

InputManager::InputManager()
{
    memset(current, false, sizeof(current));
    memset(prev, false, sizeof(prev));
}

InputManager::~InputManager()
{
}

void InputManager::Update()
{
    memcpy(prev, current, sizeof(prev));
    
    for (int i = 0; i < 256; i++)
    {
        current[i] = DXUTIsKeyDown(i);
    }
}

bool InputManager::KeyPress(DWORD pKey)
{
    return current[pKey] || prev[pKey];
}

bool InputManager::KeyUp(DWORD pKey)
{
    return !current[pKey] && prev[pKey];
}

bool InputManager::KeyDown(DWORD pKey)
{
    return current[pKey] && !prev[pKey];
}

DWORD InputManager::GetKey()
{
    for (int i = 0; i < 256; i++)
    {
        if (current[i] && !prev[i])
            return i;
    }

    return 0;
}
