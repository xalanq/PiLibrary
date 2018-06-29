// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QHBoxLayout>
#include <QImage>
#include <QListView>
#include <QPushButton>
#include <QSettings>
#include <QStringList>
#include <QVBoxLayout>

#include <client/widget/WidgetLogin.h>
#include <client/dialog/DialogSignUp.h>
#include <client/thread/ThreadLogin.h>
#include <client/values.h>
#include <client/utils.h>

WidgetLogin::WidgetLogin(UserManager &userManager, QWidget *parent) :
    QWidget(parent),
    userManager(userManager) {

    title = new WidgetLoginTitle(parent, this);

    setUI();
    setConnection();
    loadSetting();
}

WidgetLogin::~WidgetLogin() {
    saveSetting();
}

void WidgetLogin::loadSetting() {
    QSettings setting;
    setting.beginGroup("Login");

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

void WidgetLogin::saveSetting() {
    if (userManager.getToken()) {
        QSettings setting;
        setting.beginGroup("Login");

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

void WidgetLogin::slotLoginBegin() {
    lblMessage->show();
    lblMessage->setText(tr("Connecting..."));
    QString username = cbboxUsername->currentText();
    QString password = QCryptographicHash::hash(QString::fromStdString(X::saltBegin + editPassword->text().toStdString() + X::saltEnd).toLocal8Bit(), QCryptographicHash::Sha1).toHex();

    auto thread = new ThreadLogin(std::move(username), std::move(password), this);
    connect(thread, &ThreadLogin::done, this, &WidgetLogin::slotLoginEnd);
    connect(thread, &ThreadLogin::finished, thread, &QObject::deleteLater);
    thread->start();
}

void WidgetLogin::slotLoginEnd(const X::ErrorCode &ec, const X::xll &token, const X::ptree &pt) {
    if (ec == X::NoError) {
        userManager.setToken(token);
        userManager.setUser(pt);
        emit signalAccept();
    } else {
        QString s;
        if (ec == X::NoSuchUser)
            s = tr("Wrong username or password");
        else if (ec == X::LoginFailed)
            s = tr("Login failed, check network");
        else
            s = X::what(ec);
        lblMessage->setText(s);
    }
}

void WidgetLogin::slotRegister() {
    DialogSignUp dialog(this);
    dialog.exec();
}

void WidgetLogin::setUI() {
    setupUi(this);

    QFile f(":/style/WidgetLogin/style.css");
    f.open(QFile::ReadOnly);
    if (f.isOpen())
        setStyleSheet(f.readAll());

    QImage img(":/images/WidgetLogin/head.jpg");
    lblHead->setFrameStyle(QFrame::NoFrame);
    lblHead->setAlignment(Qt::AlignCenter);
    lblHead->setContentsMargins(0, 0, 0, 0);
    lblHead->setStyleSheet("");
    lblHead->setPixmap(QPixmap::fromImage(img.scaled(lblHead->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    lblHead->move(0, 0);

	lblMessage->hide();
    lblMessage->setAlignment(Qt::AlignCenter);
    lblMessage->setWordWrap(true);

	cbboxUsername->setEditable(true);
	cbboxUsername->lineEdit()->setPlaceholderText(tr("Username"));
    cbboxUsername->setView(new QListView());

    editPassword->setPlaceholderText(tr("Password"));

    btnLogin->setText(tr("&Log in"));
    btnSignUp->setText(tr("&Sign up"));
    btnLogin->setFocus();

    title->setFixedWidth(this->width());
    title->move(0, 0);
    title->raise();

    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(backgroundRole(), Qt::white);
    setPalette(pal);
}

void WidgetLogin::setConnection() {
    connect(
        btnLogin,
        &QPushButton::clicked,
        this,
        &WidgetLogin::slotLoginBegin
    );
	
	connect(
		btnSignUp,
		&QPushButton::clicked,
		this,
		&WidgetLogin::slotRegister
	);

    connect(
        title,
        &WidgetLoginTitle::signalMin,
        this,
        [this] {
            parentWidget()->setWindowState(Qt::WindowMinimized);
        }
    );

    connect(
        title,
        &WidgetLoginTitle::signalClose,
        parentWidget(),
        &QDialog::close
    );
}
