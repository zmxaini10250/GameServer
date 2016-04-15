#include <iostream>
#include "ObjectPool.hpp"

int main()
{
    auto p = CObjectPool<int>::GetInstance().GetObject(4);
    std::cout<<*p<<std::endl;

    return 0;
}
