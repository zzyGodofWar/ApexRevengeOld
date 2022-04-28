#pragma once
#include "handleTCP.h"

SOCKET valid_server = 0;

bool ConnectToServer() {
	VMProtectBegin("valid_server");
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	valid_server = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	sockaddr_in siServer;
	siServer.sin_family = AF_INET;
	siServer.sin_port = htons(58888);
	//siServer.sin_addr.S_un.S_addr = inet_addr("119.29.82.79");
	siServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int nRet = connect(valid_server, (sockaddr*)&siServer, sizeof(siServer));
	if (nRet == SOCKET_ERROR)
	{
		printf("连接服务器失败，服务器当前可能正在维护\n");
		getchar();
		return false;
	}
	printf("连接服务器成功\n");
	char szBuff[] = "[99]1014";
	send(valid_server, szBuff, sizeof(szBuff), 0);
	return true;

	VMProtectEnd();
}

void TCPHeartBeat() {
	DWORD lastTime = GetTickCount();
	char heartBeatData[] = { 'Z','Z','Y' };
	while (true) {
		if (GetTickCount() - lastTime >= 10000) {
			send(valid_server, heartBeatData, sizeof(heartBeatData), 0);
			lastTime = GetTickCount();
		}
		Sleep(1000);
	}
}

void ReceivePacket() {
	VMProtectBegin("tcppacket");
	int nRet;
	char packetBuffer[MAXWORD] = { 0 };

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)TCPHeartBeat, 0, 0, 0);
	
	char control_code[5] = { 0 };

	while (true) {

		nRet = recv(valid_server, packetBuffer, sizeof(packetBuffer), 0);
		if (nRet == 0 || nRet == SOCKET_ERROR)
		{
			CleanAndExit();
		}
		if (nRet >= 4) {
			memmove(control_code, packetBuffer, 4);
		}
		if (!strcmp("[99]", control_code)) {
			needupdate = true;
			printf("版本需要更新，按任意键打开下载网站\n");
			printf("https://ww.lanzous.com/b08xkoy9e\n");
			printf("访问密码：apex\n");
			system("pause");
			ShellExecuteA(NULL, "open", "https://ww.lanzous.com/b08xkoy9e", NULL, NULL, SW_SHOWNORMAL);
			CleanAndExit();
		}
		else if (!strcmp("[00]", control_code)) {
			if (nRet <= 5) {
				LoginUI_alertAbort();
				LoginUI_alert("卡密不正确！");
				continue;
			}

			memmove(&code_remaintime, packetBuffer + 4, 4);
			code_type = (int)(packetBuffer[9]-'0');
			if (!code_remaintime) {
				LoginUI_alertAbort();
				LoginUI_alert("卡密已到期！");
				continue;
			}
			if (code_type==0) {
				LoginUI_alertAbort();
				LoginUI_alert("卡密不正确！");
				continue;
			}
			switch (code_type) {
			case 1:
				RC4((char*)&code_remaintime, "DAY",4);
				break;
			case 2:
				RC4((char*)&code_remaintime, "WEEK", 4);
				break;
			case 3:
				RC4((char*)&code_remaintime, "MONTH", 4);
				break;
			}
			
			memset(valid_code, 0, sizeof(valid_code));
			memmove(valid_code, content_codebox, len_codebox);

			if (remembercode) {
				WritePrivateProfileStringA("Launcher", "CODE", valid_code, ".\\apexRevenge.ini");
			}
			else {
				WritePrivateProfileStringA("Launcher", "CODE", "", ".\\apexRevenge.ini");
			}

			memmove(processdata, packetBuffer + 11, nRet - 10);
			processdatalen = nRet - 11;
			login_quit = true;
			SendMessage(hwnd_loginui,WM_CLOSE,0,0);

		}
		else if (!strcmp("[02]", control_code)) {
			
			CleanAndExit();
		}
	}

	VMProtectEnd();
}