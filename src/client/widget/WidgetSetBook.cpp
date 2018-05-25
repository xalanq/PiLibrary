// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QDir>
#include <QFrame>

#include <QFileDialog>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QSettings>
#include <QVBoxLayout>

#include <client/widget/WidgetSetBook.h>
#include <client/values.h>
#include <core/Resource.h>

WidgetSetBook::WidgetSetBook(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    QWidget(parent) {

    lblCover = new QLabel(this);
    lblBookid = new QLabel(this);
    lblTitle = new QLabel(this);
    lblAuthor = new QLabel(this);
    lblISBN = new QLabel(this);
    lblPublisher = new QLabel(this);
    lblIntroduction = new QLabel(this);
    lblPriority = new QLabel(this);
    lblPosition = new QLabel(this);
    lblAmount = new QLabel(this);
    lblKeepTime = new QLabel(this);
    lblDay = new QLabel(this);
    lblHour = new QLabel(this);
    lblMinute = new QLabel(this);
    lblMessage = new QLabel(this);

    editBookid = new QLineEdit(this);
    editTitle = new QLineEdit(this);
    editAuthor = new QLineEdit(this);
    editISBN = new QLineEdit(this);
    editPublisher = new QLineEdit(this);
    editPosition = new QLineEdit(this);

    spinDay = new QSpinBox(this);
    spinHour = new QSpinBox(this);
    spinMinute = new QSpinBox(this);

    cbboxPriority = new QComboBox(this);
    editAmount = new QLineEdit(this);
    editIntroduction = new QPlainTextEdit(this);

    btnUploadCover = new QPushButton(this);
    btnAdd = new QPushButton(this);

    setUI();
    setConnection();
}

void WidgetSetBook::getData() {
    lblMessage->hide();
    auto keepTime = 0;
    (keepTime *= 0) += spinDay->value();
    (keepTime *= 24) += spinHour->value();
    (keepTime *= 60) += spinMinute->value();
    (keepTime *= 60);
    if (keepTime <= 0) {
        spinDay->setFocus();
        lblMessage->setText(tr("Keep Time must be positive"));
        lblMessage->show();
        return;
    }
    auto bookid = editBookid->text().toInt();
    if (bookid < 0) {
        editBookid->setFocus();
        lblMessage->setText(tr("Bookid must be positive"));
        lblMessage->show();
        return;
    }

    pt.clear();
    pt.put("bookid", bookid);
    if (editTitle->isModified())
        pt.put("title", editTitle->text().toStdString());
    if (editAuthor->isModified())
        pt.put("author", editAuthor->text().toStdString());
    if (editISBN->isModified())
        pt.put("ISBN", editISBN->text().toStdString());
    if (editPublisher->isModified())
        pt.put("publisher", editPublisher->text().toStdString());
    if (editIntroduction->document()->isModified())
        pt.put("introduction", editIntroduction->toPlainText().toStdString());
    pt.put("priority", priorityInfo[cbboxPriority->currentIndex()]);
    if (editAmount->isModified())
        pt.put("amount", editAmount->text().toInt());
    if (editPosition->isModified())
        pt.put("position", editPosition->text().toStdString());
    if (keepTime != beforeKeepTime)
        pt.put("maxKeepTime", keepTime);
}

void WidgetSetBook::slotUpload() {
    QSettings setting;
    setting.beginGroup("FileDialog");
    QString defaultPath;
    static bool first = true;
    if (first) {
        defaultPath = setting.value("defaultPath", QDir::homePath()).toString();
        first = false;
    }
    auto path = QFileDialog::getOpenFileName(this, tr("Upload a book cover"), defaultPath, "Images (*.png *.jpg)");
    if (!path.isNull())
        setting.setValue("defaultPath", QFileInfo(path).absolutePath());
    setting.endGroup();
    cover = Resource::get(path.toStdString());
    if (cover.getSize()) {
        QPixmap p(QSize(114, 160));
        p.loadFromData((uchar *)cover.getData(), cover.getSize());
        lblCover->setPixmap(p.scaled(QSize(114, 160)));
    }
}

void WidgetSetBook::setUI() {
    int maxWidth = 80;
    auto layoutLeft = new QVBoxLayout;
    lblCover->setFrameStyle(QFrame::NoFrame);
    lblCover->setAlignment(Qt::AlignCenter);
    lblCover->setContentsMargins(0, 0, 0, 0);
    lblCover->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    lblCover->setStyleSheet("border: 1px solid black");
    lblCover->setFixedSize(QSize(114, 160));
    lblCover->setText(tr("Cover"));

    btnUploadCover->setText(tr("&Upload"));

    layoutLeft->addWidget(lblCover);
    layoutLeft->addWidget(btnUploadCover);
    layoutLeft->addStretch();

    auto layoutRight = new QVBoxLayout;

    auto layoutBookid = new QHBoxLayout;
    lblBookid->setText(tr("&Bookid: "));
    lblBookid->setBuddy(editBookid);
    lblBookid->setFixedWidth(maxWidth);
    layoutBookid->addWidget(lblBookid);
    layoutBookid->addWidget(editBookid);
    layoutRight->addLayout(layoutBookid);

    auto layoutTitle = new QHBoxLayout;
    lblTitle->setText(tr("&Title: "));
    lblTitle->setBuddy(editTitle);
    lblTitle->setFixedWidth(maxWidth);
    layoutTitle->addWidget(lblTitle);
    layoutTitle->addWidget(editTitle);
    layoutRight->addLayout(layoutTitle);

    auto layoutAuthor = new QHBoxLayout;
    lblAuthor->setText(tr("A&uthor: "));
    lblAuthor->setBuddy(editAuthor);
    lblAuthor->setFixedWidth(maxWidth);
    layoutAuthor->addWidget(lblAuthor);
    layoutAuthor->addWidget(editAuthor);
    layoutRight->addLayout(layoutAuthor);

    auto layoutISBN = new QHBoxLayout;
    lblISBN->setText(tr("&ISBN: "));
    lblISBN->setBuddy(editISBN);
    lblISBN->setFixedWidth(maxWidth);
    layoutISBN->addWidget(lblISBN);
    layoutISBN->addWidget(editISBN);
    layoutRight->addLayout(layoutISBN);

    auto layoutPublisher= new QHBoxLayout;
    lblPublisher->setText(tr("&Publisher: "));
    lblPublisher->setBuddy(editPublisher);
    lblPublisher->setFixedWidth(maxWidth);
    layoutPublisher->addWidget(lblPublisher);
    layoutPublisher->addWidget(editPublisher);
    layoutRight->addLayout(layoutPublisher);

    auto layoutPosition = new QHBoxLayout;
    lblPosition->setText(tr("Po&sition: "));
    lblPosition->setBuddy(editPosition);
    lblPosition->setFixedWidth(maxWidth);
    layoutPosition->addWidget(lblPosition);
    layoutPosition->addWidget(editPosition);
    layoutRight->addLayout(layoutPosition);

    auto layoutKeepTime = new QHBoxLayout;
    lblKeepTime->setText(tr("Keep Time: "));
    lblKeepTime->setFixedWidth(maxWidth);
    lblDay->setText(tr("&Day"));
    lblDay->setBuddy(spinDay);
    lblHour->setText(tr("&Hour"));
    lblHour->setBuddy(spinHour);
    lblMinute->setText(tr("&Minute"));
    lblMinute->setBuddy(spinMinute);
    spinDay->setRange(0, 150 * 365);
    spinDay->setValue(7);
    spinHour->setRange(0, 59);
    spinHour->setValue(0);
    spinMinute->setRange(0, 59);
    spinMinute->setValue(0);
    beforeKeepTime = 0;
    layoutKeepTime->addWidget(lblKeepTime);
    layoutKeepTime->addWidget(spinDay);
    layoutKeepTime->addWidget(lblDay);
    layoutKeepTime->addStretch();
    layoutKeepTime->addWidget(spinHour);
    layoutKeepTime->addWidget(lblHour);
    layoutKeepTime->addStretch();
    layoutKeepTime->addWidget(spinMinute);
    layoutKeepTime->addWidget(lblMinute);
    layoutRight->addLayout(layoutKeepTime);

    auto layoutUp = new QHBoxLayout;
    layoutUp->addLayout(layoutLeft);
    layoutUp->addLayout(layoutRight);

    auto layoutDown = new QHBoxLayout;
    if (userManager.getUser().getPriority() >= X::USER) {
        priorityList.append(tr("User"));
        priorityInfo.push_back(X::USER);
    }
    if (userManager.getUser().getPriority() >= X::ADMINISTER) {
        priorityList.append(tr("Administer"));
        priorityInfo.push_back(X::ADMINISTER);
    }
    if (userManager.getUser().getPriority() >= X::SUPER_ADMINISTER) {
        priorityList.append(tr("Super Administer"));
        priorityInfo.push_back(X::SUPER_ADMINISTER);
    }
    cbboxPriority->addItems(priorityList);
    lblPriority->setText(tr("P&riority: "));
    lblPriority->setBuddy(cbboxPriority);
    layoutDown->addWidget(lblPriority);
    layoutDown->addWidget(cbboxPriority);

    lblAmount->setText(tr("A&mount: "));
    lblAmount->setBuddy(editAmount);
    editAmount->setValidator(new QIntValidator(0, 2147483647));
    layoutDown->addWidget(lblAmount);
    layoutDown->addWidget(editAmount);

    layoutDown->addStretch();

    btnAdd->setText(tr("&Add"));
    layoutDown->addWidget(btnAdd);

    auto layout = new QVBoxLayout;

    auto w = new QWidget(this);
    w->setLayout(layoutUp);
    w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    layout->addWidget(w);

    lblIntroduction->setText(tr("I&ntroduction: "));
    lblIntroduction->setBuddy(editIntroduction);
    editIntroduction->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(lblIntroduction);
    layout->addWidget(editIntroduction);
    layout->addLayout(layoutDown);

    lblMessage->hide();
    layout->addWidget(lblMessage);

    setLayout(layout);
}

void WidgetSetBook::setConnection() {
    connect(btnUploadCover, &QPushButton::clicked, this, &WidgetSetBook::slotUpload);
}
