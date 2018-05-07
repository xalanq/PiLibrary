// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/client.h>
#include <client/dialoglogin.h>

Client::Client(QWidget *parent) : 
    QMainWindow(parent) {

    setUI();
    setConnection();
}

void Client::setUI() {
    DialogLogin dialog(userManager, this);
    dialog.exec();
}

void Client::setConnection() {

}

