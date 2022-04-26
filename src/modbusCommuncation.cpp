#include "modbusCommuncation.h"

bool modbusRtuMaster::modbusRtuMasterInit()
{
	int state = modbus_connect(md_rtu);
	if (state != 0)
	{
		printf("modbus can not connect! [%d]\n", state);
		return false;
	}
	modbus_set_slave(md_rtu, id);  //设置modbus从机地址 
	struct timeval t;
	t.tv_sec = 0;
	t.tv_usec = 1000000;   //设置modbus超时时间为1000毫秒 
	modbus_set_response_timeout(md_rtu, (int)&t.tv_sec, (int)&t.tv_usec);

	//回环测试连接是否成功
	modbus_write_register(md_rtu, testAddr, 1222);
	uint16_t* tab_reg = (uint16_t*)malloc(1 * sizeof(uint16_t));
	modbus_read_registers(md_rtu, testAddr, 1, tab_reg);
	if (tab_reg[0] == 1222)
	{
		free(tab_reg);
		return true;
	}
	else
	{
		free(tab_reg);
		printf("modbus slave can not connect! \n");
		return false;
	}
	
}

void modbusRtuMaster:: modbus_rtu_read(int addr2, int num)
{
	uint16_t* tab_reg;
	tab_reg = (uint16_t*)malloc(num * sizeof(uint16_t));
	int regs = modbus_read_registers(md_rtu, addr2, num, tab_reg);
	
	//printf("modbus recevie %d :", ID);
	readData.clear();
	for (int i = 0; i < num; i++)
	{
		/*std::cout << tab_reg[i] << " ";*/
		readData.emplace_back(tab_reg[i]);
	}
	free(tab_reg);
}
void modbusRtuMaster:: modbus_rtu_write(int addr2, int value)
{
	modbus_write_register(md_rtu, addr2, value);
	//std::cout << "modbus write over " << std::endl;
}
void modbusRtuMaster::modbus_rtu_write_continue(int addr2, int nb, const uint16_t* src)
{
	modbus_write_registers(md_rtu, addr2, nb, src);
	//std::cout << "modbus write over " << std::endl;
}
void modbusRtuMaster::modbus_rtu_write_with_read(int write_addr, int write_nb, const uint16_t* src, int read_addr, int read_nb)
{
	uint16_t* tab_reg;
	tab_reg = (uint16_t*)malloc(read_nb * sizeof(uint16_t));
	modbus_write_and_read_registers(md_rtu, write_addr, write_nb, src, read_addr, read_nb, tab_reg);
	readData.clear();
	for (int i = 0; i < read_nb; i++)
	{
		/*std::cout << tab_reg[i] << " ";*/
		readData.emplace_back(tab_reg[i]);
	}
	free(tab_reg);
}


/*******************下面是测试代码***********************/
//int main()
//{
//	modbus_t* mb;
//
//	mb = modbus_new_rtu("COM1", 115200, 'N', 8, 1);   //相同的端口只能同时打开一个 
//	modbus_connect(mb);
//
//	modbus_rtu_read(1, 0, 10, mb);
//
//	int i = 0;
//	while (i < 100)
//	{
//		modbus_rtu_write(1, 3, i, mb);
//		modbus_rtu_read(1, 0, 10, mb);
//		i++;
//	}
//
//	modbus_free(mb);
//	system("pause");
//	return 0;
//}