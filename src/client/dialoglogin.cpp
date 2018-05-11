// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <boost/property_tree/ptree.hpp>

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

#include <client/dialoglogin.h>
#include <client/dialogsignup.h>
#include <core/socketinfo.h>


LoginThread::LoginThread(const QString &username, const QString &password, QObject *parent) :
    QThread(parent),
    io_service(),
    ep(boost::asio::ip::address::from_string(
          QSettings().value("Network/server_url", "127.0.0.1").toString().toStdString()),
       QSettings().value("Network/server_port", 2333).toInt()),
    username(username.toStdString()),
    password(password.toStdString()) {

    qRegisterMetaType<X::xll>("X::xll");
    qRegisterMetaType<ptree>("ptree");
}

void LoginThread::run() {
    X::xll token;
    boost::property_tree::ptree pt;
    X::ActionCode ac = X::NoAction;
    X::ErrorCode ec = X::NoError;

    pt.put("username", username);
    pt.put("password", password); 

    try {
        boost::asio::ip::tcp::socket socket(io_service);
        socket.connect(ep);
        X::tcp_sync_write(socket, 0, X::Login, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<X::ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &) {
        ec = X::LoginFailed;
        token = 0;
        pt = ptree();
    }

    if (ac != X::LoginFeedback) {
        ec = X::LoginFailed;
        token = 0;
        pt = ptree();
    }

    emit done(int(ec), token, pt);
}

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

    LoginThread *thread = new LoginThread(std::move(username), std::move(password), this);
    connect(thread, &LoginThread::done, this, &DialogLogin::slotLoginEnd);
    connect(thread, &LoginThread::finished, thread, &QObject::deleteLater);
    thread->start();
}

void DialogLogin::slotLoginEnd(const int &ec, const X::xll &token, const ptree &pt) {
    if (ec == int(X::NoError)) {
        userManager.setToken(token);
        userManager.setUser(pt);
        accept();
    } else {
        QString s;
        if (ec == int(X::NoSuchUser))
            s = tr("Wrong username or password");
        else if (ec == int(X::LoginFailed))
            s = tr("Login failed, check network");
        else
            s = QString::fromStdString(X::what(static_cast<X::ErrorCode> (ec)));
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
