// hksion.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdio.h>
#include <iostream>
#include <fstream> 
#include <string>
#include <streambuf> 
#include "Windows.h"
#include "HCNetSDK.h"
#include <time.h>
#include <conio.h>//getch()������

#pragma comment(lib,"HCNetSDK.lib")
#pragma comment(lib,"PlayCtrl.lib")
#pragma comment(lib,"GdiPlus.lib")
#pragma comment(lib,"HCCore.lib")

using namespace std;


//��������
int iNum = 0; 	//ͼƬ�������
LONG IUserID;	//������豸
LONG IHandle = -1;//��������/����;
NET_DVR_DEVICEINFO_V30 struDeviceInfo;	//�豸��Ϣ


char sDVRIP[20]="192.168.2.103";	//ץ��������豸IP��ַ
short wDVRPort = 8000;	//�豸�˿ں�
char sUserName[20]="admin";	//��¼���û���
char sPassword[20]="wentuo2020";	//�û�����
string carNum;//���ƺ�							
string LineByLine;//���ж�ȡ�ļ� 
char logPath[20] = "D:\\sdklog\\";


				  //---------------------------------------------------------------------------------
				  //��������
void Init();//��ʼ��
void Demo_SDK_Version(); //��ȡsdk�汾
void Connect();//���������¼�������ʱ��
void Htime();//��ȡ���������豸ʱ��
bool Login(/*char *sDVRIP, short wDVRPort, char *sUserName, char *sPassword*/);//ע��������豸
void CALLBACK MSesGCallback(LONG ICommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void *pUser);//�����ص�����
void SetMessageCallBack();//���ñ����ص�����
void Whitelist();//�������ȶ�
void Blacklist();//�������ȶ�
void SetupAlarm();//��������
void CloseAlarm();//��������
void OnExit(void);//�˳�
				  //---------------------------------------------------------------------------------------------------
				  //��������
				  //��ʼ��
void Init()
{
	//��ȡϵͳʱ��
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	cout << sys.wYear << "-" << sys.wMonth << "-" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << endl;

	
	cout << "�����豸IP:";
	//cin >> sDVRIP; cout << endl;
	cout << "�����豸�û���:";
	//cin >> sUserName; cout << endl;
	cout << "�����豸����:";
	/*
	char c;
	for (int i = 0; (c = _getch()) != '\r'; i++) {
		sPassword[i] = c;
		putchar('*');
	}
	*/
	cout <<endl;
	NET_DVR_INIT_CFG_ABILITY ablitty = {};
	//NET_DVR_SetSDKInitCfg(ablitty.byRes, void*);

	BOOL is = NET_DVR_Init();//��ʼ��
	switch (is)
	{
	case NET_DVR_GETLOCALIPANDMACFAIL:
		cout << " Get The PC IP Failed" << endl;
		break;
	case NET_DVR_NOERROR:
		cout << " Init NoERROR" << endl;
		break;
	case NET_DVR_ALLOC_RESOURCE_ERROR:
		cout << " Init Error Code:" + is << endl;
		break;
	case TRUE:
		cout << " Init Succeed" << endl;
		break;
	default:
		cout << " Init Error " << NET_DVR_GetLastError() <<" " <<is << endl;
		break;
	}
	 
	Demo_SDK_Version();//��ȡ SDK  �İ汾�ź� build  ��Ϣ	
}

//���������¼�������ʱ��
void Connect()
{
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);
}
//��ȡ���������豸ʱ��
void Htime() {
	BOOL iRet;
	DWORD dwReturnLen;
	NET_DVR_TIME struParams = { 0 };

	iRet = NET_DVR_GetDVRConfig(IUserID, NET_DVR_GET_TIMECFG, 1, &struParams, sizeof(NET_DVR_TIME), &dwReturnLen);

	if (iRet!=0)
	{
		NET_DVR_SetLogToFile(3, logPath, false);
		// error 3 ��ʾSDKδ��ʼ����NET_DVR_NOINIT
		cout << "NET_DVR_GetDVRConfig NET_DVR_GET_TIMECFG  error: " << NET_DVR_GetLastError()<< "IRet:"<<iRet << endl;;
		NET_DVR_Logout(IUserID);
		NET_DVR_Cleanup();
	}
	printf("%d��%d��%d��%d:%d:%d\n", struParams.dwYear, struParams.dwMonth, struParams.dwDay, struParams.dwHour, struParams.dwMinute, struParams.dwSecond);
}

