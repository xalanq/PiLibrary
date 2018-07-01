// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <regex>

#include <QCryptographicHash>
#include <QFile>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

#include <client/dialog/DialogSignUp.h>
#include <client/thread/ThreadSignUp.h>
#include <client/utils.h>
#include <client/values.h>

DialogSignUp::DialogSignUp(QWidget *parent) : 
    QDialog(parent) {

    editUsername = new QLineEdit(this);
    editNickname = new QLineEdit(this);
    editPassword = new QLineEdit(this);
    editPasswordConfirm = new QLineEdit(this);
    editEmail = new QLineEdit(this);

    lblMessage = new QLabel(this);

    btns = new QDialogButtonBox(this);

    setUI();
    setConnection();
}

void DialogSignUp::slotSignUpBegin() {
    lblMessage->hide();

    auto &&username = editUsername->text().toStdString();
    if (!X::checkUsername(username)) {
        lblMessage->show();
        if (username.size() < 1 || username.size() > 100)
            lblMessage->setText(tr("Username's length should be in [1, 100]"));
        else
            lblMessage->setText(tr("Invalid username"));
        editUsername->setFocus();
        return;
    }

    auto &&nickname = editNickname->text().toStdString();
    if (!X::checkNickname(nickname)) {
        lblMessage->show();
        lblMessage->setText(tr("Nickname's length should be in [1, 100]"));
        editNickname->setFocus();
        return;
    }

    auto &&password = editPassword->text().toStdString();
    auto &&passwordConfirm = editPasswordConfirm->text().toStdString();
    if (password != passwordConfirm) {
        lblMessage->show();
        lblMessage->setText(tr("Different passwords"));
        editPassword->setFocus();
        return;
    }

    if (!X::checkPassword(password)) {
        lblMessage->show();
        lblMessage->setText(tr("Password's length should be in [6, 100]"));
        editPassword->setFocus();
        return;
    }

    auto &&email = editEmail->text().toStdString();
    if (!X::checkEmail(email)) {
        lblMessage->show();
        if (email.size() < 5 || email.size() > 100)
            lblMessage->setText(tr("Email's length should be in [5, 100]"));
        else
            lblMessage->setText(tr("Invalid email"));
        editEmail->setFocus();
        return;
    }

    lblMessage->show();
    lblMessage->setText("Registering...");

    auto thread = new ThreadSignUp(
        editUsername->text(),
        editNickname->text(),
        QCryptographicHash::hash(QString::fromStdString(X::saltBegin + editPassword->text().toStdString() + X::saltEnd).toLocal8Bit(), QCryptographicHash::Sha1).toHex(),
        editEmail->text(),
        this
    );
    connect(thread, &ThreadSignUp::done, this, &DialogSignUp::slotSignUpEnd);
    connect(thread, &ThreadSignUp::finished, thread, &QObject::deleteLater);
    thread->start();
}

void DialogSignUp::slotSignUpEnd(const X::ErrorCode &ec) {
    if (ec == X::NoError) {
        QMessageBox::information(
            this,
            tr("Sign up"),
            tr("Sign up successfully!")
        );
        close();
    } else {
        if (ec == X::RegisterFailed) {
            lblMessage->setText(tr("Sign up failed, check network"));
        } else if (ec == X::AlreadyRegister) {
            lblMessage->setText(tr("Username has already been registered"));
            editUsername->setFocus();
        } else if (ec == X::InvalidUsername) {
            lblMessage->setText(tr("Invalid username"));
            editUsername->setFocus();
        } else if (ec == X::InvalidNickname) {
            lblMessage->setText(tr("Invalid nickname"));
            editNickname->setFocus();
        } else if (ec == X::InvalidPassword) {
            lblMessage->setText(tr("Invalid password"));
            editPassword->setFocus();
        } else if (ec == X::InvalidEmail) {
            lblMessage->setText(tr("Invalid email"));
            editEmail->setFocus();
        } else {
            lblMessage->setText(X::what(X::ErrorCode(ec)));
        }
    }
}

void DialogSignUp::setUI() {
    setWindowTitle(tr("Sign up"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);


    X::loadStyleSheet(this, ":/style/DialogSignUp/style.css");

    editUsername->setPlaceholderText(tr("Username"));
    editUsername->setFixedHeight(50);
    editNickname->setPlaceholderText(tr("Nickname"));
    editNickname->setFixedHeight(50);
    editPassword->setPlaceholderText(tr("Password"));
    editPassword->setFixedHeight(50);
    editPassword->setEchoMode(QLineEdit::Password);
    editPassword->setFixedHeight(50);
    editPasswordConfirm->setPlaceholderText(tr("Confirm password"));
    editPasswordConfirm->setEchoMode(QLineEdit::Password);
    editPasswordConfirm->setFixedHeight(50);
    editEmail->setPlaceholderText(tr("Email"));
    editEmail->setFixedHeight(50);

    lblMessage->hide();
    lblMessage->setWordWrap(true);

    btns->addButton(QDialogButtonBox::Ok)->setText(tr("Con&firm"));
    btns->addButton(QDialogButtonBox::Cancel)->setText(tr("&Cancel"));
    btns->button(QDialogButtonBox::Ok)->setFixedSize(80, 50);
    btns->button(QDialogButtonBox::Cancel)->setFixedSize(80, 50);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(editUsername);
    layout->addWidget(editNickname);
    layout->addWidget(editPassword);
    layout->addWidget(editPasswordConfirm);
    layout->addWidget(editEmail);
    layout->addStretch();
    layout->addWidget(lblMessage);
    layout->addWidget(btns);

    setLayout(layout);
    setFixedWidth(300);
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
