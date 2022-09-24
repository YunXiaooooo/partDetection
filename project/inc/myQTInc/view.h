#pragma once
#ifndef VIEW_H
#define VIEW_H

#include <QtWidgets/QMainWindow>
#include "ui_view.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "CameraDrive.h"
#include "processControl.h"
#include "myTimer.h"
#include "communication.h"
#include<stack>
#include <exception> 
class view : public QMainWindow
{
    Q_OBJECT

public:
    view(QWidget *parent = Q_NULLPTR);

private:
    Ui::projectClass ui;
    std::unique_ptr<CameraDrive> ptrMyCameras = nullptr;
    std::unique_ptr<processControl> ptrProcessControl = nullptr;
    std::shared_ptr<communicationToolProxy> ptrCommunicationToolProxy = nullptr;
    std::unique_ptr<myTimer> ptrMyTimer = nullptr;


    cv::Mat image[12];

    void siganlsConnectInit();
    void Init();

public slots:
    void pushButtonGetImage();
    void imageDisplay(int indx);
    void grab(int grabNum);
    void prepareToReply();
    void Change();

signals:
    void oneImageDispaly(int);
    void oneImageProcess(cv::Mat &img, int num);
    void readyToReply();
};

#endif
