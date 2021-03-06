// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QApplication>
#include <QCoreApplication>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

#include <client/mainwindow/MainWindow.h>
#include <client/values.h>

int main(int argc, char *argv[]) {
    // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName(QString::fromStdString(X::ORG_NAME));
    QCoreApplication::setApplicationName(QString::fromStdString(X::APP_NAME));

    QApplication a(argc, argv);

    auto language = QSettings().value("Setting/Language", "default").toString();
    if (language == "default")
        language = QLocale::system().name();

    auto *translator = new QTranslator;
    translator->load(":/i18n/" + language);
    QApplication::installTranslator(translator);
    auto *translatorQt = new QTranslator;
    translatorQt->load(":/i18n/qt_" + language);
    QApplication::installTranslator(translatorQt);

    MainWindow w;

    return a.exec();
}
