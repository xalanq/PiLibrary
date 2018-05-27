// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QApplication>
#include <QCoreApplication>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

#include <client/MainWindow.h>
#include <client/values.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(QString::fromStdString(X::ORG_NAME));
    QCoreApplication::setApplicationName(QString::fromStdString(X::APP_NAME));

    auto language = QSettings().value("Setting/Language", "default").toString();
    if (language == "default")
        language = QLocale::system().name();

    auto *translator = new QTranslator;
    translator->load(":/Language/qm/" + language);
    QApplication::installTranslator(translator);
    auto *translatorQt = new QTranslator;
    translatorQt->load(":/Language/qm/qt_" + language);
    QApplication::installTranslator(translatorQt);

    MainWindow w;

    return a.exec();
}
