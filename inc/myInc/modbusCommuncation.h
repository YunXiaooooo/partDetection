#ifndef _MODBUSCOMMUNCATION_H_
#define _MODBUSCOMMUNCATION_H_

#include <iostream>
#include <vector>
#include "modbus.h"

class modbusRtuMaster
{
public:
    modbusRtuMaster(int _id, std::string _comName, int _baud, char _parity, int _dataBit, int _stopBit):id(_id), comName(_comName), baud(_baud),
        parity(_parity), dataBit(_dataBit), stopBit(_stopBit)
    {
       
    }

    modbusRtuMaster(const modbusRtuMaster& md_rtu) = delete;

    ~modbusRtuMaster()
    {
        modbus_free(md_rtu);
    }
    bool modbusRtuMasterInit(int addr2);
    void modbus_rtu_read(int addr2, int num);
    void modbus_rtu_write(int addr2, int value);
    void modbus_rtu_write_continue(int addr2, int nb, const uint16_t* src);
    void modbus_rtu_write_with_read(int write_addr, int write_nb, const uint16_t* src, int read_addr, int read_nb);
    std::vector<unsigned short> getReadData() const
    {
        return readData;
    }


private:
    modbus_t* md_rtu;
    int id;
    std::string comName;
    int baud;
    char parity;
    int dataBit;
    int stopBit;
    std::vector<unsigned short> readData;

};

#endif //
