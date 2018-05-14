// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <regex>

#include <QCryptographicHash>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

#include <client/dialogsignup.h>

SignUpThread::SignUpThread(const QString &username, const QString &nickname, const QString &password, const QString &email, QObject *parent) :
    username(username.toStdString()),
    nickname(nickname.toStdString()),
    password(password.toStdString()),
    email(email.toStdString()),
    NetworkThread(parent) {
}

void SignUpThread::run() {
    xll token = 0;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    pt.put("username", username);
    pt.put("nickname", nickname);
    pt.put("password", password); 
    pt.put("email", email); 

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::Register, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &) {
        ec = X::RegisterFailed;
    }

    if (ac != X::RegisterFeedback)
        ec = X::RegisterFailed;

    emit done(ec);
}

DialogSignUp::DialogSignUp(QWidget *parent) : 
    QDialog(parent) {

    editUsername = new QLineEdit(this);
    editNickname = new QLineEdit(this);
    editPassword = new QLineEdit(this);
    editPasswordConfirm = new QLineEdit(this);
    editEmail = new QLineEdit(this);

    labelMessage = new QLabel(this);

    btns = new QDialogButtonBox(this);

    setUI();
    setConnection();
}

void DialogSignUp::slotSignUpBegin() {
    labelMessage->hide();

    auto &&username = editUsername->text().toStdString();
    if (username.size() < 1 || username.size() > 100 || !std::regex_match(username, X::patternUsername)) {
        labelMessage->show();
        if (username.size() < 1 || username.size() > 100)
            labelMessage->setText(tr("Username's length should be in [1, 100]"));
        else
            labelMessage->setText(tr("Invalid username"));
        editUsername->setFocus();
        return;
    }

    auto &&nickname = editNickname->text().toStdString();
    if (nickname.size() < 1 || nickname.size() > 100) {
        labelMessage->show();
        labelMessage->setText(tr("Nickname's length should be in [1, 100]"));
        editNickname->setFocus();
        return;
    }

    auto &&password = editPassword->text().toStdString();
    auto &&passwordConfirm = editPasswordConfirm->text().toStdString();
    if (password != passwordConfirm) {
        labelMessage->show();
        labelMessage->setText(tr("Different passwords"));
        editPassword->setFocus();
        return;
    }

    if (password.size() < 6 || password.size() > 100) {
        labelMessage->show();
        labelMessage->setText(tr("Password's length should be in [6, 100]"));
        editPassword->setFocus();
        return;
    }

    auto &&email = editEmail->text().toStdString();
    if (email.size() < 5 || email.size() > 100 || !std::regex_match(email, X::patternEmail)) {
        labelMessage->show();
        if (email.size() < 5 || email.size() > 100)
            labelMessage->setText(tr("Email's length should be in [5, 100]"));
        else
            labelMessage->setText(tr("Invalid email"));
        editEmail->setFocus();
        return;
    }

    labelMessage->show();
    labelMessage->setText("Registering...");

    auto thread = new SignUpThread(
        editUsername->text(),
        editNickname->text(),
        QCryptographicHash::hash(QString::fromStdString(X::saltBegin + editPassword->text().toStdString() + X::saltEnd).toLocal8Bit(), QCryptographicHash::Sha1).toHex(),
        editEmail->text(),
        this
    );
    connect(thread, &SignUpThread::done, this, &DialogSignUp::slotSignUpEnd);
    connect(thread, &SignUpThread::finished, thread, &QObject::deleteLater);
    thread->start();
}

void DialogSignUp::slotSignUpEnd(const int &ec) {
    if (ec == int(X::NoError)) {
        QMessageBox::information(
            this,
            tr("Sign up"),
            tr("Sign up successfully!")
        );
        close();
    } else {
        if (ec == int(X::RegisterFailed)) {
            labelMessage->setText(tr("Sign up failed, check network"));
        } else if (ec == int(X::AlreadyRegister)) {
            labelMessage->setText(tr("Username has already been registered"));
            editUsername->setFocus();
        } else if (ec == int(X::InvalidUsername)) {
            labelMessage->setText(tr("Invalid username"));
            editUsername->setFocus();
        } else if (ec == int(X::InvalidNickname)) {
            labelMessage->setText(tr("Invalid nickname"));
            editNickname->setFocus();
        } else if (ec == int(X::InvalidPassword)) {
            labelMessage->setText(tr("Invalid password"));
            editPassword->setFocus();
        } else if (ec == int(X::InvalidEmail)) {
            labelMessage->setText(tr("Invalid email"));
            editEmail->setFocus();
        } else {
            labelMessage->setText(QString::fromStdString(X::what(static_cast<X::ErrorCode> (ec))));
        }
    }
}

void DialogSignUp::setUI() {
    setWindowTitle(tr("Sign up"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    editUsername->setPlaceholderText(tr("Username"));
    editNickname->setPlaceholderText(tr("Nickname"));
    editPassword->setPlaceholderText(tr("Password"));
    editPassword->setEchoMode(QLineEdit::Password);
    editPasswordConfirm->setPlaceholderText(tr("Confirm password"));
    editPasswordConfirm->setEchoMode(QLineEdit::Password);
    editEmail->setPlaceholderText(tr("Email"));

    labelMessage->hide();

    btns->addButton(QDialogButtonBox::Ok)->setText(tr("Con&firm"));
    btns->addButton(QDialogButtonBox::Cancel)->setText(tr("&Cancel"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(editUsername);
    layout->addWidget(editNickname);
    layout->addWidget(editPassword);
    layout->addWidget(editPasswordConfirm);
    layout->addWidget(editEmail);
    layout->addWidget(labelMessage);
    layout->addWidget(btns);

    setLayout(layout);
}

void DialogSignUp::setConnection() {
    connect(
        btns->button(QDialogButtonBox::Cancel),
        &QPushButton::clicked,
        this,
        &DialogSignUp::close
    );

    connect(
        btns->button(QDialogButtonBox::Ok),
        &QPushButton::clicked,
        this,
        &DialogSignUp::slotSignUpBegin
    );
}
