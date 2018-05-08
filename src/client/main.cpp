// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QApplication>
#include <QCoreApplication>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

#include <client/client.h>
#include <client/xclient.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(QString::fromStdString(X::ORG_NAME));
    QCoreApplication::setApplicationName(QString::fromStdString(X::APP_NAME));

    auto language = QSettings().value("Settings/language", "default").toString();
    if (language == "default")
        language = QLocale::system().name();

    /*
    QTranslator translator;
    translator.load(APP_PATH + 'i18n/' + language)
    QApplication.installTranslator(translator)
    translatorQt = QTranslator()
    translatorQt.load(APP_PATH + 'i18n/qt_' + language)
    QApplication.installTranslator(translatorQt)
    */

    Client w;
    w.show();
    return a.exec();
}
