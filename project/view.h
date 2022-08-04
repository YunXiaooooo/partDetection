#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_view.h"

class project : public QMainWindow
{
    Q_OBJECT

public:
    project(QWidget *parent = Q_NULLPTR);

private:
    Ui::projectClass ui;





};
