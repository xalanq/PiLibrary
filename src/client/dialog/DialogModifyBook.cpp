// copyright 2018 xalanq, chang-ran
// license: lgpl v3.0

#include <QVBoxLayout>

#include <client/thread/ThreadSetBook.h>
#include <client/dialog/DialogModifyBook.h>

WidgetModifyBook::WidgetModifyBook(UserManager &userManager, const Book &book, QWidget *parent) :
    book(book),
    WidgetSetBook(userManager, parent) {

    setUI();
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
            cbboxPrirority->setCurrentIndex(i);
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
}

DialogModifyBook::DialogModifyBook(UserManager &userManager, const Book &book, QWidget *parent) :
    userManager(userManager),
    book(book),
    QDialog(parent) {

    w = new WidgetModifyBook(userManager, book, this);
        
    setUI();
}

void DialogModifyBook::setUI() {
    setWindowTitle(tr("Modify Book"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto layout = new QVBoxLayout;
    layout->addWidget(w);

    setLayout(layout);
}
