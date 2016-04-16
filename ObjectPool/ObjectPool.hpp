#ifndef _OBJECTPOOL_HPP_
#define _OBJECTPOOL_HPP_

#include <type_traits>
#include <memory>
#include <stack>

#include "Singleton.hpp"

template<class T, int>
class CObjectPoolDeleter;
template<class T, int>
class CObjectPool;

template<class T, int PoolLength = 1024>
class CObjectPoolDeleter
{
    public:
        CObjectPoolDeleter() {};
        virtual ~CObjectPoolDeleter() {};
        virtual void operator() (CObjectPool<T, PoolLength> *t)const
        {
            if (t != nullptr)
            {
                delete t;
            }
        }
};

template<class T, int PoolLength = 1024>
class CObjectPool :public CSingleton<CObjectPool<T, PoolLength>, CObjectPoolDeleter<T, PoolLength> >
{
    protected:
        typedef typename std::aligned_storage<sizeof(T), alignof(T)>::type ObjectBlock;
        ObjectBlock PoolBlock[PoolLength];
        std::stack<ObjectBlock *> FreeSpaceStack;

        class CFreeClass
        {
            public:
                CFreeClass(CObjectPool &pool) :pool(pool) {}
                void operator() (T *t) 
                { 
                    if (t == nullptr)
                        return;
                    t->~T(); 
                    pool.FreeSpaceStack.push((ObjectBlock *)t);
                }
            private:
                CObjectPool &pool;
        };

        friend class CObjectPoolDeleter<T, PoolLength>;
        friend class CSingleton<CObjectPool<T, PoolLength>, CObjectPoolDeleter<T, PoolLength> >;
    public:
        typedef std::unique_ptr<T, CFreeClass> Ptr;

        template<class... Args>
            Ptr GetObject(Args&&... args)
            {
                if (FreeSpaceStack.empty())
                    return Ptr(nullptr, *this);
                ObjectBlock *space = FreeSpaceStack.top();
                FreeSpaceStack.pop();
                return Ptr(new (space)T(args...), CFreeClass(*this));
            }

    protected:
        CObjectPool()
        {
            for (int i = 0; i < PoolLength; ++i)
            {
                FreeSpaceStack.push(&PoolBlock[i]);
            }
        }
        virtual ~CObjectPool() {}
};

#endif
