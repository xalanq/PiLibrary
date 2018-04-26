#include "socketinfo.h"

int SocketInfo::getLength() const {
    return BUFFER_SIZE;
}

const char *SocketInfo::getBuffer() const {
    return buffer;
}

char *SocketInfo::getBuffer() {
    return buffer;
}

int SocketInfo::getBodyLength() const {
    return bodyLength;
}

void SocketInfo::setBodyLength(int value) {
    bodyLength = value;
}