//ע��������豸
/*

*/
bool Login(/*char *sDVRIP, short wDVRPort, char *sUserName, char *sPassword*/)
{
	//����
	IUserID = NET_DVR_Login_V30(sDVRIP, wDVRPort, sUserName, sPassword, &struDeviceInfo);

	//NET_DVR_USER_LOGIN_INFO info = { 0 };
	//info.sDeviceAddress = ;
	//IUserID = NET_DVR_Login_V40(info, &struDeviceInfo);
	switch (IUserID)
	{
	default:
		break;
	}
	if (IUserID < 0)
	{
		
		std::cout << "Login Error:" << NET_DVR_GetLastError() << std::endl;
		
		NET_DVR_Cleanup();
		return false;
	}
	else if (IUserID == 0) {
		std::cout << "Login Succeed" << std::endl;
		return true;
	}
	else if (IUserID == 1) {
		cout << "User Name And Password False" << endl;
		return false;
	}
	else {
		cout << "Login Faild:" + IUserID << endl;
		return false;
	}

	

}

//Demo_SDK_Version()��������sdk�汾��ȡ����
void Demo_SDK_Version()
{
	unsigned int uiVersion = NET_DVR_GetSDKBuildVersion();

	char strTemp[1024] = { 0 };
	sprintf_s(strTemp, "HCNetSDK V%d.%d.%d.%d\n", \
		(0xff000000 & uiVersion) >> 24, \
		(0x00ff0000 & uiVersion) >> 16, \
		(0x0000ff00 & uiVersion) >> 8, \
		(0x000000ff & uiVersion));
	printf(strTemp);
}

//�����쳣��Ϣ�ص�����
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	char tempbuf[256] = { 0 };
	switch (dwType)
	{
	case EXCEPTION_RECONNECT:    //Ԥ��ʱ����  
		printf("----------reconnect--------%d\n", time(NULL));
		break;
	default:
		break;
	}
}


