#include <iostream>
#include <vector>
#include "ObjectPool/ObjectPool.hpp"
#include "ObjectPool/SingletonObject.hpp" 
class MyClassA
{
public:
	int GetInt() { return i; }
protected:
	MyClassA() {}
	virtual ~MyClassA() 
	{
		std::cout << "~MyClassA()" << std::endl;
	}
	int i = 0;
};

class MyClassB
{
public:
	int GetInt() { return i; }
	MyClassB() {}
	virtual ~MyClassB()
	{
		std::cout << "~MyClassB()" << std::endl;
	}
protected:
	int i = 0;
};

int main()
{
    auto &object = CSingletonObject<MyClassA>::GetInstance();
    
    const int length = 2;
    const CObjectPool<MyClassB,length> &pp = CObjectPool<MyClassB,length>::GetConstInstance();
    std::vector<CObjectPool<MyClassB,length>::Ptr> p;
    for (int i = 0; i < length; ++i)
    {
        p.push_back(CObjectPool<MyClassB, length>::GetInstance().GetObject());
    }
    CObjectPool<MyClassB,length>::Ptr ap = CObjectPool<MyClassB, length>::GetInstance().GetObject();
    ap.reset(nullptr);
    for (int i = 0; i < length; ++i)
    {
        p.at(i).reset(nullptr);
    }
    CObjectPool<MyClassB,length>::Ptr ap2 = CObjectPool<MyClassB, length>::GetInstance().GetObject();
    ap2.reset(nullptr);
    
	return 0;
}
