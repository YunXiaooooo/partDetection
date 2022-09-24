#include "communication.h"

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
	modbus_set_response_timeout(md_rtu, t.tv_sec, t.tv_usec);

	//回环测试连接是否成功
	modbus_write_register(md_rtu, testAddr, 1222);
	uint16_t* tab_reg = (uint16_t*)malloc(1 * sizeof(uint16_t));
	modbus_read_registers(md_rtu, testAddr, 1, tab_reg);
	modbus_write_register(md_rtu, testAddr, 0);//恢复默认

	if (tab_reg == nullptr)
	{
		printf("modbus slave [tenet] test failure (malloc failure) \n");
		return false;
	}
	if (tab_reg[0] == 1222)
	{
		free(tab_reg);
		return true;
	}
	else
	{
		free(tab_reg);
		printf("modbus slave [tenet] test failure (Data not equal) \n");
		return false;
	}

}

void modbusRtuMaster::modbus_rtu_read(const int addr2, const int num)
{
	assert(num > 0);
	uint16_t* tab_reg;
	tab_reg = (uint16_t*)malloc(num * sizeof(uint16_t));
	if (tab_reg == nullptr)
	{
		printf("modbus_rtu_read failure (malloc failure) \n");
		return ;
	}
	int regs = modbus_read_registers(md_rtu, addr2, num, tab_reg);
	if (regs != num)
	{
		printf("modbus_read_registers failure (libmodbus failure) \n");
		free(tab_reg);

		modbus_rtu_write(testAddr, 1222);
		modbus_rtu_write(testAddr, 0);
		////重连
		//modbus_free(md_rtu);
		//try {
		//	md_rtu = modbus_new_rtu(comName.c_str(), baud, parity, dataBit, stopBit);   //相同的端口只能同时打开一个 析构函数中释放，所以写在构造函数中。使用前需要向init
		//}
		//catch (std::exception& e)
		//{
		//	printf("modbus_new_rtu failure \n");
		//	std::exit(0);
		//}
		//modbusRtuMasterInit();
		return;
	}
	//printf("modbus recevie %d :", ID);
	readData.clear();
	for (int i = 0; i < num; i++)
	{
		/*std::cout << tab_reg[i] << " ";*/
		readData.emplace_back(tab_reg[i]);
	}
	free(tab_reg);
}
void modbusRtuMaster::modbus_rtu_write(const int addr2, const int value)
{
	int num = modbus_write_register(md_rtu, addr2, value);
	if (num != 1)
	{
		printf("modbus_write_register failure (libmodbus failure) \n");
	}
	//std::cout << "modbus write over " << std::endl;
}
void modbusRtuMaster::modbus_rtu_write_continue(const int addr2, const int nb, const uint16_t* src)
{
	int num = modbus_write_registers(md_rtu, addr2, nb, src);
	if (num != nb)
	{
		printf("modbus_write_registers failure (libmodbus failure) \n");
	}
	//std::cout << "modbus write over " << std::endl;
}
void modbusRtuMaster::modbus_rtu_write_with_read(const int write_addr, const int write_nb, const uint16_t* src, const int read_addr, const int read_nb)
{
	assert(read_nb > 0);
	uint16_t* tab_reg;
	tab_reg = (uint16_t*)malloc(read_nb * sizeof(uint16_t));
	if (tab_reg == nullptr)
	{
		printf("modbus_rtu_read failure (malloc failure) \n");
		return;
	}
	int num = modbus_write_and_read_registers(md_rtu, write_addr, write_nb, src, read_addr, read_nb, tab_reg);
	if (num != read_nb)
	{
		printf("modbus_write_and_read_registers failure (libmodbus failure) \n");
	}
	readData.clear();
	for (int i = 0; i < read_nb; i++)
	{
		/*std::cout << tab_reg[i] << " ";*/
		readData.emplace_back(tab_reg[i]);
	}
	free(tab_reg);
}

