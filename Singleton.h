#pragma once

template<typename Type>
class Singleton
{
public:
	Singleton() {}
	virtual ~Singleton() {}

private:
	static Type* instance;

public:
	static void InitInstance() { instance = new Type(); }
	static void ReleaseInstance() { SAFE_DELETE(instance); }
	static Type* GetInstance() { return instance; }

};

template<typename Type>
Type* Singleton<Type>::instance = nullptr;
