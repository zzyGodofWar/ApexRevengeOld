#include "d3dOverlay.h"

bool ´°¿Ú´´½¨(int width, int height, HWND *dxhwnd, IDirect3D9 **lpD3d9Object, IDirect3DDevice9 **lpD3d9Device) {
	HWND winHwnd = 0;
	void WindowCreateThread(HWND *hwnd);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WindowCreateThread, &winHwnd, 0, 0);

	while (winHwnd == 0) {
		Sleep(1);
	}
	if (winHwnd <= 0) {
		return false;
	}

	SetWindowPos(winHwnd, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
	MARGINS margins = { -1 ,0,0,0 };
	SetLayeredWindowAttributes(winHwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	DwmExtendFrameIntoClientArea(winHwnd, &margins);
	ShowWindow(winHwnd, SW_SHOWDEFAULT);
	UpdateWindow(winHwnd);

	IDirect3D9 *d3d9Object;
	IDirect3DDevice9 *d3d9device;
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = winHwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;// D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	d3d9Object = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9Object) {
		return false;
	}

	HRESULT cdresult = d3d9Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, winHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3d9device);
	if (FAILED(cdresult)) {
		return false;
	}

	*dxhwnd = winHwnd;
	*lpD3d9Object = d3d9Object;
	*lpD3d9Device = d3d9device;

	return true;
}

void WindowCreateThread(HWND *hwnd) {
	int Random(int x, int y);
	void GetRandomString(int length, wchar_t *string);

	wchar_t classText[128] = { 0 }, windowText[128] = { 0 }, ranText[128] = { 0 };

	wcscat(classText, L"TECENT_QQ_MUSIC_");
	GetRandomString(Random(8, 12), ranText);
	wcscat(classText, ranText);

	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = DefWindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = (HMODULE)Random(1234, 999999); //GetModuleHandle(NULL);
	wndClass.hIcon = (HICON)Random(2234, 999999);//LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // (HCURSOR)Random(3234, 999999); //LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(RGB(0, 0, 0));
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = classText;
	wndClass.hIconSm = NULL;

	if (!RegisterClassEx(&wndClass)) {
		*hwnd = (HWND)-1;
		return;
	}

	//memset(classText, 0, sizeof(classText));
	memset(ranText, 0, sizeof(ranText));
	wcscat(windowText, L"Tecent QQ_");
	GetRandomString(Random(12, 16), ranText);
	wcscat(windowText, ranText);

	HWND winHwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,//WS_EX_LTRREADING | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		wndClass.lpszClassName,
		windowText,
		WS_POPUP,
		0,
		0,
		800,
		600,
		NULL,
		NULL,
		wndClass.hInstance,
		NULL
	);

	if (!winHwnd) {
		*hwnd = (HWND)-1;
		return;
	}
	*hwnd = winHwnd;

	MSG Msg;
	while (GetMessage(&Msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

void D3DÉèÖÃ´°¿Ú´©Í¸(HWND wHwnd,bool state) {
	if (state) {
		SetWindowLong(wHwnd, GWL_EXSTYLE, GetWindowLong(wHwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
	}
	else {
		SetWindowLong(wHwnd, GWL_EXSTYLE, GetWindowLong(wHwnd, GWL_EXSTYLE) & ~WS_EX_TRANSPARENT);
	}

}

void D3D»­¾ØÐÎ(IDirect3DDevice9 **lpd3d9device, LPD3DXLINE *lpd3dline, int x1, int y1, int x2, int y2, bool OutLine, int LineWidth, D3DCOLOR LineColor, bool Filled, D3DCOLOR FillColor) {
	if (Filled) {
		if (lpd3d9device != NULL) {
			D3DRECT fillRect;
			fillRect.x1 = x1;
			fillRect.y1 = y1;
			fillRect.x2 = x2;
			fillRect.y2 = y2;
			(*lpd3d9device)->Clear(1, &fillRect, D3DCLEAR_TARGET, FillColor, 0, 0);
		}
	}
	if (OutLine) {
		if (lpd3dline != NULL) {
			if (LineWidth < 1) {
				LineWidth = 1;
			}
			(*lpd3dline)->SetWidth(1);


			D3DXVECTOR2 lineVec[5];
			for (int i = 0; i < LineWidth; i++) {
				lineVec[0].x = x1 + i;
				lineVec[0].y = y1 + i;

				lineVec[1].x = x2 - i;
				lineVec[1].y = y1 + i;

				lineVec[2].x = x2 - i;
				lineVec[2].y = y2 - i;

				lineVec[3].x = x1 + i;
				lineVec[3].y = y2 - i;

				lineVec[4].x = x1 + i;
				lineVec[4].y = y1 + i;

				(*lpd3dline)->Draw(lineVec, 5, LineColor);
			}
		}

	}
}


void D3DÐ´×Ö(LPD3DXFONT *lpd3dfont, int x, int y, int fontHeight, D3DCOLOR color, char *string) {
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = rect.left + strlen(string)*fontHeight;
	rect.bottom = rect.top + fontHeight;
	(*lpd3dfont)->DrawTextA(NULL, string, -1, &rect, DT_LEFT, color);
}

void D3DÃè±ßÐ´×Ö(LPD3DXFONT *lpd3dfont, int x, int y, int fontHeight, D3DCOLOR color, D3DCOLOR backcolor, char *string) {
	int lineWidth = 0;
	int lineCount = 1;
	int tmp = 0;
	int pos = 0;
	while (*(string+pos)!='\0') {
		if (*(string + pos)=='\n') {
			lineCount++;
			tmp = 0;
		}
		else {
			tmp++;
		}
		if (tmp > lineWidth) {
			lineWidth = tmp;
		}
		pos++;
	}
	

	RECT rect;
	rect.top = y;
	rect.bottom = rect.top + fontHeight * lineCount;
	int RightPos = lineWidth *fontHeight;

	rect.left = x - 2;
	rect.right = rect.left + RightPos;
	(*lpd3dfont)->DrawTextA(NULL, string, -1, &rect, DT_LEFT, backcolor);

	rect.left = x + 2;
	rect.right = rect.left + RightPos;
	(*lpd3dfont)->DrawTextA(NULL, string, -1, &rect, DT_LEFT, backcolor);

	rect.left = x;
	rect.right = rect.left + RightPos;

	rect.top = y - 1;
	rect.bottom = rect.top + fontHeight * lineCount;
	(*lpd3dfont)->DrawTextA(NULL, string, -1, &rect, DT_LEFT, backcolor);

	rect.top = y + 1;
	rect.bottom = rect.top + fontHeight * lineCount;
	(*lpd3dfont)->DrawTextA(NULL, string, -1, &rect, DT_LEFT, backcolor);

	rect.left = x;
	rect.top = y;
	rect.right = rect.left + RightPos;
	rect.bottom = rect.top + fontHeight * lineCount;
	(*lpd3dfont)->DrawTextA(NULL, string, -1, &rect, DT_LEFT, color);
}

int Random(int x, int y) {
	srand(time(0));
	return rand() % (y + 1 - x) + x;
}

void GetRandomString(int length, wchar_t *string) {
	srand(time(0));
	for (int i = 0; i < length; i++) {
		int ulValue = rand();
		ulValue = ulValue % 100;
		if (ulValue >= 70) {
			ulValue = rand();
			ulValue = ulValue % (L'9' - L'0' + 1) + L'0';
		}
		else {

			ulValue = rand();
			ulValue = ulValue % (L'Z' - L'A' + 1) + L'A';
		}
		string[i] = (WCHAR)ulValue;
	}
	string[length] = L'\0';
}