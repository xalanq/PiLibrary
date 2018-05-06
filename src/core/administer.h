// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/abstractuser.h>
#include <core/xcore.h>

class Administer : public AbstractUser
{
public:
    enum {
        ADD_BOOK = 1,
        REMOVE_BOOK = 2,
        EDIT_BOOK = 4,
        ADD_ADMINISTER = 8,
        REMOVE_ADMINISTER = 16,
        EDIT_ADMINISTER = 32
    };

    xint getAuthority() const;
    void setAuthority(const xint &value);

private:
    xint authority;
};
