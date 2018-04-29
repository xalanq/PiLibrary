#ifndef ADMINISTER_H
#define ADMINISTER_H

#include "xcore.h"
#include "abstractuser.h"

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

    uint getAuthority() const;
    void setAuthority(const uint &value);

private:
    uint authority;
};

#endif // ADMINISTER_H
