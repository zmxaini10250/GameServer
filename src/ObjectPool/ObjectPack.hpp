#ifndef _OBJECTPOOL_HPP_
#define _OBJECTPOOL_HPP_

#include <type_traits>
#include <memory>
#include <stack>
#include <unordered_map>

#include "Singleton.hpp"

template<class T, int PackLength = 1024>
class CObjectPack
{
    protected:
        typedef typename std::aligned_storage<sizeof(T), alignof(T)>::type ObjectBlock;
        ObjectBlock PackBlock[PackLength];
        std::stack<ObjectBlock *> FreeSpaceStack;
        std::unordered_map<int, Ptr> ObjectMap;
        class CFreeClass
        {
            public:
                CFreeClass(CObjectPack &pack) :pack(pack) {}
                void operator() (T *t) 
                { 
                    if (t == nullptr)
                        return;
                    t->~T(); 
                    pack.FreeSpaceStack.push((ObjectBlock *)t);
                }
            private:
                CObjectPack &pack;
        };

    public:
        typedef std::shared_ptr<T> Ptr;

        template<class... Args>
            int CreateObjectIndex(Args&&... args)
            {
                if (FreeSpaceStack.empty())
                {
                    return -1;
                }
                ObjectBlock *space = FreeSpaceStack.top();
                FreeSpaceStack.pop();
                ObjectMap.insert(std::make_pair(space - PackBlock, Ptr(new (space)T(args...), CFreeClass(*this))));
                return (space - PackBlock);
            }

        template<class... Args>
            std::weak_ptr<T> CreateObjectPtr(Args&&... args)
            {
                if (FreeSpaceStack.empty())
                {
                    return std::weak_ptr<T>();
                }
                ObjectBlock *space = FreeSpaceStack.top();
                FreeSpaceStack.pop();
                Ptr p(new (space)T(args...), CFreeClass(*this));
                std::weak_ptr<T> wp(p);
                ObjectMap.insert(std::make_pair(space - PackBlock, p.move()));
                return wp;
            }

        template<class... Args>
    public:
        bool isFull()
        {
            return FreeSpaceStack.empty();
        }

        CObjectPack()
        {
            for (int i = 0; i < PackLength; ++i)
            {
                FreeSpaceStack.push(&PackBlock[i]);
            }
        }

        virtual ~CObjectPack() {}
};

#endif
