#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <random>
#include <cmath>
#include <string>
#include <SDKwavefile.h>
#include <SDKsound.h>

#pragma comment(lib, "dsound.lib")

using namespace std;

using V2 = D3DXVECTOR2;
using V3 = D3DXVECTOR3;
using MAT = D3DXMATRIX;
using COLOR = D3DXCOLOR;

#define g_device DXUTGetD3D9Device()
#define DT DXUTGetElapsedTime()

#include "math.h"

#include "Singleton.h"
#include "CameraManager.h"
#include "ImageManager.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "GameManager.h"
