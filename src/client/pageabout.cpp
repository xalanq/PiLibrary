// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <client/pageabout.h>
#include <client/xclient.h>

PageAbout::PageAbout(QWidget *parent) :
    QWidget(parent) {

    label = new QLabel(this);
    btnUpdate = new QPushButton(this);

    setUI();
    setConnection();
}

void PageAbout::setUI() {
    auto btnLayout = new QHBoxLayout;
    auto layout = new QVBoxLayout;

    btnLayout->addStretch();
    btnLayout->addWidget(btnUpdate);
    btnLayout->addStretch();

    layout->addStretch();
    layout->addWidget(label);
    layout->addLayout(btnLayout);
    layout->addStretch();

    setLayout(layout);
}

void PageAbout::setConnection() {
    label->setText(tr(
        "<center><b> %1 v%2</b></center> \
         <p>Copyright &copy; 2018 by %3.</p> \
         <p>Email: %4</p> \
         <p>Website: <a href=\'http://%5\'>%6</a></p> \
         <p>Github: <a href=\'https://%7\'>%8</a></p> \
         <p>Lisence: LGPL v3.0</p>").
        arg(QString::fromStdString(X::APP_NAME).toHtmlEscaped()).
        arg(QString::fromStdString(X::VERSION).toHtmlEscaped()).
        arg(QString::fromStdString(X::AUTHOR[0] + ", " + X::AUTHOR[1]).toHtmlEscaped()).
        arg(QString::fromStdString(X::EMAIL[0] + ", " + X::EMAIL[1]).toHtmlEscaped()).
        arg(QString::fromStdString(X::WEBSITE).toHtmlEscaped()).
        arg(QString::fromStdString(X::WEBSITE).toHtmlEscaped()).
        arg(QString::fromStdString(X::GITHUB).toHtmlEscaped()).
        arg(QString::fromStdString(X::GITHUB).toHtmlEscaped())
    );
    label->setOpenExternalLinks(true);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    label->setWordWrap(true);

    btnUpdate->setText(tr("Check update"));
}