//�����ص�����
void CALLBACK MSesGCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser)
{

	ofstream oFile;//�����ļ������
	oFile.open("���ƺ�.csv", ofstream::app);    //��Ҫ������ļ� 	
											 //��ȡϵͳʱ��
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	cout << sys.wYear << "-" << sys.wMonth << "-" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << endl;
	int i = 0;
	char filename[100];
	FILE *fSnapPic = NULL;
	FILE *fSnapPicPlate = NULL;
	//���´�������ο���ʵ��Ӧ���в������ڸûص�������ֱ�Ӵ������ݱ����ļ���
	//�������ʹ����Ϣ�ķ�ʽ(PostMessage)����Ϣ��Ӧ��������д���
	switch (lCommand) {
		//��ͨץ�Ľ��(�ϱ�����Ϣ)
	case COMM_UPLOAD_PLATE_RESULT: {
		NET_DVR_PLATE_RESULT struPlateResult = { 0 };
		memcpy(&struPlateResult, pAlarmInfo, sizeof(struPlateResult));
		printf("���ƺ�: %s\n", struPlateResult.struPlateInfo.sLicense);//���ƺ�		
																	// oFile << struPlateResult.struPlateInfo.sLicense << endl;//���泵�ƺŵ�csv�ļ�		
		oFile << struPlateResult.struPlateInfo.sLicense << "," << sys.wYear << "-" << sys.wMonth << "-" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << endl;//���泵�ƺŵ�csv�ļ�	
																																															//����ͼ
		if (struPlateResult.dwPicLen != 0 && struPlateResult.byResultType == 1)
		{
			sprintf(filename, "./pic/%s.jpg", struPlateResult.struPlateInfo.sLicense);
			fSnapPic = fopen(filename, "wb");
			fwrite(struPlateResult.pBuffer1, struPlateResult.dwPicLen, 1, fSnapPic);
			iNum++;
			fclose(fSnapPic);
		}
		//����ͼ
		if (struPlateResult.dwPicPlateLen != 0 && struPlateResult.byResultType == 1)
		{
			sprintf(filename, "./pic/1/%s.jpg", struPlateResult.struPlateInfo.sLicense);
			fSnapPicPlate = fopen(filename, "wb");
			fwrite(struPlateResult.pBuffer1, struPlateResult.dwPicLen, 1, fSnapPicPlate);
			iNum++;
			fclose(fSnapPicPlate);
		}
		//������Ϣ����......
		break;
	}
								   //��ͨץ�Ľ��(�±�����Ϣ)
	case COMM_ITS_PLATE_RESULT: {
		NET_ITS_PLATE_RESULT struITSPlateResult = { 0 };
		memcpy(&struITSPlateResult, pAlarmInfo, sizeof(struITSPlateResult));
		for (i = 0; i < struITSPlateResult.dwPicNum; i++)
		{
			printf("���ƺ�: %s\n", struITSPlateResult.struPlateInfo.sLicense);//���ƺ�
			carNum = struITSPlateResult.struPlateInfo.sLicense;
			NET_DVR_SetLogToFile(3, logPath, false);
			oFile << carNum << "," << sys.wYear << "-" << sys.wMonth << "-" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << endl;//���泵�ƺŵ�csv�ļ�	
			if ((struITSPlateResult.struPicInfo[i].dwDataLen != 0) && (struITSPlateResult.struPicInfo[i].byType == 1) || (struITSPlateResult.struPicInfo[i].byType == 2))
			{
				//����ͼƬ
				sprintf(filename, "./pic/%s_%d.jpg", struITSPlateResult.struPlateInfo.sLicense, i);
				fSnapPic = fopen(filename, "wb");
				fwrite(struITSPlateResult.struPicInfo[i].pBuffer, struITSPlateResult.struPicInfo[i].dwDataLen, 1, fSnapPic);
				iNum++;
				fclose(fSnapPic);
			}
			//����СͼƬ
			if ((struITSPlateResult.struPicInfo[i].dwDataLen != 0) && (struITSPlateResult.struPicInfo[i].byType == 0))
			{
				sprintf(filename, "./pic/1/%s_%d.jpg", struITSPlateResult.struPlateInfo.sLicense, i);
				fSnapPicPlate = fopen(filename, "wb");
				fwrite(struITSPlateResult.struPicInfo[i].pBuffer, struITSPlateResult.struPicInfo[i].dwDataLen, 1, \
					fSnapPicPlate);
				iNum++;
				fclose(fSnapPicPlate);
			}
			//������Ϣ����......
		}
		//Whitelist();//�������ȶ�
		//Blacklist();//�������ȶ�
		break;
	}
	default: {
		std::cout << lCommand << endl;
		break;
	}
	}
	oFile.close();//�ر��ļ�
	return;
}

//���ñ����ص�����
void SetMessageCallBack()
{
	NET_DVR_SetDVRMessageCallBack_V30(MSesGCallback, NULL);
}


