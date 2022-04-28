#pragma once
#include "server_shared.h"
#include "driver.h"
#include "constList.h"
#include "other_func.h"
#include "d3dOverlay.h"
#include "driverLoad.h"
#include "handleTCP.h"
#include "LoginUI.h"
#include "offsets.h"
#include "game_math.h"
#include "resource.h"

#include "VMP\VMProtectSDK.h"


#include <Windows.h>
#include<windowsx.h>
#include <TlHelp32.h>
#include <time.h>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
#include <iostream>
using namespace std;

extern Matrix ViewMartix;
extern int g_width, g_height;
extern DWORD32 g_pid;
extern HWND g_hwnd;
extern DWORD64 g_add_gamebase , g_add_entitylist, g_add_entitylocal, g_add_nameList;
extern SOCKET g_sock_1, g_sock_2;

extern HWND ov_hwnd;
extern IDirect3D9 *d3dObject;
extern IDirect3DDevice9 *d3dDevice;
extern LPD3DXFONT dx_font_verda;
extern LPD3DXFONT dx_font_soti;

extern HWND hwnd_console;

extern char valid_code[48];
extern char processdata[48];
extern int processdatalen;

extern void CleanAndExit();

extern int code_remaintime;
extern int code_type;

extern bool spoofmachine;

extern bool needupdate;