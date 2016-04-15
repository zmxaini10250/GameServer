#ifndef _OBJECTPOOL_HPP_
#define _OBJECTPOOL_HPP_

#include <type_traits>
#include <memory>
#include <iostream>
#include <stack>

#include "Singleton.hpp"

template<class T,int PoolLength>
class CObjectPool
{
    private:
        typedef typename std::aligned_storage<sizeof(T), alignof(T)>::type ObjectBlock;
        ObjectBlock PoolBlock[PoolLength];
        std::stack<ObjectBlock *> FreeSpaceStack;

        class CFreeClass
        {
            public:
                CFreeClass(CObjectPool &pool) :pool(pool) {}
                void operator() (T *t) const { t->~T(); pool.FreeSpaceStack.push((ObjectBlock *)t); }
            private:
                CObjectPool &pool;
        };



    public:
        typedef std::unique_ptr<T, CFreeClass> Ptr;

        CObjectPool()
        {
            for (int i = 0; i < PoolLength; ++i)
            {
                FreeSpaceStack.push(&PoolBlock[i]);
            }
        }

        virtual ~CObjectPool() {};

        template<class... Args>
            Ptr GetObject(Args&&... args)
            {
                if (FreeSpaceStack.empty())
                    return Ptr(nullptr, *this);
                ObjectBlock *space = FreeSpaceStack.top();
                FreeSpaceStack.pop();
                return Ptr(new (space)T(args...), CFreeClass(*this));
            }

};

#endif
