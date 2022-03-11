#include "myTimer.h"
#include "modbus.h"
#include "modbusCommuncation.h"
#include "common.h"
#include <time.h>

void  myTimerTask(modbusRtuMaster* mbMaster)
{

	//����ִ��modbusͨ�ż����ձ�־λ��λ

	static int timeCounter = 0;
	const int readNum = 4;
	std::vector<unsigned short> data;

	timeCounter++;
	if (timeCounter == 1000 / 100)//1000ָ1000ms��100ָ��ʱ���趨100ms��ע�⵱ǰ�趨����Ƿ��޸�
	{
		timeCounter = 0;

		static uint16_t heartbeat = 1;
		heartbeat = (heartbeat == 1) ? 0 : 1;
		//mbMaster->modbus_rtu_write(1, heartbeat); //��ͣ��ת��ȷ����λ������
		mbMaster->modbus_rtu_write_with_read(1, 1, &heartbeat, 0, readNum);
	}
	else
	{
		mbMaster->modbus_rtu_read(0, readNum);

	}

	data = mbMaster->getReadData();
	if (data.size() == readNum)
	{
		static unsigned short preGrab = 0;//λ��2����������ȷ���Ƿ�����
		if (preGrab == 0 && data[2] == 1)
		{
			//ȷ��ץ���ź�
			cdTool.setGrabFlag(true);
			cdTool.conditionVariableToolNotifyOne();
			printf("׼��ץ��ͼƬ \n");
		}
		preGrab = data[2];
	}
	else
	{
		printf("The num of read data error \n");
	}

	if (testResults.readFlag() == true)
	{
		//for (int i = 0; i < 4; i++)
		//{
		//	mbMaster->modbus_rtu_write(6+i, resultCode[i].readCodeValue());
		//	resultCode[i].setCodeValue(0);
		//}
		uint16_t src[4];
		for (int i = 0; i < 4; i++)
		{
			src[i] = testResults.readCodeValue(i);
			testResults.setCodeValue(i,0);
		}
		clock_t start = clock();

		mbMaster->modbus_rtu_write_continue(6, 4, src);
		testResults.setFlag(false);

		clock_t ends = clock();
		printf("Running Time = %f \n", ((double)(ends)-start) / CLOCKS_PER_SEC);
	}


	//clock_t ends = clock();
	//printf("Running Time = %f \n", ((double)(ends)-start) / CLOCKS_PER_SEC);

}