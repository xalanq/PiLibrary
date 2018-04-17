#include "administer.h"

X::uint Administer::getAuthority() const {
    return authority;
}

void Administer::setAuthority(const X::uint &value) {
    authority = value;
}
