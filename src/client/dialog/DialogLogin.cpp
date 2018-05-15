// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QMetaType>
#include <QPushButton>
#include <QSettings>
#include <QStringList>
#include <QVBoxLayout>

#include <client/dialog/DialogLogin.h>
#include <client/dialog/DialogSignUp.h>
#include <client/thread/ThreadLogin.h>
#include <client/values.h>
#include <core/utils.h>

DialogLogin::DialogLogin(UserManager &userManager, QWidget *parent) :
    userManager(userManager),
    QDialog(parent) {

    cbboxUsername = new QComboBox(this);
    editPassword = new QLineEdit(this);

    labelMessage = new QLabel(this);

    btnSignUp = new QPushButton(this);
    btns = new QDialogButtonBox(this);

    setUI();
    setConnection();
    loadSetting();
}

DialogLogin::~DialogLogin() {
    saveSetting();
}

void DialogLogin::loadSetting() {
    QSettings setting;
    setting.beginGroup("LoginDialog");

    auto list = setting.value("Username").toStringList();
    auto current = setting.value("Current").toString();
    int pos = -1;
    for (int i = 0; i < list.size(); ++i) {
        cbboxUsername->addItem(list[i]);
        if (list[i] == current)
            pos = i;
    }
    if (pos != -1) {
        cbboxUsername->setCurrentIndex(pos);
        editPassword->setFocus();
    }

    setting.endGroup();
}

void DialogLogin::saveSetting() {
    if (userManager.getToken()) {
        QSettings setting;
        setting.beginGroup("LoginDialog");

        QStringList list;
        auto current = cbboxUsername->currentText();
        bool flag = 1;
        for (int i = 0; i < cbboxUsername->count(); ++i) {
            auto name = cbboxUsername->itemText(i);
            list.append(name);
            if (name == current)
                flag = 0;
        }
        if (flag)
            list.append(current);
        list.sort();

        setting.setValue("Username", list);
        setting.setValue("Current", current);

        setting.endGroup();
    }
}

void DialogLogin::slotLoginBegin() {
    labelMessage->show();
    labelMessage->setText(tr("Connecting..."));
    QString username = cbboxUsername->currentText();
    QString password = QCryptographicHash::hash(QString::fromStdString(X::saltBegin + editPassword->text().toStdString() + X::saltEnd).toLocal8Bit(), QCryptographicHash::Sha1).toHex();

    auto thread = new ThreadLogin(std::move(username), std::move(password), this);
    connect(thread, &ThreadLogin::done, this, &DialogLogin::slotLoginEnd);
    connect(thread, &ThreadLogin::finished, thread, &QObject::deleteLater);
    thread->start();
}

void DialogLogin::slotLoginEnd(const X::ErrorCode &ec, const X::xll &token, const X::ptree &pt) {
    if (ec == X::NoError) {
        userManager.setToken(token);
        userManager.setUser(pt);
        accept();
    } else {
        QString s;
        if (ec == X::NoSuchUser)
            s = tr("Wrong username or password");
        else if (ec == X::LoginFailed)
            s = tr("Login failed, check network");
        else
            s = QString::fromStdString(X::what(ec));
        labelMessage->setText(s);
    }
}

void DialogLogin::slotRegister() {
    DialogSignUp dialog(this);
    dialog.exec();
}

void DialogLogin::setUI() {
    setWindowTitle(QString::fromStdString(X::APP_NAME));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    labelMessage->hide();

    cbboxUsername->setEditable(true);
    cbboxUsername->lineEdit()->setPlaceholderText(tr("Username"));

    editPassword->setEchoMode(QLineEdit::Password);
    editPassword->setPlaceholderText(tr("Password"));

    btnSignUp->setText(tr("&Sign up"));
    btns->addButton(QDialogButtonBox::Ok)->setText(tr("&Log in"));
    btns->addButton(btnSignUp, QDialogButtonBox::ActionRole);
    btns->setCenterButtons(true);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(cbboxUsername);
    layout->addWidget(editPassword);
    layout->addWidget(labelMessage);
    layout->addWidget(btns);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(layout);
    adjustSize();
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

void DialogLogin::setConnection() {
    connect(
        btnSignUp,
        &QPushButton::clicked,
        this,
        &DialogLogin::slotRegister
    );

    connect(
        btns->button(QDialogButtonBox::Ok),
        &QPushButton::clicked,
        this,
        &DialogLogin::slotLoginBegin
    );
}
