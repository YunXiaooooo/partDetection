#include <iostream>
#include "CameraDrive.h"
#include "myTimer.h"
#include "modbusCommuncation.h"
#include "modbus.h"
#include <thread>
#include "common.h"



int main()
{
    //ShowWindow(GetForegroundWindow(), 0);//��ȡ��ǰ��CMD���ڵľ����Ȼ��ͨ��ShowWindow����

    CameraDrive* ptrMyCameras = new CameraDrive;
    ptrMyCameras->getExternalCameraNum();
    ptrMyCameras->camerasInitInSoftTriggerMode();//������������ʼ��
    
    std::thread grabThread(grabByHk, ptrMyCameras);

    myTimer* communcationTimer = new myTimer;

    modbusRtuMaster* mbMaster = new modbusRtuMaster(1, "COM1", 9600, 'N', 8,  1);
    if (!mbMaster->modbusRtuMasterInit(0))//������Ե�ַ,�����Ƿ���������
    {
        delete communcationTimer;
        delete mbMaster;
        delete ptrMyCameras;
        printf("modbusδ���ӣ�����رգ�\n");
        return 0;
    }
    else
    {
        printf("modbus���ӳɹ���\n");
    }
    communcationTimer->start(100, myTimerTask, mbMaster);



    


    while (DisplayConfirmSaveAsMessageBox() != IDYES);//�ȴ�ȷ�ϳ���ر�

    delete communcationTimer;
    cdTool.setTerminnationFlag(true);
    cdTool.conditionVariableToolNotifyAll();
    grabThread.join();

    delete mbMaster;
    delete ptrMyCameras;
    printf("���������ر�\n");


    return 0;
}




