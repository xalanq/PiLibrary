// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <regex>

#include <boost/property_tree/ptree.hpp>

#include <QCryptographicHash>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

#include <client/DialogModify.h>

ModifyThread::ModifyThread(const X::xll &token, const QString &nickname, const QString &email, const QString &passwordOld, const QString &passwordNew, QObject *parent) :
    QThread(parent),
    io_service(),
    ep(boost::asio::ip::address::from_string(
          QSettings().value("Network/server_url", "127.0.0.1").toString().toStdString()),
       QSettings().value("Network/server_port", 2333).toInt()),
    token(token),
    nickname(nickname.toStdString()),
    email(email.toStdString()),
    passwordOld(passwordOld.toStdString()),
    passwordNew(passwordNew.toStdString()) {

}

void ModifyThread::run() {
    X::xll token = this->token;
    boost::property_tree::ptree pt;
    X::ActionCode ac = X::NoAction;
    X::ErrorCode ec = X::NoError;

    pt.put("nickname", nickname);
    pt.put("email", email); 
    pt.put("passwordOld", passwordOld);
    if (passwordNew.size() > 0)
        pt.put("passwordNew", passwordNew);

    try {
        boost::asio::ip::tcp::socket socket(io_service);
        socket.connect(ep);
        X::tcp_sync_write(socket, token, X::Modify, pt);
        pt = boost::property_tree::ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<X::ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &) {
        ec = X::ModifyFailed;
    }

    if (ac != X::ModifyFeedback) {
        ec = X::ModifyFailed;
    }

    emit done(int(ec));
}

DialogModify::DialogModify(UserManager &userManager, QWidget *parent) : 
    userManager(userManager),
    QDialog(parent) {

    editNickname = new QLineEdit(this);
    editEmail = new QLineEdit(this);
    editPasswordOld = new QLineEdit(this);
    editPasswordNew = new QLineEdit(this);
    editPasswordNewConfirm = new QLineEdit(this);

    labelMessage = new QLabel(this);

    btns = new QDialogButtonBox(this);

    setUI();
    setConnection();
}

void DialogModify::slotModifyBegin() {
    labelMessage->hide();

    auto &&nickname = editNickname->text();
    if (!X::checkNickname(nickname.toStdString())) {
        labelMessage->show();
        labelMessage->setText(tr("Nickname's length should be in [1, 100]"));
        editNickname->setFocus();
        return;
    }

    auto &&email = editEmail->text();
    if (!X::checkEmail(email.toStdString())) {
        labelMessage->show();
        if (email.size() < 5 || email.size() > 100)
            labelMessage->setText(tr("Email's length should be in [5, 100]"));
        else
            labelMessage->setText(tr("Invalid email"));
        editEmail->setFocus();
        return;
    }

    auto &&passwordOld = editPasswordOld->text();

    auto &&passwordNew = editPasswordNew->text();
    auto &&passwordNewConfirm = editPasswordNewConfirm->text();
    if (passwordNew != passwordNewConfirm) {
        labelMessage->show();
        labelMessage->setText(tr("Different passwords"));
        editPasswordNew->setFocus();
        return;
    }

    if (passwordNew.size() > 0 && !X::checkPassword(passwordNew.toStdString())) {
        labelMessage->show();
        labelMessage->setText(tr("Password's length should be in [6, 100]"));
        editPasswordNew->setFocus();
        return;
    }

    labelMessage->show();
    labelMessage->setText("Modifying...");

    ModifyThread *thread = new ModifyThread(
        userManager.getToken(),
        nickname,
        email,
        QCryptographicHash::hash(QString::fromStdString(X::saltBegin + passwordOld.toStdString() + X::saltEnd).toLocal8Bit(), QCryptographicHash::Sha1).toHex(),
        passwordNew.size() > 0 ? QCryptographicHash::hash(QString::fromStdString(X::saltBegin + passwordNew.toStdString() + X::saltEnd).toLocal8Bit(), QCryptographicHash::Sha1).toHex() : QString(),
        this
    );
    connect(thread, &ModifyThread::done, this, &DialogModify::slotModifyEnd);
    connect(thread, &ModifyThread::finished, thread, &QObject::deleteLater);
    thread->start();
}

void DialogModify::slotModifyEnd(const int &ec) {
    if (ec == int(X::NoError)) {
        QMessageBox::information(
            this,
            tr("Modify"),
            tr("Modify successfully!")
        );
        userManager.getUser()->setNickname(editNickname->text().toStdString());
        userManager.getUser()->setEmail(editEmail->text().toStdString());
        accept();
    } else {
        if (ec == int(X::ModifyFailed)) {
            labelMessage->setText(tr("Modify failed, check network"));
        } else if (ec == int(X::InvalidNickname)) {
            labelMessage->setText(tr("Invalid nickname"));
            editNickname->setFocus();
        } else if (ec == int(X::InvalidEmail)) {
            labelMessage->setText(tr("Invalid email"));
            editEmail->setFocus();
        } else if (ec == int(X::InvalidPassword)) {
            labelMessage->setText(tr("Invalid password"));
            editPasswordOld->setFocus();
        } else if (ec == int(X::InvalidNewPassword)) {
            labelMessage->setText(tr("Invalid new password"));
            editPasswordNew->setFocus();
        } else {
            labelMessage->setText(QString::fromStdString(X::what(static_cast<X::ErrorCode> (ec))));
        }
    }
}

void DialogModify::setUI() {
    setWindowTitle(tr("Modify"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    editNickname->setPlaceholderText(tr("Nickname"));
    editEmail->setPlaceholderText(tr("Email"));
    editPasswordOld->setPlaceholderText(tr("Old Password"));
    editPasswordOld->setEchoMode(QLineEdit::Password);
    editPasswordNew->setPlaceholderText(tr("New Password (Optional)"));
    editPasswordNew->setEchoMode(QLineEdit::Password);
    editPasswordNewConfirm->setPlaceholderText(tr("New Password Confirm"));
    editPasswordNewConfirm->setEchoMode(QLineEdit::Password);

    editNickname->setText(QString::fromStdString(userManager.getUser()->getNickname()));
    editEmail->setText(QString::fromStdString(userManager.getUser()->getEmail()));

    labelMessage->hide();

    btns->addButton(QDialogButtonBox::Ok)->setText(tr("Con&firm"));
    btns->addButton(QDialogButtonBox::Cancel)->setText(tr("&Cancel"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(editNickname);
    layout->addWidget(editEmail);
    layout->addWidget(editPasswordOld);
    layout->addWidget(editPasswordNew);
    layout->addWidget(editPasswordNewConfirm);
    layout->addWidget(labelMessage);
    layout->addWidget(btns);

    setLayout(layout);
}

void DialogModify::setConnection() {
    connect(
        btns->button(QDialogButtonBox::Cancel),
        &QPushButton::clicked,
        this,
        &DialogModify::close
    );

    connect(
        btns->button(QDialogButtonBox::Ok),
        &QPushButton::clicked,
        this,
        &DialogModify::slotModifyBegin
    );
}
