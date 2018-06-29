// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QApplication>
#include <QDesktopWidget>
#include <QMenuBar>
#include <QSize>

#include <client/dialog/DialogLogin.h>
#include <client/mainwindow/MainWindow.h>
#include <client/thread/ThreadLogout.h>

MainWindow::MainWindow(QWidget *parent) : 
    QMainWindow(parent) {
    
    widgetMain = nullptr;
    userManager = nullptr;
    bookManager = nullptr;
    go();
}

MainWindow::~MainWindow() {
    saveSetting();
}

void MainWindow::go() {
    if (widgetMain)
        delete widgetMain;
    if (userManager) {
        userManager->refresh();
        delete userManager;
    }
    if (bookManager) {
        bookManager->refresh();
        delete bookManager;
    }

    userManager = new UserManager;
    bookManager = new BookManager(*userManager);

    login();

    widgetMain = new WidgetMain(*userManager, *bookManager, this);

    setUI();
    setConnection();
    loadSetting();
    show();
}

void MainWindow::loadSetting() {
    QSettings setting;
    setting.beginGroup("MainWindow");

    restoreState(setting.value("State").toByteArray());

    setting.endGroup();
}

void MainWindow::saveSetting() {
    QSettings setting;
    setting.beginGroup("MainWindow");

    setting.setValue("State", saveState());

    setting.endGroup();
    widgetMain->saveSetting();
}

void MainWindow::logout() {
    auto thread = new ThreadLogout(userManager->getToken(), this);
    connect(thread, &ThreadLogout::done, this, [this] {
        hide();
        go();
    });
    connect(thread, &ThreadLogout::finished, thread, &QObject::deleteLater);
    thread->start();
}

void MainWindow::setUI() {
    setCentralWidget(widgetMain);
    setMinimumSize(QSize(1280, 760));
    adjustSize();
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    menuBar()->setVisible(false);
}

void MainWindow::setConnection() {
    connect(widgetMain, &WidgetMain::signalLogout, this, &MainWindow::logout);
}

void MainWindow::login() {
    DialogLogin dialog(*userManager);
    if (dialog.exec() != QDialog::Accepted)
        exit(0);
    if (userManager->getToken() == 0)
        exit(0);
}

