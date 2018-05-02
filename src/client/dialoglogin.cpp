// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <boost/property_tree/ptree.hpp>

#include <QApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QSettings>

#include <client/dialoglogin.h>
#include <core/socketinfo.h>

DialogLogin::DialogLogin(boost::asio::io_service &io_service, QWidget *parent) :
    io_service(io_service),
    QDialog(parent) {

    labelMessage = new QLabel(this);

    cbboxUsername = new QComboBox(this);
    editPassword = new QLineEdit(this);

    btns = new QDialogButtonBox(this);

    setUI();
    setConnection();
    loadSetting();
}

void DialogLogin::slotLogin() {
    labelMessage->setText(tr("Login..."));
    labelMessage->show();

    QString username = cbboxUsername->currentText();
    QString password = QCryptographicHash::hash(QString::fromStdString(X::saltBegin + editPassword->text().toStdString() + X::saltEnd).toLocal8Bit(), QCryptographicHash::Sha1).toHex();

    boost::asio::ip::tcp::endpoint ep(
        boost::asio::ip::address::from_string(
            QSettings().value("Network/server_url", "127.0.0.1").toString().toStdString()),
        QSettings().value("Network/server_port", 2333).toInt());
    boost::asio::ip::tcp::socket socket(io_service);
    boost::property_tree::ptree pt;
    SocketInfo info;

    socket.connect(ep);

    pt.put("username", username.toStdString());
    pt.put("password", password.toStdString());

    auto str = SocketInfo::encodePtree(pt);

    auto size = SocketInfo::HEADER_SIZE + 1 + str.size();

    info.setSize(size);
    info.encode(0, static_cast<X::uint> (str.size()), X::Login, str);

    boost::asio::write(
        socket,
        boost::asio::buffer(info.getBuffer(), size)
    );

    boost::asio::read(
        socket,
        boost::asio::buffer(info.getBuffer(), 1),
        boost::asio::transfer_exactly(1)
    );
    boost::asio::read(
        socket,
        boost::asio::buffer(info.getBuffer(), SocketInfo::HEADER_SIZE),
        boost::asio::transfer_exactly(SocketInfo::HEADER_SIZE)
    );
    auto length = info.decodeHeaderLength();
    auto token = info.decodeHeaderToken();

    info.setSize(length);
    boost::asio::read(
        socket,
        boost::asio::buffer(info.getBuffer(), length),
        boost::asio::transfer_exactly(length)
    );

    pt = boost::property_tree::ptree();
    info.decodeBody(length, pt);
    str = info.encodePtree(pt, true);

    auto ec = static_cast<X::ErrorCode> (pt.get<int>("error_code"));
    if (ec == X::NoError) {
        emit signalLoginToken(token);
        close();
    } {
        QString s;
        if (ec == X::NoSuchUser)
            s = tr("Wrong username or password");
        else if (ec == X::LoginFailed)
            s = tr("Login failed");
        else
            s = QString::fromStdString(X::what(ec));
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
        SLOT(slotLogin())
    );
}

void DialogLogin::loadSetting() {

}
