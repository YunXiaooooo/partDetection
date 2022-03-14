#include <iostream>
#include <memory>
#include "CameraDrive.h"
#include "myTimer.h"
#include "modbusCommuncation.h"
#include "modbus.h"
#include <thread>
#include "common.h"



int main()
{
    //ShowWindow(GetForegroundWindow(), 0);//��ȡ��ǰ��CMD���ڵľ����Ȼ��ͨ��ShowWindow����
    
    std::unique_ptr<CameraDrive> ptrMyCameras(new CameraDrive);//���������ڽ���ʱ�Զ�����
    ptrMyCameras->getExternalCameraNum();
    ptrMyCameras->camerasInitInSoftTriggerMode();//������������ʼ��
    
    std::thread grabThread(grabByHk, std::move(ptrMyCameras));//ptrMyCamerasָ��Ȩ������grabThread�̣߳��߳̽���ʱ�Զ�����
    std::unique_ptr<myTimer> communcationTimer(new myTimer);

    std::unique_ptr<modbusRtuMaster> mbMaster(new modbusRtuMaster(1, "COM1", 9600, 'N', 8, 1));
    if (!mbMaster->modbusRtuMasterInit(0))//������Ե�ַ,�����Ƿ���������
    {
        printf("modbusδ���ӣ����򼴽��رգ�\n");
        cdTool.setTerminnationFlag(true);
        cdTool.conditionVariableToolNotifyAll();
        grabThread.join();

        printf("����رգ�\n");
        return 0;
    }
    else
    {
        printf("modbus���ӳɹ���\n");
    }
    communcationTimer->start(100, myTimerTask, mbMaster.get());



    


    while (DisplayConfirmSaveAsMessageBox() != IDYES);//�ȴ�ȷ�ϳ���ر�

    cdTool.setTerminnationFlag(true);
    cdTool.conditionVariableToolNotifyAll();
    grabThread.join();

    printf("���������ر�\n");


    return 0;
}




