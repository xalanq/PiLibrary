// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QGroupBox>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QMessageBox>
#include <QVBoxLayout>

#include <client/thread/ThreadReturnBook.h>
#include <client/thread/ThreadSetPriority.h>
#include <client/page/PageAdminister.h>
#include <client/utils.h>

PageAdminister::PageAdminister(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    QWidget(parent),
    userManager(userManager),
    bookManager(bookManager) {

    lblReturnBookid = new QLabel(this);
    lblReturnUserid = new QLabel(this);
    editReturnUserid = new QLineEdit(this);
    editReturnBookid = new QLineEdit(this);
    btnReturn = new QPushButton(this);

    lblSetPriorityUserid = new QLabel(this);
    lblSetPriority = new QLabel(this);
    editSetPriorityUserid = new QLineEdit(this);
    cbboxSetPriority = new QComboBox(this);
    btnSetPriority = new QPushButton(this);

    setUI();
    setConnection();
}

void PageAdminister::slotReturn() {
    auto userid = editReturnUserid->text().toInt();
    auto bookid = editReturnBookid->text().toInt();
    auto thread = new ThreadReturnBook(userManager.getToken(), userid, bookid, this);
    connect(thread, &ThreadReturnBook::done, this, [this](const X::ErrorCode &ec) {
        QString text;
        if (ec != X::NoError)
            text = X::what(ec);
        else
            text = tr("Successfully!");
        QMessageBox::information(this, tr("Return Result"), text, QMessageBox::Ok);
    });
    connect(thread, &ThreadReturnBook::finished, thread, &QObject::deleteLater);
    thread->start();
}

void PageAdminister::slotSetPriority() {
    auto userid = editSetPriorityUserid->text().toInt();
    auto priority = priorityInfo[cbboxSetPriority->currentIndex()];
    auto thread = new ThreadSetPriority(userManager.getToken(), userid, priority, this);
    connect(thread, &ThreadSetPriority::done, this, [this](const X::ErrorCode &ec) {
        QString text;
        if (ec != X::NoError)
            text = X::what(ec);
        else
            text = tr("Successfully!");
        QMessageBox::information(this, tr("Set Priority Result"), text, QMessageBox::Ok);
    });
    connect(thread, &ThreadSetPriority::finished, thread, &QObject::deleteLater);
    thread->start();
}

void PageAdminister::setUI() {
    auto layoutReturn = new QVBoxLayout;

    lblReturnUserid->setText(tr("&Userid: "));
    lblReturnUserid->setBuddy(editReturnUserid);
    lblReturnBookid->setText(tr("&Bookid: "));
    lblReturnBookid->setBuddy(editReturnBookid);
    editReturnUserid->setValidator(new QIntValidator(0, 2147483647));
    editReturnBookid->setValidator(new QIntValidator(0, 2147483647));
    btnReturn->setText(tr("&Return"));

    auto layoutReturnUserid = new QHBoxLayout;
    layoutReturnUserid->addWidget(lblReturnUserid);
    layoutReturnUserid->addWidget(editReturnUserid);
    layoutReturnUserid->addStretch();
    layoutReturn->addLayout(layoutReturnUserid);

    auto layoutReturnBookid = new QHBoxLayout;
    layoutReturnBookid->addWidget(lblReturnBookid);
    layoutReturnBookid->addWidget(editReturnBookid);
    layoutReturnBookid->addStretch();
    layoutReturn->addLayout(layoutReturnBookid);

    auto layoutReturnButton = new QHBoxLayout;
    layoutReturnButton->addWidget(btnReturn);
    layoutReturnButton->addStretch();
    layoutReturn->addLayout(layoutReturnButton);

    auto groupReturn = new QGroupBox(tr("Return Book"), this);
    groupReturn->setLayout(layoutReturn);

    
    auto layoutSetPriority = new QVBoxLayout;

    lblSetPriorityUserid->setText(tr("&Userid: "));
    lblSetPriorityUserid->setBuddy(editSetPriorityUserid);;
    lblSetPriority->setText(tr("&Priority: "));
    lblSetPriority->setBuddy(cbboxSetPriority);
    auto priority = userManager.getUser().getPriority();
    if (userManager.getUser().getPriority() > X::USER) {
        priorityList.append(tr("User"));
        priorityInfo.push_back(X::USER);
    }
    if (userManager.getUser().getPriority() > X::ADMINISTER) {
        priorityList.append(tr("Administer"));
        priorityInfo.push_back(X::ADMINISTER);
    }
    if (userManager.getUser().getPriority() > X::SUPER_ADMINISTER) {
        priorityList.append(tr("Super Administer"));
        priorityInfo.push_back(X::SUPER_ADMINISTER);
    }
    cbboxSetPriority->addItems(priorityList);
    btnSetPriority->setText(tr("&Set Priority"));

    auto layoutSetPriorityUserid = new QHBoxLayout;
    layoutSetPriorityUserid->addWidget(lblSetPriorityUserid);
    layoutSetPriorityUserid->addWidget(editSetPriorityUserid);
    layoutSetPriorityUserid->addStretch();
    layoutSetPriority->addLayout(layoutSetPriorityUserid);

    auto layoutSetPriorityCombo = new QHBoxLayout;
    layoutSetPriorityCombo->addWidget(lblSetPriority);
    layoutSetPriorityCombo->addWidget(cbboxSetPriority);
    layoutSetPriorityCombo->addStretch();
    layoutSetPriority->addLayout(layoutSetPriorityCombo);

    auto layoutSetPriorityButton = new QHBoxLayout;
    layoutSetPriorityButton->addWidget(btnSetPriority);
    layoutSetPriorityButton->addStretch();
    layoutSetPriority->addLayout(layoutSetPriorityButton);

    auto groupSetPriority = new QGroupBox(tr("Set Priority"), this);
    groupSetPriority->setLayout(layoutSetPriority);

    auto layout = new QVBoxLayout;
    layout->addWidget(groupReturn);
    layout->addWidget(groupSetPriority);
    layout->addStretch();

    setLayout(layout);

    if (userManager.getUser().getPriority() < X::SUPER_ADMINISTER)
        groupSetPriority->hide();
}

void PageAdminister::setConnection() {
    connect(btnReturn, &QPushButton::clicked, this, &PageAdminister::slotReturn);
    connect(btnSetPriority, &QPushButton::clicked, this, &PageAdminister::slotSetPriority);
}

