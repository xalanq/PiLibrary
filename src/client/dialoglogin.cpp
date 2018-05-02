// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <boost/property_tree/ptree.hpp>
#include <boost/thread.hpp>

#include <QApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QSettings>

#include <client/dialoglogin.h>
#include <core/socketinfo.h>


LoginThread::LoginThread(const QString &username, const QString &password, QObject *parent) :
    QThread(parent),
    io_service(),
    ep(boost::asio::ip::address::from_string(
          QSettings().value("Network/server_url", "127.0.0.1").toString().toStdString()),
       QSettings().value("Network/server_port", 2333).toInt()),
    username(username),
    password(password) {

}

void LoginThread::run() {
    X::ull token;
    boost::property_tree::ptree pt;
    X::ActionCode ac;
    X::ErrorCode ec;

    pt.put("username", username.toStdString());
    pt.put("password", password.toStdString()); 

    try {
        boost::asio::ip::tcp::socket socket(io_service);
        socket.connect(ep);
        X::tcp_sync_write(socket, 0, X::Login, pt);
        pt = boost::property_tree::ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<X::ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &e) {
        token = 0;
        ec = X::LoginFailed;
    }

    if (ac != X::LoginFeedback) {
        token = 0;
        ec = X::LoginFailed;
    }
    emit done(token, int(ec));
}

DialogLogin::DialogLogin(QWidget *parent) :
    QDialog(parent) {

    labelMessage = new QLabel(this);

    cbboxUsername = new QComboBox(this);
    editPassword = new QLineEdit(this);

    btns = new QDialogButtonBox(this);

    setUI();
    setConnection();
    loadSetting();
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

void DialogLogin::slotLoginEnd(const unsigned long long &token, const int &ec) {
    if (ec == int(X::NoError)) {
        emit done(token);
        close();
    } else {
        QString s;
        if (ec == int(X::NoSuchUser))
            s = tr("Wrong username or password");
        else if (ec == int(X::LoginFailed))
            s = tr("Login failed");
        else
            s = QString::fromStdString(X::what(static_cast<X::ActionCode> (ec)));
        labelMessage->setText(s);
    }
}

void DialogLogin::setUI() {
    setWindowTitle(QString::fromStdString(X::APP_NAME));
    /*
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(300, 170);
    setMinimumWidth(300);
    setMaximumWidth(300);
    setMinimumHeight(170);
    setMaximumHeight(170);
    */
    labelMessage->hide();

    cbboxUsername->setEditable(true);
    cbboxUsername->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    cbboxUsername->lineEdit()->setPlaceholderText(tr("Username"));

    editPassword->setEchoMode(QLineEdit::Password);
    editPassword->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    editPassword->setPlaceholderText(tr("Password"));

    btns->addButton(QDialogButtonBox::Ok)->setText(tr("&Login"));
    btns->addButton(QDialogButtonBox::Cancel)->setText(tr("&Quit"));
    btns->setCenterButtons(true);
    btns->button(QDialogButtonBox::Ok)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    btns->button(QDialogButtonBox::Cancel)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(cbboxUsername);
    layout->addWidget(editPassword);
    layout->addWidget(labelMessage);
    layout->addWidget(btns);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(layout);
}

void DialogLogin::setConnection() {
    connect(
        btns->button(QDialogButtonBox::Cancel),
        SIGNAL(clicked()),
        this,
        SLOT(close())
    );

    connect(
        btns->button(QDialogButtonBox::Ok),
        SIGNAL(clicked()),
        this,
        SLOT(slotLoginBegin())
    );
}

void DialogLogin::loadSetting() {

}
