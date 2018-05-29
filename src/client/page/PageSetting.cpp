// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QGroupBox>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QSettings>
#include <QVBoxLayout>

#include <client/dialog/DialogModifyUser.h>
#include <client/page/PageSetting.h>

PageSetting::PageSetting(UserManager &userManager, QWidget *parent) :
    QWidget(parent),
    userManager(userManager) {

    btnModify = new QPushButton(this);
    btnLogout = new QPushButton(this);

    btnRefresh = new QPushButton(this);

    cbboxLanguage = new QComboBox(this);
    lblLanguage = new QLabel(this);

    setUI();
    setConnection();
}

void PageSetting::slotChangeLanguage(int index) {
    QSettings setting;
    setting.beginGroup("Setting");
    setting.setValue("Language", languageFileName[index]);
    setting.endGroup();
}

void PageSetting::slotModify() {
    DialogModifyUser dialog(userManager, this);
    if (dialog.exec() == QDialog::Accepted)
        emit signalModifyUser();
}

void PageSetting::slotLogout() {
    auto res = QMessageBox::information(this, tr("Logout"), tr("Are you sure??"), QMessageBox::Ok, QMessageBox::Cancel);
    if (res == QMessageBox::Ok)
        emit signalLogout();
}

void PageSetting::setUI() {
    auto groupUser = new QGroupBox(tr("User"), this);
    btnModify->setText(tr("Modify"));
    btnLogout->setText(tr("Logout"));

    auto layoutUser = new QHBoxLayout;
    layoutUser->addWidget(btnModify);
    layoutUser->addWidget(btnLogout);
    layoutUser->addStretch();

    groupUser->setLayout(layoutUser);


    auto groupBook = new QGroupBox(tr("Book"), this);

    btnRefresh->setText(tr("Refresh cache"));
    auto layoutBook = new QHBoxLayout;
    layoutBook->addWidget(btnRefresh);
    layoutBook->addStretch();

    groupBook->setLayout(layoutBook);


    auto groupInterface = new QGroupBox(tr("Interface"));

    languageList.append(tr("Default"));
    languageList.append("English");
    languageList.append(u8"¼òÌåÖÐÎÄ");
    languageFileName.append("default");
    languageFileName.append("en_US");
    languageFileName.append("zh_CN");

    lblLanguage->setText(tr("&Language (need restart)"));
    lblLanguage->setBuddy(cbboxLanguage);
    cbboxLanguage->addItems(languageList);
    cbboxLanguage->setCurrentIndex(languageFileName.indexOf(QSettings().value("Setting/Language", "default").toString()));
    auto layoutInterface = new QHBoxLayout;
    layoutInterface->addWidget(lblLanguage);
    layoutInterface->addWidget(cbboxLanguage);

    groupInterface->setLayout(layoutInterface);


    auto layout = new QVBoxLayout;
    layout->addWidget(groupUser);
    layout->addWidget(groupBook);
    layout->addWidget(groupInterface);
    layout->addStretch();
    setLayout(layout);
}

void PageSetting::setConnection() {
    connect(btnModify, &QPushButton::clicked, this, &PageSetting::slotModify);
    connect(btnLogout, &QPushButton::clicked, this, &PageSetting::slotLogout);
    connect(btnRefresh, SIGNAL(clicked()), this, SIGNAL(signalRefresh()));
    connect(cbboxLanguage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PageSetting::slotChangeLanguage);
}
