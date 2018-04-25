#ifndef SOCKETINFO_H
#define SOCKETINFO_H

#include "xserver.h"
#include <ctime>

class SocketInfo {
public:

    const char *getBuffer() const;
    char *getBuffer();

    int getLength() const;
    int getBodyLength() const;
    void setBodyLength(int value);

private:
    enum { HEADER_SIZE = 12, BUFFER_SIZE = 512 };
    char buffer[BUFFER_SIZE];
    int bodyLength;
};

#endif // SOCKETINFO_H
