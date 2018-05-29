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
#include <QPalette>
#include <client/dialog/DialogLogin.h>
#include <client/dialog/DialogSignUp.h>
#include <client/thread/ThreadLogin.h>
#include <client/values.h>
#include <client/utils.h>

DialogLogin::DialogLogin(UserManager &userManager, QWidget *parent) :
    QDialog(parent),
    userManager(userManager) {

    cbboxUsername = new QComboBox(this);
    editPassword = new QLineEdit(this);

    labelMessage = new QLabel(this);

    
	btnLogin = new QPushButton(this);
	btnSignUp = new QPushButton(this);

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
            s = X::what(ec);
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

	labelMessage->setGeometry(50, 180, 400, 30);
	labelMessage->hide();

	cbboxUsername->setEditable(true);
	cbboxUsername->lineEdit()->setPlaceholderText(tr("Username"));
	cbboxUsername->setGeometry(50, 60, 200, 40);
	cbboxUsername->setStyleSheet("QComboBox::dro-down{subcontrol - origin: padding;subcontrol - position: top right;width: 20px;border - left - width: 1px;border - left - color: darkgray;border - left - style: solid; border - top - right - radius: 3px;border - bottom - right - radius: 3px;}");

    editPassword->setEchoMode(QLineEdit::Password);
    editPassword->setPlaceholderText(tr("Password"));
	editPassword->setGeometry(50, 98, 200, 40);
	editPassword->setStyleSheet("border:2px solid gray;border-bottom-left-radius:10px;border-bottom-right-radius:10px;padding:2px 4px");

    btnLogin->setText(tr("&Log in"));
    btnSignUp->setText(tr("&Sign up"));

	btnLogin->setStyleSheet("QPushButton{background-color:rgb(120,170,255);\
							border:2px groove gray;\
							border-radius:10px;\
							padding:2px 4px} \
							QPushButton:hover{ background-color:rgb(85, 170, 255); }\
							QPushBotton:pressed{background-color:black}");
	btnSignUp->setStyleSheet("QPushButton{background-color:rgb(120,170,255);\
							border:2px groove gray; \
							border-radius:10px; \
							padding:2px 4px} \
							QPushButton:hover{ background-color:rgb(85, 170, 255); } \
							QPushBotton:pressed{background-color:black}");

	btnLogin->setGeometry(75, 190, 150, 50);
	btnSignUp->setGeometry(75, 260, 150, 35);
   
	setFixedSize(300, 350);
   
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    setStyleSheet("QDialog{background-image: url(:/images/login.jpg);}");
}

void DialogLogin::setConnection() {
    connect(
        btnLogin,
        &QPushButton::clicked,
        this,
        &DialogLogin::slotLoginBegin
    );
	
	connect(
		btnSignUp,
		&QPushButton::clicked,
		this,
		&DialogLogin::slotRegister
	);
}
