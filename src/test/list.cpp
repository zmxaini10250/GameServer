#include <list>
#include <memory>
#include <stdio.h>

int main()
{
    std::list<std::unique_ptr<int>> l;
    l.push_back(std::make_unique<int>(6));
    printf("%d\n", *(*l.begin()));
    return 0;
}