/*
Ҫʹ PC �ܹ��յ��豸�����������ı�������Ϣ�����뽫�豸�����������е�Զ�̱���������
ַ(struAlarmHostIpAddr)���ó� PC ���� IP ��ַ����ӿ��е� sLocalIP ����һ�£���
Զ�̱����� ���˿ں�(wAlarmHostIpPort)���ó� PC ���ļ����˿ںţ���ӿ��е� wLocalPort ����һ�£���
*/
//��������
void SetupAlarm()
{
	//��������
	NET_DVR_SETUPALARM_PARAM struSetupParam = { 0 };
	struSetupParam.dwSize = sizeof(NET_DVR_SETUPALARM_PARAM);
	struSetupParam.byAlarmInfoType = 1;//�ϴ�������Ϣ���ͣ�0-�ϱ�����Ϣ(NET_DVR_PLATE_RESULT), 1-�±�����Ϣ(NET_ITS_PLATE_RESULT)
	struSetupParam.byLevel = 1;//�������ȼ���0- һ�ȼ����ߣ���1- ���ȼ����У���2- ���ȼ����ͣ�
							   //bySupport ��λ��ʾ��ֵ��0 - �ϴ���1 - ���ϴ�;  bit0 - ��ʾ���������Ƿ��ϴ�ͼƬ;

	IHandle = NET_DVR_SetupAlarmChan_V41(IUserID, &struSetupParam);//���������ϴ�ͨ������ȡ��������Ϣ��
	if (IHandle < 0)
	{
		std::cout << "NET_DVR_SetupAlarmChan_V41 Failed! Error number��" << NET_DVR_GetLastError() << std::endl;
		NET_DVR_Logout(IUserID);
		NET_DVR_Cleanup();
		return;
	}
	std::cout << "\n" << endl;
}
//��������
void CloseAlarm()
{
	//���������ϴ�ͨ��
	if (!NET_DVR_CloseAlarmChan_V30(IHandle))//�������IHandle
	{
		std::cout << "NET_DVR_CloseAlarmChan_V30 Failed! Error number��" << NET_DVR_GetLastError() << std::endl;
		NET_DVR_Logout(IUserID);
		NET_DVR_Cleanup();
		return;
	}
	IHandle = -1;//�������;
}
//�˳�
void OnExit(void)
{
	std::cout << "Begin exit..." << std::endl;
	//��������
	CloseAlarm();
	//�ͷ����
	NET_DVR_Logout(IUserID);//ע���û�
	NET_DVR_Cleanup();//�ͷ�SDK��Դ	
}
//�������ȶ�
void Whitelist() {

	ifstream iFile;
	iFile.open("������.csv", ios::in);
	if (!iFile.is_open())
		std::cout << "�Ҳ����ļ�";
	while (getline(iFile, LineByLine))
	{
		if (LineByLine.empty()) {
			continue;
		}
		else {
			size_t found = LineByLine.find(carNum.substr(4, 8));//carNum.substr(4, 8) ��ȡ���ƺš�����NF8202��ΪNF8202
			if (found != std::string::npos) {
				std::cout << "������:" << LineByLine << '\n';
			}

		}
	}
	iFile.close();//�ر��ļ�
}

//�������ȶ�
void Blacklist() {

	ifstream iFile;
	iFile.open("������.csv", ios::in);
	if (!iFile.is_open())
		std::cout << "Not Found The File black.csv";

	while (getline(iFile, LineByLine))
	{
		if (LineByLine.empty()) {
			continue;
		}
		else {
			size_t found = LineByLine.find(carNum.substr(4, 8));//carNum.substr(4, 8) ��ȡ���ƺš�����NF8202��ΪNF8202
			if (found != std::string::npos) {
				std::cout << "������:" << LineByLine << '\n';
			}

		}
	}
	iFile.close();//�ر��ļ�
}

int main()
{
	Init();//��ʼ��sdk
	Connect();//���������¼�������ʱ��			  	
	Login(/*sDVRIP, wDVRPort, sUserName, sPassword*/);	//ע���豸
	//Htime(); //��ȡ���������豸ʱ��
	SetupAlarm();//�������˴�Ӧ����һ������
	SetMessageCallBack();	//ע�ᱨ���ص����� 

	while(1) {
		SetMessageCallBack();	//�����ص����� 				
		Sleep(500);
	}
	Sleep(-1);
	atexit(OnExit);//�˳�
	return 0;

}