#include "buffer.h"

#include <memory.h>

Buffer::Buffer() : is_alloc(false), len(0) {
    cap = 1024;
    char buf1[1024] = {0};
    buf = buf1;
}

Buffer::Buffer(int cap) : len(0) {
    if (cap <= 1024) {
        char buf1[1024] = {0};
        buf = buf1; 
        is_alloc = false;
        cap = 1024;
    }else{
        is_alloc = true;
        buf = (char *)malloc(cap);
        this->cap = cap;
    }
}

Buffer::~Buffer(){
    if (is_alloc) {
        free(buf);
    }
}

int Buffer::append(char *data, int len) {
    if (this->len + len > cap) {
        this->buf = (char *)realloc(this->buf,this->len + len);
        if (this->buf == nullptr) {
            return -1;
        }
    } 

    memcpy(this->buf + this->len, data, len);
    this->len += len;

    return 0;
}

char *Buffer::remove(int offset) {
    if (offset > len) {
        return nullptr;
    }

    this->len -= offset;
    return (char *)memmove(this->buf, this->buf+offset, len - offset);
}

