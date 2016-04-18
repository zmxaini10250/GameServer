#ifndef _TYPE_DEFINE_H_
#define _TYPE_DEFINE_H_

#include <unistd.h>

typedef char Byte;
typedef ssize_t (*ReadFunction)(int fd, void *buffer, size_t readSize);

#endif
