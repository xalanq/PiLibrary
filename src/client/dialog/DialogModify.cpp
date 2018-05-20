// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <regex>

#include <QCryptographicHash>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

#include <client/dialog/DialogModify.h>
#include <client/thread/ThreadModify.h>
#include <client/values.h>
#include <core/utils.h>

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

    auto thread = new ThreadModify(
        userManager.getToken(),
        nickname,
        email,
        QCryptographicHash::hash(QString::fromStdString(X::saltBegin + passwordOld.toStdString() + X::saltEnd).toLocal8Bit(), QCryptographicHash::Sha1).toHex(),
        passwordNew.size() > 0 ? QCryptographicHash::hash(QString::fromStdString(X::saltBegin + passwordNew.toStdString() + X::saltEnd).toLocal8Bit(), QCryptographicHash::Sha1).toHex() : QString(),
        this
    );
    connect(thread, &ThreadModify::done, this, &DialogModify::slotModifyEnd);
    connect(thread, &ThreadModify::finished, thread, &QObject::deleteLater);
    thread->start();
}

void DialogModify::slotModifyEnd(const X::ErrorCode &ec) {
    if (ec == X::NoError) {
        QMessageBox::information(
            this,
            tr("Modify"),
            tr("Modify successfully!")
        );
        userManager.getUser().setNickname(editNickname->text().toStdString());
        userManager.getUser().setEmail(editEmail->text().toStdString());
        accept();
    } else {
        if (ec == X::ModifyFailed) {
            labelMessage->setText(tr("Modify failed, check network"));
        } else if (ec == X::InvalidNickname) {
            labelMessage->setText(tr("Invalid nickname"));
            editNickname->setFocus();
        } else if (ec == X::InvalidEmail) {
            labelMessage->setText(tr("Invalid email"));
            editEmail->setFocus();
        } else if (ec == X::InvalidPassword) {
            labelMessage->setText(tr("Invalid password"));
            editPasswordOld->setFocus();
        } else if (ec == X::InvalidNewPassword) {
            labelMessage->setText(tr("Invalid new password"));
            editPasswordNew->setFocus();
        } else {
            labelMessage->setText(QString::fromStdString(X::what(ec)));
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

    editNickname->setText(QString::fromStdString(userManager.getUser().getNickname()));
    editEmail->setText(QString::fromStdString(userManager.getUser().getEmail()));

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
