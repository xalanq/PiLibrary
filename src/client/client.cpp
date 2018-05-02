// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/client.h>
#include <client/dialoglogin.h>

Client::Client(QWidget *parent) : 
    QMainWindow(parent) {
    DialogLogin dialog(io_service);
    dialog.exec();
}
