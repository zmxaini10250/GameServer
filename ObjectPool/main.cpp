#include <iostream>
#include "ObjectPool.hpp"

int main()
{
    CObjectPool<int,50> pool;
    auto p = pool.GetObject(4);
    std::cout<<*p<<std::endl;

    return 0;
}
