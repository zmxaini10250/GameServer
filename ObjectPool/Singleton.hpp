#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

#include <memory>

template<class T>
class CSingleton
{
    public:
        static const T& GetConstSingleton()
        {
            return GetSingleton();
        }

        static T& GetSingleton()
        {
            if (SingletonPtr == nullptr)
            {
                SingletonPtr.reset(new T());
            }
            return *SingletonPtr;
        }
    private:
        CSingleton() {}
        ~CSingleton() {};
        static std::unique_ptr<T> SingletonPtr;
};

template<typename T>
std::unique_ptr<T> CSingleton<T>::SingletonPtr;

#endif
