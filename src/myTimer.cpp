#include "myTimer.h"
#include "modbus.h"
#include "modbusCommuncation.h"
#include "common.h"
#include <time.h>

void  myTimerTask(modbusRtuMaster* mbMaster)
{

	//����ִ��modbusͨ�ż����ձ�־λ��λ

	//static int timeCounter = 0;
	const int readNum = 1;
	std::vector<unsigned short> data;

	//timeCounter++;
	//if (timeCounter == 1000 / 100)//1000ָ1000ms��100ָ��ʱ���趨100ms��ע�⵱ǰ�趨����Ƿ��޸�
	//{
	//	timeCounter = 0;

	//	static uint16_t heartbeat = 1;
	//	heartbeat = (heartbeat == 1) ? 0 : 1;
	//	//mbMaster->modbus_rtu_write(1, heartbeat); //��ͣ��ת��ȷ����λ������
	//	mbMaster->modbus_rtu_write_with_read(1, 1, &heartbeat, 0, readNum);
	//}
	//else
	//{
	//	mbMaster->modbus_rtu_read(0, readNum);
	//}

	mbMaster->modbus_rtu_read(0, readNum);
	data = mbMaster->getReadData();
	if (data.size() == readNum)
	{
		static unsigned short preGrab = 0;//λ��2����������ȷ���Ƿ�����
		if (preGrab == 0 && data[0] == 1)
		{
			//ȷ��ץ���ź�
			cdTool.setGrabFlag(true);
			cdTool.conditionVariableToolNotifyOne();
			printf("׼��ץ��ͼƬ \n");
		}
		preGrab = data[0];
	}
	else
	{
		printf("The num of read data error \n");
	}

	if (cdTool.readGrabFinishedFlag() == true)
	{
		mbMaster->modbus_rtu_write(1, 1);//���ץ�Ľ��������ź�
		cdTool.setgrabFinishedFlag(false);
	}

	if (palletRecognitionStates.readFlag() == true)//׼������
	{
		//for (int i = 0; i < 4; i++)
		//{
		//	mbMaster->modbus_rtu_write(6+i, resultCode[i].readCodeValue());
		//	resultCode[i].setCodeValue(0);
		//}
		uint16_t src[48];
		std::vector<int> buff;
		for (int i = 0; i < 4; i++)
		{
			buff = palletRecognitionStates.readStatesAboutOneGrabPic(i);
			for (int j = 0; j < 12; j++)
			{
				src[i*12+j] = buff[j];
			}
			buff.clear();
			palletRecognitionStates.setStatesAboutOneGrabPic(i, buff);
		}
		clock_t start = clock();

		mbMaster->modbus_rtu_write_continue(2, 48, src);
		palletRecognitionStates.setFlag(false);

		clock_t ends = clock();
		printf("Running Time = %f \n", ((double)(ends)-start) / CLOCKS_PER_SEC);
	}


	//clock_t ends = clock();
	//printf("Running Time = %f \n", ((double)(ends)-start) / CLOCKS_PER_SEC);

}