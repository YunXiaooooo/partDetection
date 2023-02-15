#pragma once
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <iostream>
#include <QObject>
#include "modbus.h"
#include "myThreadPool.h"
#include <atomic>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include "infoBoard.h"
class modbusRtuMaster
{
public:
    modbusRtuMaster(const int _id, const std::string& _comName, const int _baud, const char _parity, const int _dataBit, const int _stopBit, const int _testAddr, std::shared_ptr<infoBoard> ptrInfoBoard):
        id(_id),comName(_comName),baud(_baud),parity(_parity),dataBit(_dataBit),stopBit(_stopBit),testAddr(_testAddr), ptrInfoBoardForTool(ptrInfoBoard)
    {
        if (md_rtu == nullptr)
        {
            try {
                md_rtu = modbus_new_rtu(comName.c_str(), baud, parity, dataBit, stopBit);   //相同的端口只能同时打开一个 析构函数中释放，所以写在构造函数中。使用前需要向init
            }
            catch(std::exception&e)
            {
                printf("modbus_new_rtu failure \n");
                std::exit(1);
            }
        }
    }

    modbusRtuMaster(const modbusRtuMaster&) = delete;
    modbusRtuMaster& operator=(const modbusRtuMaster&) = delete;

    ~modbusRtuMaster()
    {
        if (md_rtu != nullptr)
        {
            printf("modbusRtuMaster 析构 \n");
            modbus_free(md_rtu);
        }
    }
    bool modbusRtuMasterInit() ;
    void modbus_rtu_read(const int addr2, const int num);
    void modbus_rtu_write(const int addr2, const int value);
    void modbus_rtu_write_continue(const int addr2, const int nb, const uint16_t* src);
    void modbus_rtu_write_with_read(const int write_addr, const int write_nb, const uint16_t* src, const int read_addr, const int read_nb);
    std::vector<unsigned short> getReadData() const
    {
        return readData;
    }


private:
    modbus_t* md_rtu = nullptr;//libmodbus只支持指针，没必要每次都get()
    const int id;
    const std::string comName;
    const int baud;
    const char parity;
    const int dataBit;
    const int stopBit;
    std::vector<unsigned short> readData;
    const int testAddr;

    std::shared_ptr<infoBoard> ptrInfoBoardForTool;
};



class communicationToolProxy :public QObject
{
    Q_OBJECT
public:
    communicationToolProxy(std::shared_ptr<infoBoard> ptrInfoBoard):ptrInfoBoardForProxy(ptrInfoBoard)
    {
        if (tool == nullptr)
        {
            try {
                tool = std::make_unique<modbusRtuMaster>(2, "COM1", 9600, 'N', 8, 1, 88-1, ptrInfoBoardForProxy);
                //回环测试地址，使用与PLC约定的地址以外的地址
                //地址都减一是因为表格地址和实际地址存在一位偏差
            }
            catch (std::exception& e)
            {
                printf("New modbusRtuMaster Failure (%s) \n", e.what());
                std::exit(1);
            }
            
            if (!tool->modbusRtuMasterInit())
            {
                printf("modbusRtuMasterInit Failure \n");
                QMessageBox::critical(NULL, QStringLiteral("error"), QStringLiteral("modbusRtuMasterInit Failure"), QMessageBox::Yes);
                std::exit(1);
            }
        }
    }
    ~communicationToolProxy()
    {
    }

    communicationToolProxy(const communicationToolProxy&) = delete;
    communicationToolProxy& operator=(const communicationToolProxy&) = delete;
    void ChangeAllResultSetOne();
    int ReceiveGrabSignal();
    void sendGrabFinishedSignal(const int num);
    void setTestResults(std::vector<std::vector<int>>& data);
    void clearGrabFinishedSignal(const int num);

    void sendTestResults();
    void setTestCompeleteFlag(const bool tf)
    {
        testCompeleteFlag = tf;
    }
    void setGrabFinishedFlag(const bool tf,const int num)
    {
        grabFinishedFlags[num] = tf;
    }
    void task();

private:
    std::unique_ptr<modbusRtuMaster> tool = nullptr;
    uint16_t testResults[48] = {0};
    std::array<std::atomic_bool, 12> grabFinishedFlags{ false };
    std::array<std::atomic_bool, 12> clearFinishedFlags{ false};
    std::atomic_bool testCompeleteFlag = false;
    std::atomic_bool isTaskRun = false;
    std::atomic_bool AllResultSetOne = false;

    std::shared_ptr<infoBoard> ptrInfoBoardForProxy;

signals:
    void toGrab(int);

};



#endif