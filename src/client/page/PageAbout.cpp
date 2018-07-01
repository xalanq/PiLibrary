// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <client/page/PageAbout.h>
#include <client/values.h>

PageAbout::PageAbout(QWidget *parent) :
    QWidget(parent) {

    label = new QLabel(this);
    btnUpdate = new QPushButton(this);

    setUI();
    setConnection();
}

void PageAbout::setUI() {
    auto lblLayout = new QHBoxLayout;
    auto btnLayout = new QHBoxLayout;
    auto layout = new QVBoxLayout;

    lblLayout->addStretch();
    lblLayout->addWidget(label);
    lblLayout->addStretch();

    btnLayout->addStretch();
    btnLayout->addWidget(btnUpdate);
    btnLayout->addStretch();

    layout->addStretch();
    layout->addLayout(lblLayout);
    layout->addLayout(btnLayout);
    layout->addStretch();

    setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
}

void PageAbout::setConnection() {
    label->setText(tr(
        "<p style='font-family: Microsoft YaHei'><center><b><font size=24> %1 v%2</font></b></center></p> \
         <p style='font-family: Microsoft YaHei'>Copyright &copy; 2018 by %3.</p> \
         <p style='font-family: Microsoft YaHei'>Email: %4</p> \
         <p style='font-family: Microsoft YaHei'>Website: <a href=\'http://%5\'>%6</a></p> \
         <p style='font-family: Microsoft YaHei'>Github: <a href=\'https://%7\'>%8</a></p> \
         <p style='font-family: Microsoft YaHei'>Lisence: LGPL v3.0</p>").
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
    label->setWordWrap(true);
    label->setFixedWidth(600);

    btnUpdate->setText(tr("Check update"));
}
