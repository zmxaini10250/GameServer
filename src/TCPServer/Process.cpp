#include <iostream>
#include "Process.h"
#include "Data.h"

void CProcess::ProcessData(const Data& data)
{
    std::cout<<data.buffer<<std::endl;
}
