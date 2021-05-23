#pragma once

class Object
{
public:
	Object() {}
	virtual ~Object() {}

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

private:
	friend class ObjectManager;

	bool isDestroy = false;

public:
	void Destroy() { isDestroy = true; }

};

class ObjectManager : public Singleton<ObjectManager>
{
public:
	ObjectManager();
	virtual ~ObjectManager();

	void Update();
	void Render();

private:
	list<Object*> objects;

public:
	template<typename Type>
	Type Add(Type pObject)
	{
		pObject->Init();
		objects.push_back(pObject);
		return pObject;
	}

	template<typename Type>
	Type* Search()
	{
		for (auto i : objects)
		{
			if (typeid(*i) == typeid(Type))
				return static_cast<Type*>(i);
		}

		Type* cast = NULL;

		for (auto i : objects)
		{
			cast = dynamic_cast<Type*>(i);
			if (cast) return cast;
		}

		return NULL;
	}

	template<typename Type>
	list<Type*> SearchAll()
	{
		list<Type*> result;

		for (auto i : objects)
		{
			if (typeid(*i) == typeid(Type))
				result.push_back(static_cast<Type*>(i));
		}

		if (!result.empty()) return result;

		Type* cast = NULL;

		for (auto i : objects)
		{
			cast = dynamic_cast<Type*>(i);
			if (cast) result.push_back(cast);
		}

		return result;
	}

	void Clear();

};

#define OBJECT ObjectManager::GetInstance()
