#include "DXUT.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	Clear();
}

void ObjectManager::Update()
{
	for (auto i = objects.begin(); i != objects.end();)
	{
		if ((*i)->isDestroy)
		{
			SAFE_RELEASE(*i);
			SAFE_DELETE(*i);

			i = objects.erase(i);
		}
		else
		{
			(*i)->Update();
			i++;
		}
	}
}

void ObjectManager::Render()
{
	for (auto i : objects)
	{
		i->Render();
	}
}

void ObjectManager::Clear()
{
	for (auto i : objects)
	{
		SAFE_DELETE(i);
	}
	objects.clear();
}
