// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <regex>

#include <QCryptographicHash>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

#include <client/dialog/DialogSignUp.h>
#include <client/thread/ThreadSignUp.h>
#include <client/values.h>
#include <core/utils.h>

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
    if (!X::checkUsername(username)) {
        labelMessage->show();
        if (username.size() < 1 || username.size() > 100)
            labelMessage->setText(tr("Username's length should be in [1, 100]"));
        else
            labelMessage->setText(tr("Invalid username"));
        editUsername->setFocus();
        return;
    }

    auto &&nickname = editNickname->text().toStdString();
    if (!X::checkNickname(nickname)) {
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

    if (!X::checkPassword(password)) {
        labelMessage->show();
        labelMessage->setText(tr("Password's length should be in [6, 100]"));
        editPassword->setFocus();
        return;
    }

    auto &&email = editEmail->text().toStdString();
    if (!X::checkEmail(email)) {
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
            labelMessage->setText(tr("Sign up failed, check network"));
        } else if (ec == X::AlreadyRegister) {
            labelMessage->setText(tr("Username has already been registered"));
            editUsername->setFocus();
        } else if (ec == X::InvalidUsername) {
            labelMessage->setText(tr("Invalid username"));
            editUsername->setFocus();
        } else if (ec == X::InvalidNickname) {
            labelMessage->setText(tr("Invalid nickname"));
            editNickname->setFocus();
        } else if (ec == X::InvalidPassword) {
            labelMessage->setText(tr("Invalid password"));
            editPassword->setFocus();
        } else if (ec == X::InvalidEmail) {
            labelMessage->setText(tr("Invalid email"));
            editEmail->setFocus();
        } else {
            labelMessage->setText(QString::fromStdString(X::what(X::ErrorCode(ec))));
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
	
	editUsername->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
	editNickname->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
	editPassword->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
	editPasswordConfirm->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
	editEmail->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");



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
