#include "administer.h"

Administer::uint Administer::getAuthority() const {
    return authority;
}

void Administer::setAuthority(const uint &value) {
    authority = value;
}
