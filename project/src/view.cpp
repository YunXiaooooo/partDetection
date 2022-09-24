#include "view.h"
#include <filesystem>

view::view(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    Init();
    siganlsConnectInit();


}

void view::siganlsConnectInit()
{
    connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(Change()));
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(pushButtonGetImage()));
    connect(this, SIGNAL(oneImageDispaly(int)), this, SLOT(imageDisplay(int)));
    connect(this, SIGNAL(oneImageProcess(cv::Mat&, int)), ptrProcessControl.get(), SLOT(enqueue(cv::Mat&, int)));
    connect(this, SIGNAL(readyToReply()), this, SLOT(prepareToReply()));
    connect(ptrCommunicationToolProxy.get(), SIGNAL(toGrab(int)), this, SLOT(grab(int)));
}
void view::Init()
{
    //相机初始化
    if (ptrMyCameras == nullptr)
    {
        try {
            ptrMyCameras = std::make_unique<CameraDrive>();
        }
        catch (std::exception& e)
        {
            printf("New Camera Failure (%s)\n", e.what());
            //std::exit(1);//abort不做任何释放，exit会做部分清理
        }
    }
    try {
        int ExternalCameraNum = ptrMyCameras->getExternalCameraNum();
        bool initFlag = ptrMyCameras->camerasInitInSoftTriggerMode();//相机软件触发初始化
        if (ExternalCameraNum < 1 || initFlag == false)
        {
            printf("Init Camera Failure :ExternalCameraNum = %d, initFlag = %d\n", ExternalCameraNum, initFlag);
            //std::exit(1);
        }

    }
    catch (std::exception& e)
    {
        printf("Init Camera Failure (%s)\n", e.what());
        //std::exit(1);
    }

    if (ptrProcessControl == nullptr)
    {
        try {
            ptrProcessControl = std::make_unique<processControl>();
        }
        catch (std::exception& e)
        {
            printf("New processControl Failure (%s)\n", e.what());
            std::exit(1);
        }
    }
    if (ptrCommunicationToolProxy == nullptr)
    {
        try {
            ptrCommunicationToolProxy = std::make_shared<communicationToolProxy>();
        }
        catch (std::exception& e)
        {
            printf("New communicationToolProxy Failure (%s)\n", e.what());
            std::exit(1);
        }
    }
    if (ptrMyTimer == nullptr)
    {
        try {
            ptrMyTimer = std::make_unique<myTimer>();
        }
        catch (std::exception& e)
        {
            printf("New myTimer Failure (%s) \n", e.what());
            std::exit(1);
        }
        ptrMyTimer->start(200, myTimerTask, ptrCommunicationToolProxy);//第一个参数为定时器间隔 ms
    }
}


void view::imageDisplay(int indx)
{
    cv::Mat pix;
    printf("image[indx].channels()==%d \n", image[indx].channels());
    if (image[indx].channels() != 3)
    {
        cv::cvtColor(image[indx], pix, CV_GRAY2RGB);
    }
    else
    {
        pix = image[indx].clone();
    }
    QImage singleQImage(pix.data, pix.cols, pix.rows, pix.step, QImage::Format_RGB888);
    static std::vector<QLabel*> ptrLabelForImage{ ui.label_1,ui.label_2,ui.label_3,ui.label_4,
    ui.label_5 ,ui.label_6,ui.label_7,ui.label_8,
    ui.label_9,ui.label_10,ui.label_11,ui.label_12 };
    ptrLabelForImage[indx]->setPixmap(QPixmap::fromImage(singleQImage).scaled(ptrLabelForImage[indx]->width(), ptrLabelForImage[indx]->height(), Qt::KeepAspectRatio));
}

void view::pushButtonGetImage()
{
    static int indx = 0;
    grab(indx);
    indx++;
    if (indx >= 12)
    {
        indx = 0;
    }
}
void view::Change()
{
    static bool flag = false;
    flag = !flag;
    ptrCommunicationToolProxy->ChangeAllResultSetOne();
    if (flag)
    {
        ui.pushButton_3->setText("Change:test Result is [1]");
    }
    else
    {
        ui.pushButton_3->setText("Change:test Result is [1-4]");
    }
}

int getFileNumber(std::string folderPath) //需要c++17标准，此文件使用c++17编译
{
    using namespace std::filesystem;
    if (!exists(folderPath))		// 如果目录不存在
        return 0;
    path root(folderPath);
    //如果路径不是目录也不是文件 
    if (!is_directory(root) && is_regular_file(root)) {
        return 0;
    }
    // 如果只是单个文件，返回1
    if (is_regular_file(root)) {
        return 1;
    }
    // DFS
    std::stack<path> st;
    st.push(root);
    int files = 0;
    while (!st.empty()) {
        path folder = st.top();
        st.pop();
        directory_entry entry(folder);	//文件入口
        directory_iterator list(entry);	//文件入口容器
        for (auto& it : list) {
            if (is_regular_file(it)) {	//文件的话，文件数++
                ++files;
            }
            if (is_directory(it)) {	//目录的话，入栈
                st.push(it);
            }
        }
    }
    return files;
    //return 0;
}
void view::grab(int grabNum)
{
    static int imageNum = getFileNumber("./grabPic/0");
    //try
    //{
    //    printf("waitting grab \n");
    //    image[grabNum] = ptrMyCameras->grabForSoftTriggerMode(0);//传入0是因为仅有一个相机
    //    printf("grab finished \n");
    //}
    //catch (std::exception& e)
    //{
    //    printf("grabForSoftTrigger failure! (%s) \n", e.what());
    //    std::exit(1);
    //}
    //try{
    //    cv::imwrite("./grabPic/" + std::to_string(grabNum) + "/" + std::to_string(imageNum) + ".bmp", image[grabNum]);
    //}
    //catch (std::exception& e)
    //{
    //    printf("save the grab image failure! (%s) \n", e.what());
    //}
    //ptrCommunicationToolProxy->setGrabFinishedFlag(true, grabNum);
    //emit oneImageProcess(image[grabNum], grabNum);
    //emit oneImageDispaly(grabNum);

    //if (grabNum==11)
    //{
    //    imageNum++;
    //    //等待处理结果并发送
    //    emit readyToReply();
    //}


    //无相机测试
    try
    {
        image[grabNum] = cv::imread("./grabPic0825/" + std::to_string(grabNum) + "/2.jpg", cv::IMREAD_GRAYSCALE);
    }
    catch (std::exception& e)
    {
        printf("读图失败:%s \n", e.what());
    }
    ptrCommunicationToolProxy->setGrabFinishedFlag(true, grabNum);
    emit oneImageProcess(image[grabNum], grabNum);
    emit oneImageDispaly(grabNum);
    if (grabNum == 11)
    {
        //等待处理结果并发送
        emit readyToReply();
    }
}

void view::prepareToReply()
{
    ptrProcessControl->waitTestResults();
    std::vector<std::vector<int>> testResults = ptrProcessControl->getTestResults();
    ptrCommunicationToolProxy->setTestResults(testResults);
    ptrCommunicationToolProxy->setTestCompeleteFlag(true);
 
    //打印结果
    std::string s = "/**********************************************************************/\n";
    for (int i = 0, ilength = testResults.size(); i < ilength; ++i)
    {
        for (int j = 0, jlength = testResults[i].size(); j < jlength; ++j)
        {
            s = s + std::to_string(testResults[i][j]) + "    ";
        }
        s = s + "\n";
    }
    s = s + "/**********************************************************************/\n";
    printf("%s \n",s.c_str());
}