#include <iostream>
#include "CameraDrive.h"
#include "myTimer.h"
#include "modbusCommuncation.h"
#include "modbus.h"
#include <thread>
#include "common.h"



int main()
{
    //ShowWindow(GetForegroundWindow(), 0);//获取最前端CMD窗口的句柄，然后通过ShowWindow隐藏

    CameraDrive* ptrMyCameras = new CameraDrive;
    ptrMyCameras->getExternalCameraNum();
    ptrMyCameras->camerasInitInSoftTriggerMode();//相机软件触发初始化
    
    std::thread grabThread(grabByHk, ptrMyCameras);

    myTimer* communcationTimer = new myTimer;

    modbusRtuMaster* mbMaster = new modbusRtuMaster(1, "COM1", 9600, 'N', 8,  1);
    if (!mbMaster->modbusRtuMasterInit(0))//传入测试地址,测试是否正常连接
    {
        delete communcationTimer;
        delete mbMaster;
        delete ptrMyCameras;
        printf("modbus未连接，程序关闭！\n");
        return 0;
    }
    else
    {
        printf("modbus连接成功！\n");
    }
    communcationTimer->start(100, myTimerTask, mbMaster);



    


    while (DisplayConfirmSaveAsMessageBox() != IDYES);//等待确认程序关闭

    delete communcationTimer;
    cdTool.setTerminnationFlag(true);
    cdTool.conditionVariableToolNotifyAll();
    grabThread.join();

    delete mbMaster;
    delete ptrMyCameras;
    printf("程序正常关闭\n");


    return 0;
}




