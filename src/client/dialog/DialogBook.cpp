// copyright 2018 xalanq, chang-ran
// license: lgpl v3.0

#include <functional>

#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QVBoxLayout>

#include <client/dialog/DialogBook.h>
#include <client/dialog/DialogChooseTime.h>
#include <client/dialog/DialogModifyBook.h>
#include <client/thread/ThreadBorrowBook.h>
#include <client/thread/ThreadStarBook.h>
#include <client/thread/ThreadUnStarBook.h>
#include <core/utils.h>

DialogBook::DialogBook(UserManager &userManager, BookManager &bookManager, const X::xint &bookid, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    QDialog(parent) {

    strStar = tr("&Star");
    strUnStar = tr("Un&star");
    strBorrow = tr("&Borrow");
    strBorrowed = tr("Borrowed");

    lblCover = new QLabel(this);
    lblTitle = new QLabel(this);
    lblAuthor = new QLabel(this);
    lblIntroduction = new QLabel(this);
    lblPosition = new QLabel(this);
    lblMaxKeepTime = new QLabel(this);

    lblBookid = new QLabel(this);
    lblPublisher = new QLabel(this);
    lblAmount = new QLabel(this);
    lblISBN = new QLabel(this);

    btnStar = new QPushButton(this);
    btnBorrow = new QPushButton(this);
    btnModify = new QPushButton(this);
    btnMore = new QPushButton(this);

    setUI();
    setConnection();

    bookManager.getBook(bookid, std::bind(&DialogBook::setBook, this, std::placeholders::_1));
}

void DialogBook::setBook(const Book &book) {
    bookPtr = &book;

    setWindowTitle(QString::fromStdString(book.getTitle()) + " - " + QString::fromStdString(book.getAuthor()));

    auto cover = book.getCover();
    QPixmap p(QSize(172, 237));
    if (cover.getSize())
        p.loadFromData((uchar *)cover.getData(), cover.getSize());
    else
        p.fill(Qt::black);
    lblCover->setPixmap(p.scaled(QSize(172, 237)));

    lblTitle->setText(QString::fromStdString(book.getTitle()));
    lblAuthor->setText(QString::fromStdString(book.getAuthor()));
    lblIntroduction->setText(QString::fromStdString(book.getIntroduction()));
    lblPosition->setText(QString::fromStdString(book.getPosition()));

    lblBookid->setText(tr("Bookid: ") + QString::number(book.getBookid()));
    lblPublisher->setText(tr("Publisher: ") + QString::fromStdString(book.getPublisher()));
    lblISBN->setText(tr("ISBN: ") + QString::fromStdString(book.getISBN()));
    lblAmount->setText(tr("Amount: ") + QString::number(book.getAmount()));

    auto maxKeepTime = book.getMaxKeepTime();
    X::xll d = maxKeepTime / 60 / 60 / 24;
    X::xll h = maxKeepTime / 60 / 60 - d * 24;
    X::xll m = maxKeepTime / 60 - d * 24 * 60 - h * 60;
    lblMaxKeepTime->setText(tr("MaxKeepTime: ") + 
                            QString::number(d) + tr("d ") + 
                            QString::number(h) + tr("h ") + 
                            QString::number(m) + tr("m "));

    auto bookid = book.getBookid();
    if (bookid > 0) {
        btnStar->setDisabled(false);
        btnBorrow->setDisabled(false);
        btnMore->setDisabled(false);

        btnStar->setText(strStar);
        if (userManager.isStaredBook(bookid))
            btnStar->setText(strUnStar);

        btnBorrow->setText(strBorrow);
        if (userManager.isBorrowedBook(bookid)) {
            btnBorrow->setText(strBorrowed);
            btnBorrow->setDisabled(true);
        }

        if (userManager.getUser().getPriority() >= std::max(X::xint(X::ADMINISTER), book.getPriority())) {
            btnModify->setDisabled(false);
            btnModify->show();
        }
    }
}

void DialogBook::slotStarBegin() {
    if (btnStar->text() == strStar) {
        auto thread = new ThreadStarBook(userManager.getToken(), bookPtr->getBookid(), this);
        connect(thread, &ThreadStarBook::done, this, &DialogBook::slotStarEnd);
        connect(thread, &ThreadStarBook::finished, thread, &QObject::deleteLater);
        thread->start();
    } else {
        auto thread = new ThreadUnStarBook(userManager.getToken(), bookPtr->getBookid(), this);
        connect(thread, &ThreadUnStarBook::done, this, &DialogBook::slotStarEnd);
        connect(thread, &ThreadUnStarBook::finished, thread, &QObject::deleteLater);
        thread->start();
    }
}

void DialogBook::slotStarEnd(const X::ErrorCode &ec) {
    if (ec != X::NoError) {
        QMessageBox::critical(
            this,
            tr("Failed to star"),
            QString::fromStdString(X::what(ec))
        );
        return;
    }
    if (btnStar->text() == strStar) {
        userManager.starBook(bookPtr->getBookid());
        btnStar->setText(strUnStar);
    } else {
        try {
            userManager.unStarBook(bookPtr->getBookid());
        } catch (std::exception &) {}
        btnStar->setText(strStar);
    }
}

void DialogBook::slotBorrowBegin() {
    auto bookid = bookPtr->getBookid();
    DialogChooseTime dialog(bookManager.getBookBrief(bookid).getMaxKeepTime(), this);
    if (dialog.exec() == QDialog::Accepted) {
        keepTime = dialog.getKeepTime();
        auto thread = new ThreadBorrowBook(userManager.getToken(), bookid, keepTime, this);
        connect(thread, &ThreadBorrowBook::done, this, &DialogBook::slotBorrowEnd);
        connect(thread, &ThreadBorrowBook::finished, thread, &QObject::deleteLater);
        thread->start();
    }
}

void DialogBook::slotBorrowEnd(const X::ErrorCode &ec) {
    if (ec != X::NoError) {
        QMessageBox::critical(
            this,
            tr("Failed to borrow"),
            QString::fromStdString(X::what(ec))
        );
        return;
    }
    QMessageBox::information(
        this,
        tr("Borrow book"),
        tr("Successfully borrow a book: \n") +
        QString::fromStdString(bookPtr->getTitle()) +
        tr("\nPlease go to ") +
        QString::fromStdString(bookPtr->getPosition()) +
        tr(" to get the book")
    );
    auto beginTime = time(0);
    auto endTime = beginTime + keepTime;
    userManager.borrowBook(bookPtr->getBookid(), beginTime, endTime);
    btnBorrow->setText(strBorrowed);
    btnBorrow->setDisabled(true);
}

void DialogBook::slotModify() {
    DialogModifyBook dialog(userManager, bookManager, *bookPtr, this);
    connect(&dialog, &DialogModifyBook::signalModify, this, [this] {
        bookManager.getBook(bookPtr->getBookid(), std::bind(&DialogBook::setBook, this, std::placeholders::_1));
        emit signalModify();
    });
    dialog.exec();
}

void DialogBook::slotMore() {
    if (lblBookid->isHidden()) {
        lblBookid->show();
        lblPublisher->show();
        lblISBN->show();
        lblAmount->show();
    } else {
        lblBookid->hide();;
        lblPublisher->hide();
        lblISBN->hide();
        lblAmount->hide();
    }
}

void DialogBook::setUI() {
    setWindowTitle(tr("Book"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    btnBorrow->setText(strBorrow);
    btnBorrow->setDisabled(true);
    btnStar->setText(strStar);
    btnStar->setDisabled(true);
    btnModify->setText(tr("&Modify"));
    btnModify->setDisabled(true);
    btnModify->hide();
    btnMore->setText(tr("Mo&re"));
    btnMore->setDisabled(true);

    auto layoutInfo = new QVBoxLayout;
    layoutInfo->addWidget(lblTitle);
    layoutInfo->addWidget(lblAuthor);
    layoutInfo->addWidget(lblPosition);
    layoutInfo->addWidget(lblMaxKeepTime);

    layoutInfo->addWidget(lblBookid);
    layoutInfo->addWidget(lblPublisher);
    layoutInfo->addWidget(lblISBN);
    layoutInfo->addWidget(lblAmount);
    slotMore();

    auto layoutUp = new QHBoxLayout;
    layoutUp->addWidget(lblCover);
    layoutUp->addLayout(layoutInfo);
    layoutUp->addStretch();

    auto w = new QWidget(this);
    w->setLayout(layoutUp);
    w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    auto layoutButton = new QHBoxLayout;
    layoutButton->addWidget(btnStar);
    layoutButton->addWidget(btnBorrow);
    layoutButton->addWidget(btnModify);
    layoutButton->addWidget(btnMore);

    auto layout = new QVBoxLayout;
    layout->addWidget(w);
    layout->addWidget(lblIntroduction);
    lblIntroduction->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lblIntroduction->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    lblIntroduction->setWordWrap(true);
    layout->addLayout(layoutButton);

    setLayout(layout);
}

void DialogBook::setConnection() {
    connect(btnStar,
            &QPushButton::clicked,
            this,
            &DialogBook::slotStarBegin);
    connect(btnBorrow,
            &QPushButton::clicked,
            this,
            &DialogBook::slotBorrowBegin);
    connect(btnModify,
            &QPushButton::clicked,
            this,
            &DialogBook::slotModify);
    connect(btnMore,
            &QPushButton::clicked,
            this,
            &DialogBook::slotMore);

}
