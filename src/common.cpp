#include <tchar.h>
#include <future> 
#include "common.h"
#include "imageProcess.h"
#include "CameraDrive.h"


#include <windows.h> //windows.h必须晚于modbus.h 不然会报错不知道为什么

resultCodes testResults;
conditionVariableTool cdTool;

int DisplayConfirmSaveAsMessageBox()
{
    int msgboxID = MessageBox(
        NULL,
        L"您确认需要关闭检测程序吗？",
        L"检测程序正在运行",
        MB_ICONEXCLAMATION | MB_YESNO
    );

    if (msgboxID == IDYES)
    {
        // TODO: add code
    }

    return msgboxID;
}



void grabByHk(std::unique_ptr<CameraDrive> ptrMyCameras)
{
    int grabNum = 0;
    cv::Mat originImage[4];
    std::vector<std::future < unsigned int >> futs;
    while (1)
    {
        cdTool.conditionVariableToolWait();

        if (cdTool.readTerminnationFlag() == true)
        {   
            cdTool.setTerminnationFlag(false);
            for (auto& fut : futs)
            {
                fut.get();
            }
            printf("抓拍线程退出\n");
            break;//退出线程
        }
        else if (cdTool.readGrabFlag() == true)
        {
            originImage[grabNum] = ptrMyCameras->grabForSoftTriggerMode(0);//传入0是因为仅有一个相机
            cdTool.setGrabFlag(false);
            futs.push_back(std::async(imageProcessTask, originImage[grabNum], grabNum));
            printf("图片[%d]加入处理线程\n", grabNum);
            grabNum++;

            if (futs.size() == 4)
            {
                grabNum = 0;
                for (int i=0;i<4;i++)
                {
                    testResults.setCodeValue(i, futs[i].get());
                }
                futs.clear();
                //四幅图像处理完毕，准备发送结果
                printf("四幅图像处理完毕，准备发送结果\n");
                testResults.setFlag(true);
            }
        }
       
    }
}

