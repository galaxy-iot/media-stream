#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>

struct Buffer {
    char *buf;
    int  len;
    int  cap;

    bool is_alloc;

    Buffer();
    Buffer(int cap);
    ~Buffer();

    int append(char *data, int len);
    char *remove(int offset);
};



#endif