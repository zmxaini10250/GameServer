#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

#include <memory>

template<class T, class D = std::default_delete<T>>
class CSingleton
{
    public:
        static const T& GetConstInstance()
        {
            return GetInstance();
        }

        static T& GetInstance()
        {
            if (SingletonPtr == nullptr)
            {
                SingletonPtr.reset(new T());
            }
            return *SingletonPtr;
        }

    protected:
        CSingleton() {}
        virtual ~CSingleton() {};
        static std::unique_ptr<T, D> SingletonPtr;
};

template<class T, class D>
std::unique_ptr<T, D> CSingleton<T, D>::SingletonPtr;


#endif
