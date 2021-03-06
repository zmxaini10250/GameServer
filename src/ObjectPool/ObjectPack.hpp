#ifndef _OBJECTPOOL_HPP_
#define _OBJECTPOOL_HPP_

#include <type_traits>
#include <memory>
#include <stack>
#include <unordered_map>

template<class T, int PackLength = 1024>
class CObjectPack
{
    protected:
        typedef typename std::aligned_storage<sizeof(T), alignof(T)>::type ObjectBlock;
        ObjectBlock PackBlock[PackLength];
        std::stack<ObjectBlock *> FreeSpaceStack;
        std::unordered_map<int, std::shared_ptr<T>> ObjectMap;
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
        template<class... Args>
            int CreateObjectIndex(Args&&... args)
            {
                if (FreeSpaceStack.empty())
                {
                    return -1;
                }
                ObjectBlock *space = FreeSpaceStack.top();
                FreeSpaceStack.pop();
                ObjectMap.insert(std::make_pair(space - PackBlock, std::shared_ptr<T>(new (space)T(args...), CFreeClass(*this))));
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
				std::shared_ptr<T> p(new (space)T(args...), CFreeClass(*this));
                std::weak_ptr<T> wp(p);
                ObjectMap.insert(std::make_pair(space - PackBlock, std::move(p)));
                return wp;
            }

        int RemoveObject(int index)
        {
            typename std::unordered_map<int, std::shared_ptr<T>>::const_iterator it = ObjectMap.find(index);
            if (it != ObjectMap.cend())
            {
                ObjectMap.erase(it);
                return  0;
            }
            return -1;
        }

        std::weak_ptr<T> GetObject(int index)
        {
            typename std::unordered_map<int, std::shared_ptr<T>>::const_iterator it = ObjectMap.find(index);
            if (it != ObjectMap.cend())
            {
                return std::weak_ptr<T>(it->second);
            }
            return std::weak_ptr<T>();
        }
    public:
        const std::unordered_map<int, std::shared_ptr<T>>& GetObjectMap()const
        {
            return ObjectMap;
        }

        bool isFull()
        {
            return FreeSpaceStack.empty();
        }
        
        int GetFreeSpace()
        {
            return FreeSpaceStack.size();
        }
        
        void Clear()
        {
            ObjectMap.clear();
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
