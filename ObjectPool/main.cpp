#include <iostream>
#include <vector>
#include "ObjectPool.hpp"

int main()
{
    const int length = 2;
    const CObjectPool<int,length> &pp = CObjectPool<int,length>::GetConstInstance();
    std::vector<CObjectPool<int,length>::Ptr> p;
    for (int i = 0; i < length; ++i)
    {
        p.push_back(CObjectPool<int, length>::GetInstance().GetObject(i));
    }
    CObjectPool<int,length>::Ptr ap = CObjectPool<int, length>::GetInstance().GetObject(5);
    ap.reset(nullptr);
    for (int i = 0; i < length; ++i)
    {
        p.at(i).reset(nullptr);
    }
    CObjectPool<int,length>::Ptr ap2 = CObjectPool<int, length>::GetInstance().GetObject(5);
    ap2.reset(nullptr);
    return 0;
}
