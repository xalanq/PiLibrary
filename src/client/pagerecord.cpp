// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/pagerecord.h>

PageRecord::PageRecord(UserManager &userManager, QWidget *parent) :
    userManager(userManager),
    QWidget(parent) {

}

PageRecord::~PageRecord() {
}
