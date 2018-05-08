// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QFrame>
#include <QHBoxLayout>
#include <QList>
#include <QString>
#include <QVBoxLayout>

#include <client/mainwidget.h>
#include <client/pageabout.h>
#include <client/pagebrowse.h>
#include <client/pagerecord.h>
#include <client/pagereturn.h>
#include <client/pagesetting.h>

MainWidget::MainWidget(UserManager &userManager, QWidget *parent) :
    userManager(userManager),
    QWidget(parent) {

    widgetHead = new WidgetHead(userManager, this);
    listWidget = new QListWidget(this);
    pageWidget = new QStackedWidget(this);
    scrollArea = new QScrollArea(this);

    setUI();
    setConnection();
}

void MainWidget::loadSetting() {

}

void MainWidget::saveSetting() {

}

void MainWidget::setUI() {
    initListWidget();
    initPageWidget();
    scrollArea->setWidget(pageWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::NoFrame);

    auto layoutLeft = new QVBoxLayout;
    layoutLeft->addWidget(widgetHead);
    layoutLeft->addWidget(listWidget);

    auto layout = new QHBoxLayout;

    layout->addLayout(layoutLeft);
    layout->addWidget(scrollArea);

    setLayout(layout);
}

void MainWidget::setConnection() {
    connect(
        listWidget,
        &QListWidget::currentRowChanged,
        pageWidget,
        &QStackedWidget::setCurrentIndex
    );
}

void MainWidget::initListWidget() {
    QStringList items;
    items.append(tr("Browse"));
    items.append(tr("Record"));
    if (userManager.isAdminister())
        items.append(tr("Return"));
    items.append(tr("Setting"));
    items.append(tr("About"));

    listWidget->addItems(items);
    listWidget->setCurrentRow(0);
}

void MainWidget::initPageWidget() {
    QList<QWidget *> items;
    items.append(new PageBrowse(userManager, this));
    items.append(new PageRecord(userManager, this));
    if (userManager.isAdminister())
        items.append(new PageReturn(userManager, this));
    items.append(new PageSetting(userManager, this));
    items.append(new PageAbout(this));

    for (auto item : items)
        pageWidget->addWidget(item);
    pageWidget->setCurrentIndex(0);
}
