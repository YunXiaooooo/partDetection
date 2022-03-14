#include <tchar.h>
#include <future> 
#include "common.h"
#include "imageProcess.h"
#include "CameraDrive.h"


#include <windows.h> //windows.h��������modbus.h ��Ȼ�ᱨ��֪��Ϊʲô

resultCodes testResults;
conditionVariableTool cdTool;

int DisplayConfirmSaveAsMessageBox()
{
    int msgboxID = MessageBox(
        NULL,
        L"��ȷ����Ҫ�رռ�������",
        L"��������������",
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
            printf("ץ���߳��˳�\n");
            break;//�˳��߳�
        }
        else if (cdTool.readGrabFlag() == true)
        {
            originImage[grabNum] = ptrMyCameras->grabForSoftTriggerMode(0);//����0����Ϊ����һ�����
            cdTool.setGrabFlag(false);
            futs.push_back(std::async(imageProcessTask, originImage[grabNum], grabNum));
            printf("ͼƬ[%d]���봦���߳�\n", grabNum);
            grabNum++;

            if (futs.size() == 4)
            {
                grabNum = 0;
                for (int i=0;i<4;i++)
                {
                    testResults.setCodeValue(i, futs[i].get());
                }
                futs.clear();
                //�ķ�ͼ������ϣ�׼�����ͽ��
                printf("�ķ�ͼ������ϣ�׼�����ͽ��\n");
                testResults.setFlag(true);
            }
        }
       
    }
}

