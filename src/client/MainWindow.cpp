// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QApplication>
#include <QDesktopWidget>
#include <QMenuBar>
#include <QSize>

#include <client/MainWindow.h>
#include <client/dialog/DialogLogin.h>

MainWindow::MainWindow(QWidget *parent) : 
    QMainWindow(parent),
    userManager(),
    bookManager(userManager) {

    login();

    mainWidget = new MainWidget(userManager, bookManager, this);

    setUI();
    setConnection();
}

void MainWindow::loadSetting() {
    setting.beginGroup("MainWindow");

    restoreState(setting.value("State").toByteArray());

    setting.endGroup();
}

void MainWindow::saveSetting() {
    setting.beginGroup("MainWindow");

    setting.setValue("State", saveState());

    setting.endGroup();
    mainWidget->saveSetting();
}

void MainWindow::setUI() {
    setCentralWidget(mainWidget);
    setMinimumSize(QSize(800, 600));
    adjustSize();
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    menuBar()->setVisible(false);
}

void MainWindow::setConnection() {

}

void MainWindow::login() {
    DialogLogin dialog(userManager, this);
    if (dialog.exec() != QDialog::Accepted)
        exit(0);
    if (userManager.getToken() == 0)
        exit(0);
}

