// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/administer.h>

Administer::uint Administer::getAuthority() const {
    return authority;
}

void Administer::setAuthority(const uint &value) {
    authority = value;
}
