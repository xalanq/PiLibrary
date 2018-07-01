// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QString>
#include <QWidget>

#include <core/utils.h>

namespace X {
    extern QString what(const ErrorCode &ec);
    extern bool loadStyleSheet(QWidget *w, const QString &filename);
}