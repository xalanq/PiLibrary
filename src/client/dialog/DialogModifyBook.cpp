// copyright 2018 xalanq, chang-ran
// license: lgpl v3.0

#include <QMessageBox>
#include <QVBoxLayout>

#include <client/thread/ThreadSetBook.h>
#include <client/dialog/DialogModifyBook.h>
#include <client/utils.h>

WidgetModifyBook::WidgetModifyBook(UserManager &userManager, BookManager &bookManager, const Book &book, QWidget *parent) :
    book(book),
    WidgetSetBook(userManager, bookManager, parent) {

    setUI();
    setConnection();
}

void WidgetModifyBook::slotModify() {
    WidgetSetBook::getData();
    bookManager.updateBook(pt, cover, std::bind(&WidgetModifyBook::slotModifyEnd, this, std::placeholders::_1));
}

void WidgetModifyBook::slotModifyEnd(const X::ErrorCode &ec) {
    if (ec == X::NoError) {
        QMessageBox::information(this, tr("Modify result"), tr("Successfully!"));
        emit signalModify();
    } else {
        lblMessage->setText(X::what(ec));
        lblMessage->show();
    }
}

void WidgetModifyBook::setUI() {
    lblBookid->show();
    editBookid->show();
    editBookid->setText(QString::number(book.getBookid()));
    editBookid->setDisabled(true);
    editTitle->setText(QString::fromStdString(book.getTitle()));
    editAuthor->setText(QString::fromStdString(book.getAuthor()));
    editISBN->setText(QString::fromStdString(book.getISBN()));
    editPublisher->setText(QString::fromStdString(book.getPublisher()));
    editIntroduction->document()->setPlainText(QString::fromStdString(book.getIntroduction()));
    editIntroduction->document()->setModified(false);
    auto priority = book.getPriority();
    for (int i = 0; i < priorityInfo.size(); ++i)
        if (priorityInfo[i] == priority) {
            cbboxPriority->setCurrentIndex(i);
            break;
        }
    editPosition->setText(QString::fromStdString(book.getPosition()));
    editAmount->setText(QString::number(book.getAmount()));

    auto maxKeepTime = book.getMaxKeepTime();
    X::xll d = maxKeepTime / 60 / 60 / 24;
    X::xll h = maxKeepTime / 60 / 60 - d * 24;
    X::xll m = maxKeepTime / 60 - d * 24 * 60 - h * 60;
    spinDay->setValue(d);
    spinHour->setValue(h);
    spinMinute->setValue(m);
    beforeKeepTime = maxKeepTime;

    btnAdd->setText(tr("Modif&y"));

    if (book.getCover().getSize()) {
        QPixmap p(QSize(114, 160));
        p.loadFromData((uchar *)book.getCover().getData(), book.getCover().getSize());
        lblCover->setPixmap(p.scaled(QSize(114, 160)));
    }
}

void WidgetModifyBook::setConnection() {
    connect(btnAdd, &QPushButton::clicked, this, &WidgetModifyBook::slotModify);
}

DialogModifyBook::DialogModifyBook(UserManager &userManager, BookManager &bookManager, const Book &book, QWidget *parent) :
    QDialog(parent) {

    w = new WidgetModifyBook(userManager, bookManager, book, this);
        
    setUI();
    setConnection();
}

void DialogModifyBook::setUI() {
    setWindowTitle(tr("Modify Book"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto layout = new QVBoxLayout;
    layout->addWidget(w);

    setLayout(layout);
}

void DialogModifyBook::setConnection() {
    connect(w, SIGNAL(signalModify()), this, SIGNAL(signalModify()));
}
