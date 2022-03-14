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
    //ShowWindow(GetForegroundWindow(), 0);//获取最前端CMD窗口的句柄，然后通过ShowWindow隐藏
    
    std::unique_ptr<CameraDrive> ptrMyCameras(new CameraDrive);//在生命周期结束时自动析构
    ptrMyCameras->getExternalCameraNum();
    ptrMyCameras->camerasInitInSoftTriggerMode();//相机软件触发初始化
    
    std::thread grabThread(grabByHk, std::move(ptrMyCameras));//ptrMyCameras指针权限移入grabThread线程，线程结束时自动析构
    std::unique_ptr<myTimer> communcationTimer(new myTimer);

    std::unique_ptr<modbusRtuMaster> mbMaster(new modbusRtuMaster(1, "COM1", 9600, 'N', 8, 1));
    if (!mbMaster->modbusRtuMasterInit(0))//传入测试地址,测试是否正常连接
    {
        printf("modbus未连接，程序即将关闭！\n");
        cdTool.setTerminnationFlag(true);
        cdTool.conditionVariableToolNotifyAll();
        grabThread.join();

        printf("程序关闭！\n");
        return 0;
    }
    else
    {
        printf("modbus连接成功！\n");
    }
    communcationTimer->start(100, myTimerTask, mbMaster.get());



    


    while (DisplayConfirmSaveAsMessageBox() != IDYES);//等待确认程序关闭

    cdTool.setTerminnationFlag(true);
    cdTool.conditionVariableToolNotifyAll();
    grabThread.join();

    printf("程序正常关闭\n");


    return 0;
}




