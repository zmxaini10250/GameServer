#include "Process.h"
#include "Data.h"

void CProcess::ProcessData(const Data& data)
{
    printf("%s\n", data.buffer);
}