int communicationToolProxy::ReceiveGrabSignal()
{
	const int readNum = 12;
	std::vector<unsigned short> data;
	static std::vector<unsigned short> preState(readNum,0);
	tool->modbus_rtu_read(53-1, readNum);
	data = tool->getReadData();
	
	assert(data.size() == readNum);

	//printf("modbus read data:[%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d]\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6],
		//data[7], data[8], data[9], data[10], data[11]);
	int res = -1;
	for (int i = 0; i < readNum; ++i)
	{
		if (res == -1 && data[i] == 1 && preState[i] == 0)
		{
			res = i;
		}
		if (data[i] == 0 && preState[i] == 1)
		{
			clearFinishedFlags[i] = true;
		}
		preState[i] = data[i];
	}
	return res;
}

void communicationToolProxy::sendGrabFinishedSignal(const int num)
{
	tool->modbus_rtu_write(65-1+num, 1);//第num次相机抓拍结束发送信号
	//switch (num)
	//{
	//case 0:
	//	tool->modbus_rtu_write(54-1, 1);//相机抓拍结束发送信号
	//	break;
	//case 1:
	//	tool->modbus_rtu_write(56-1, 1);//相机抓拍结束发送信号
	//	break;
	//case 2:
	//	tool->modbus_rtu_write(58-1, 1);//相机抓拍结束发送信号
	//	break;
	//case 3:
	//	tool->modbus_rtu_write(60-1, 1);//相机抓拍结束发送信号
	//	break;
	//default:
	//	break;
	//}	
}

void communicationToolProxy::clearGrabFinishedSignal(const int num)
{
	tool->modbus_rtu_write(65 - 1 + num, 0);//第num次相机抓拍结束清除信号

	//switch (num)
	//{
	//case 0:
	//	tool->modbus_rtu_write(54 - 1, 0);//相机抓拍结束发送信号
	//	break;
	//case 1:
	//	tool->modbus_rtu_write(56 - 1, 0);//相机抓拍结束发送信号
	//	break;
	//case 2:
	//	tool->modbus_rtu_write(58 - 1, 0);//相机抓拍结束发送信号
	//	break;
	//case 3:
	//	tool->modbus_rtu_write(60 - 1, 0);//相机抓拍结束发送信号
	//	break;
	//default:
	//	break;
	//}
}

void communicationToolProxy::setTestResults(std::vector<std::vector<int>>& data)
{
	assert(data.size() == 12);
	if (AllResultSetOne)
	{
		for (int i = 0; i < 12; ++i)
		{
			assert(data[i].size() == 4);
			for (int j = 0; j < 4; ++j)
			{
				testResults[i * 4 + j] = 1;
			}
		}
	}
	else
	{
		//for (int i = 0; i < 12; ++i)
		//{
		//	assert(data[i].size() == 4);
		//	for (int j = 0; j < 4; ++j)
		//	{
		//		testResults[i * 4 + j] = static_cast<uint16_t>(data[i][j]);
		//	}
		//}
		for (int i = 0; i < 48; ++i)
		{
			int r = i / 6;
			int c = i % 6;
			testResults[i] = static_cast<uint16_t>(data[(r / 2) * 3 + c / 2][(r % 2) * 2 + c % 2]);
		}
	}
}
void communicationToolProxy::sendTestResults()
{
	tool->modbus_rtu_write_continue(3-1, 48, testResults);
}

void communicationToolProxy::task()
{
	if (isTaskRun)//重入时直接退出
	{
		return;
	}
	isTaskRun = true;
	
	int grabNum = ReceiveGrabSignal();
	if (grabNum != -1)
	{
		printf("reeive grab signal, grabNum = %d \n", grabNum);
		emit toGrab(grabNum);
	}
	
	for (int i = 0; i < 12; ++i)
	{
		if (grabFinishedFlags[i])
		{
			if (i != 11)//最后一次抓拍完成信号暂时不给，在检测结果返回以后再给
			{
				sendGrabFinishedSignal(i);
				printf("sendGrabFinishedSignal, i = %d \n", i);
			}
			grabFinishedFlags[i] = false;
		}
		if (clearFinishedFlags[i])
		{
			clearGrabFinishedSignal(i);
			printf("clearGrabFinishedSignal, i = %d \n", i);
			clearFinishedFlags[i] = false;
		}
	}

	if (testCompeleteFlag)//
	{
		sendTestResults();
		sendGrabFinishedSignal(11);
		testCompeleteFlag = false;
	}
	isTaskRun = false;
}

void communicationToolProxy::ChangeAllResultSetOne()
{
	AllResultSetOne = !AllResultSetOne;
}