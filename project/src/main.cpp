#include "view.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include "et199_64.h"


void VerifyEncryption(void);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	#ifndef TEST
	VerifyEncryption();
	#endif // !TEST

    view w;
    w.show();
    return a.exec();
}

long long hex2int(const std::string& hexStr)
{
	char* offset;
	if (hexStr.length() > 2)
	{
		if (hexStr[0] == '0' && hexStr[1] == 'x')
		{
			return strtoll(hexStr.c_str(), &offset, 0);
		}
	}
	return strtoll(hexStr.c_str(), &offset, 16);
}

void VerifyEncryption(void)
{
	DWORD dwRet;
	DWORD dwCount = 0;	// 锁枚举
	ET_CONTEXT* pContext;
	std::string CustomerNum = "0xD25E0F9D";


	// 枚举ET199并返回数量
	dwRet = ETEnum(NULL, &dwCount);
	if (dwRet != ET_E_INSUFFICIENT_BUFFER && dwRet) {
		// QMessageBox::critical(NULL, "错误", "找不到指定的加密锁！[ERROR CODE: 1]", QMessageBox::Yes);
		QMessageBox::critical(NULL, QStringLiteral("error"), QStringLiteral("The specified encryption lock could not be found"), QMessageBox::Yes);
		exit(1);
	}
	//根据数量分配内存用于存储CONTEXT结构数组
	pContext = new ET_CONTEXT[dwCount];
	//清0缓冲区
	memset(pContext, 0, sizeof(ET_CONTEXT) * dwCount);
	//使用分配的缓冲区重新进行枚举
	dwRet = ETEnum(pContext, &dwCount);
	if (dwRet) {
		// QMessageBox::critical(NULL, "错误", "找不到指定的加密锁！[ERROR CODE: 2]", QMessageBox::Yes);
		QMessageBox::critical(NULL, QStringLiteral("error"), QStringLiteral("The specified encryption lock could not be found"), QMessageBox::Yes);
		delete[] pContext;
		exit(1);
	}
	printf("Find ET199 %d\n", dwCount);
	if (dwCount < 1) {
		// QMessageBox::critical(NULL, "错误", "找不到指定的加密锁！[ERROR CODE: 3]", QMessageBox::Yes);
		QMessageBox::critical(NULL, QStringLiteral("error"), QStringLiteral("The specified encryption lock could not be found"), QMessageBox::Yes);
		delete[] pContext;
		exit(1);
	}
	//打开ET199
	dwRet = ETOpen(&pContext[0]);
	if (dwRet) {
		QMessageBox::critical(NULL, QStringLiteral("error"), QStringLiteral("The encryption lock is abnorma"), QMessageBox::Yes);
		ETClose(&pContext[0]);
		delete[] pContext;
		exit(1);
	}
	//printf("Open ET199: \n");
	//printf("COS Version: %d.%02d\n", pContext[0].dwVersion >> 8 & 0xff, pContext[0].dwVersion & 0xff);
	//printf("HardSerial: ");
	//for (int i = 0; i<8; i++) printf("%02X", pContext[0].bID[i]);
	//printf("\n");

	//printf("ATR:%s\n", pContext[0].bAtr);
	//printf("Customer:%X\n", pContext[0].dwCustomer);
	//printf("Customer:%x\n", hex2int(m_map["CustomerNum"]));
	if (pContext[0].dwCustomer != static_cast<int>(hex2int(CustomerNum))) // 这是根据密码生成的客户号
	{
		QMessageBox::critical(NULL, QStringLiteral("error"), QStringLiteral("Encryption lock authentication failed"), QMessageBox::Yes);
		ETClose(&pContext[0]);
		delete[] pContext;
		exit(1);
	}
	ETClose(&pContext[0]);
	delete[] pContext;
}