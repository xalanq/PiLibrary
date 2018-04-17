QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PiLibrary
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    client/main.cpp \
    client/mainwindow.cpp \
    core/abstractuser.cpp \
    core/loginrecord.cpp \
    core/loginrecordmanager.cpp \
    core/user.cpp \
    core/administer.cpp \
    core/browserecord.cpp \
    core/borrowrecord.cpp \
    core/bookcore.cpp \
    core/book.cpp \
    core/resource.cpp

HEADERS += \
    client/mainwindow.h \
    core/abstractuser.h \
    core/loginrecord.h \
    core/loginrecordmanager.h \
    core/user.h \
    core/administer.h \
    core/browserecord.h \
    core/borrowrecord.h \
    core/bookcore.h \
    core/book.h \
    core/resource.h \
    core/xalanq.h

DISTFILES += \
    .gitignore \
    LICENSE \
    README.md \
    client/.gitignore \
    client/README.md
