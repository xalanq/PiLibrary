// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QApplication>

#include <client/mainwindow.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
