#pragma once
#include "LoginUI.h"

HWND hwnd_loginui;
IDirect3D9 *d3dObject_loginui;
IDirect3DDevice9 *d3dDevice_loginui;
LPD3DXFONT dx_loginui_font;

MouseCallback mouseCallBack;

char content_codebox[46] = "";
int len_codebox = strlen(content_codebox);

bool eventblock = false;

char alert_text[128] = { 0 };
bool have_alert = false;
int alert_begintime = 0;

bool remembercode;
void ValidCode();
bool login_quit = false;

void LoadLoginUI() {
	if (!´°¿Ú´´½¨(800,450,&hwnd_loginui,&d3dObject_loginui,&d3dDevice_loginui)) {
		std::cout << "Æô¶¯Ê§°Ü£º½çÃæ¼ÓÔØÊ§°Ü\n";
		getchar();
		CleanAndExit();
		return;
	}
	D3DÉèÖÃ´°¿Ú´©Í¸(hwnd_loginui, false);

	SetForegroundWindow(hwnd_loginui);

	LPDIRECT3DTEXTURE9 bg_image;
	LPD3DXSPRITE dx_sprite;
	LPD3DXLINE dx_line;
	//IDR_PNG1    RCDATA DISCARDABLE    "texture.png"
	D3DXCreateTextureFromResourceA(d3dDevice_loginui, NULL, MAKEINTRESOURCEA(IDB_PNG1),&bg_image);
	D3DXCreateSprite(d3dDevice_loginui, &dx_sprite);

	D3DXCreateFontA(d3dDevice_loginui, 16, 0, FW_BOLD, 0, 0, 5, 0, 4, 0, "Verdana", &dx_loginui_font);
	D3DXCreateLine(d3dDevice_loginui, &dx_line);


	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(hwnd_loginui, (nScreenWidth - 800) / 2, (nScreenHeight - 450) / 2, 800, 450, true);

	bool firstIn = true;
	HWND ForcusHWND;
	wchar_t randtitle[12];

	LRESULT WINAPI LoginUIProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	SetWindowLongPtr(hwnd_loginui, GWLP_WNDPROC, (LONG_PTR)LoginUIProc);

	//Ìí¼ÓÊó±ê¼àÌý

	int posx_minsize = 680;
	int posx_close= 740;
	char text_minsize[] = "[Ð¡»¯]";
	char text_close[] = "[½áÊø]";
	char text_login[] = "µÇÂ¼";
	char text_paste[] = "Õ³Ìù";
	char text_empty[] = "Çå¿Õ";
	char text_blackscreen[] = "½â¾öºÚÆÁ";

	void callback_minsize(MouseEvent mevent);
	mouseCallBack.AddCallBack(1, posx_minsize, 1, posx_minsize + strlen(text_minsize) * 7, 18, &callback_minsize);

	void callback_close(MouseEvent mevent);
	mouseCallBack.AddCallBack(2, posx_close, 1, posx_close + strlen(text_close)*7, 18, &callback_close);

	RECT area_codebox = { 50 , 60 , 500 ,90 };
	RECT area_btn_login = { area_codebox.right + 5 , area_codebox.top , area_btn_login.left + 50 ,area_btn_login.top + 30 };
	RECT area_btn_paste = { area_btn_login.right + 5 , area_btn_login.top , area_btn_paste.left + 50 ,area_btn_paste.top + 30 };
	RECT area_btn_empty = { area_btn_paste.right + 5 , area_btn_paste.top , area_btn_empty.left + 50 ,area_btn_empty.top + 30 };
	RECT area_btn_blackscreen = { area_codebox.left + 5 , area_codebox.bottom + 5 , area_btn_blackscreen.left + 90 ,area_btn_blackscreen.top + 30 };

	RECT area_btn_remembercode = { area_btn_blackscreen.right + 50 , area_btn_blackscreen.top , area_btn_remembercode.left + 120 ,area_btn_remembercode.top + 30 };
	RECT area_btn_machinespoofer = { area_btn_remembercode.right + 5 , area_btn_blackscreen.top, area_btn_machinespoofer.left + 120 ,area_btn_machinespoofer.top + 30 };

	void callback_login(MouseEvent mevent);
	mouseCallBack.AddCallBack(3, &area_btn_login, &callback_login);

	void callback_paste(MouseEvent mevent);
	mouseCallBack.AddCallBack(4, &area_btn_paste, &callback_paste);

	void callback_empty(MouseEvent mevent);
	mouseCallBack.AddCallBack(5, &area_btn_empty, &callback_empty);

	void callback_blackscreen(MouseEvent mevent);
	mouseCallBack.AddCallBack(6, &area_btn_blackscreen, &callback_blackscreen);

	void callback_remembercode(MouseEvent mevent);
	mouseCallBack.AddCallBack(6, &area_btn_remembercode, &callback_remembercode);

	void callback_spoofmachine(MouseEvent mevent);
	mouseCallBack.AddCallBack(6, &area_btn_machinespoofer, &callback_spoofmachine);
	
	while (true) {
		Sleep(1);
		if (login_quit) {
			break;
		}

		ForcusHWND = GetForegroundWindow();
		if (ForcusHWND == hwnd_console) {
			if (firstIn) {
				SetWindowPos(hwnd_loginui, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				firstIn = false;
				SetWindowPos(hwnd_loginui, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			}
		}
		else if (ForcusHWND != hwnd_loginui) {
			firstIn = true;
			//continue;
		}

		if (IsIconic(hwnd_console)) {
			ShowWindow(hwnd_loginui, SW_HIDE);
			continue;
		}
		else {
			ShowWindow(hwnd_loginui, SW_NORMAL);
		}

		GetRandomString(10,randtitle);
		SetConsoleTitle(randtitle);

		d3dDevice_loginui->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, NULL);
		d3dDevice_loginui->BeginScene();

		dx_sprite->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXVECTOR3 drawpos = { 0 ,0 ,0 };
		D3DXMATRIX S;

		D3DXMatrixScaling(&S, 0.8f, 0.9f, 1.0f);

		dx_sprite->SetTransform(&S);

		dx_sprite->Draw(bg_image, NULL, NULL, &drawpos, D3DCOLOR_ARGB(255, 255, 255, 255));

		dx_sprite->End();


		D3DÃè±ßÐ´×Ö(&dx_loginui_font, 8, 3, 16, d3dcolor_White, d3dcolor_Black, "APEX Legends Revenge");


		//×îÐ¡»¯
		D3DÃè±ßÐ´×Ö(&dx_loginui_font, posx_minsize, 5, 16, d3dcolor_White, d3dcolor_Black, text_minsize);

		//¹Ø±Õ°´Å¥
		D3DÃè±ßÐ´×Ö(&dx_loginui_font, posx_close, 5, 16, d3dcolor_White, d3dcolor_Black, text_close);

		//¿¨ÃÜÊäÈë
		D3DÃè±ßÐ´×Ö(&dx_loginui_font, area_codebox.left, area_codebox.top - 20, 16, d3dcolor_White, d3dcolor_Black, "ÇëÊäÈë¿¨ÃÜ:");
		D3D»­¾ØÐÎ(&d3dDevice_loginui, &dx_line, area_codebox.left, area_codebox.top, area_codebox.right, area_codebox.bottom, true, 1, d3dcolor_Black, true, d3dcolor_White);

		D3DÐ´×Ö(&dx_loginui_font, area_codebox.left + 5 , area_codebox.top + 7, 16, d3dcolor_Black, content_codebox);


		//µÇÂ¼
		D3D»­¾ØÐÎ(&d3dDevice_loginui, &dx_line, area_btn_login.left, area_btn_login.top, area_btn_login.right, area_btn_login.bottom, true, 1, d3dcolor_Black, true, d3dcolor_ammo_energy1);
		D3DÐ´×Ö(&dx_loginui_font, area_btn_login.left + 10, area_btn_login.top + 7, 16, d3dcolor_Black, text_login);

		//Õ³Ìù
		D3D»­¾ØÐÎ(&d3dDevice_loginui, &dx_line, area_btn_paste.left, area_btn_paste.top, area_btn_paste.right, area_btn_paste.bottom, true, 1, d3dcolor_Black, true, d3dcolor_ammo_heavy1);
		D3DÐ´×Ö(&dx_loginui_font, area_btn_paste.left + 10, area_btn_paste.top + 7, 16, d3dcolor_Black, text_paste);

		//Çå¿Õ
		D3D»­¾ØÐÎ(&d3dDevice_loginui, &dx_line, area_btn_empty.left, area_btn_empty.top, area_btn_empty.right, area_btn_empty.bottom, true, 1, d3dcolor_Black, true, d3dcolor_ammo_light1);
		D3DÐ´×Ö(&dx_loginui_font, area_btn_empty.left + 10, area_btn_empty.top + 7, 16, d3dcolor_Black, text_empty);

		//½â¾öºÚÆÁ
		D3D»­¾ØÐÎ(&d3dDevice_loginui, &dx_line, area_btn_blackscreen.left, area_btn_blackscreen.top, area_btn_blackscreen.right, area_btn_blackscreen.bottom, true, 1, d3dcolor_Black, true, d3dcolor_ammo_shotgun1);
		D3DÐ´×Ö(&dx_loginui_font, area_btn_blackscreen.left + 10, area_btn_blackscreen.top + 7, 16, d3dcolor_Black, text_blackscreen);

		//¼Ç×¡¿¨ÃÜ
		D3D»­¾ØÐÎ(&d3dDevice_loginui, &dx_line, area_btn_remembercode.left, area_btn_remembercode.top, area_btn_remembercode.right, area_btn_remembercode.bottom, true, 1, d3dcolor_Black, true, d3dcolor_ammo_sniper1);
		if (remembercode) {
			D3DÐ´×Ö(&dx_loginui_font, area_btn_remembercode.left + 10, area_btn_remembercode.top + 7, 16, d3dcolor_Black, " ¡ô ¼Ç×¡¿¨ÃÜ");
		}
		else {
			D3DÐ´×Ö(&dx_loginui_font, area_btn_remembercode.left + 10, area_btn_remembercode.top + 7, 16, d3dcolor_Black, " ¡ó ¼Ç×¡¿¨ÃÜ");
		}
		
		//¹ý»úÆ÷Âë
		D3D»­¾ØÐÎ(&d3dDevice_loginui, &dx_line, area_btn_machinespoofer.left, area_btn_machinespoofer.top, area_btn_machinespoofer.right, area_btn_machinespoofer.bottom, true, 1, d3dcolor_Black, true, d3dcolor_ammo_sniper1);
		if (spoofmachine) {
			D3DÐ´×Ö(&dx_loginui_font, area_btn_machinespoofer.left + 10, area_btn_machinespoofer.top + 7, 16, d3dcolor_Black, " ¡ô ¹ý»úÆ÷Âë");
		}
		else {
			D3DÐ´×Ö(&dx_loginui_font, area_btn_machinespoofer.left + 10, area_btn_machinespoofer.top + 7, 16, d3dcolor_Black, " ¡ó ¹ý»úÆ÷Âë");
		}

		if (have_alert) {
			if (!alert_begintime) {
				alert_begintime = GetTickCount();
			}

			D3D»­¾ØÐÎ(&d3dDevice_loginui, &dx_line, 200, 110, 600, 330, true, 1, d3dcolor_Black, true, d3dcolor_ammo_sniper1);
			D3DÃè±ßÐ´×Ö(&dx_loginui_font,220,130,16,d3dcolor_Red, d3dcolor_White,alert_text);

			if (GetTickCount() - alert_begintime >= 3000) {
				memset(alert_text, 0, 128);
				have_alert = false;
				eventblock = false;
				alert_begintime = 0;
			}
		}

		d3dDevice_loginui->EndScene();
		d3dDevice_loginui->Present(NULL, NULL, NULL, NULL);
		
	}

	dx_loginui_font->Release();
	bg_image->Release();
	dx_line->Release();
	dx_sprite->Release();
	d3dObject_loginui->Release();
	d3dDevice_loginui->Release();
	
	return;
}

void callback_remembercode(MouseEvent mevent) {
	remembercode = !remembercode;
}

void callback_spoofmachine(MouseEvent mevent) {
	spoofmachine = !spoofmachine;
}

void handleClipboard() {
	OpenClipboard(NULL);
	HGLOBAL hGlobal = NULL;
	hGlobal = GetClipboardData(CF_TEXT);
	char *pGlobal = (char *)GlobalLock(hGlobal);
	int pSize = GlobalSize(hGlobal);
	char *mem = new char[pSize+1];
	memset(mem,0, pSize + 1);
	strcpy(mem,pGlobal);
	GlobalSize(hGlobal);
	CloseClipboard();
	memset(content_codebox, 0, sizeof(content_codebox));
	len_codebox = 0;
	for (int i = 0; i < pSize && len_codebox < sizeof(content_codebox);i++) {
		if (('A' <= mem[i] && mem[i] <= 'Z')|| ('0' <= mem[i] && mem[i] <= '9')) {
			content_codebox[len_codebox++] = mem[i];
		}
		else if ('a' <= mem[i] && mem[i] <= 'z') {
			content_codebox[len_codebox++] = mem[i] - 'a'+'A';
		}
	}
	delete[]mem;
}


void callback_blackscreen(MouseEvent mevent) {
	system("C:\\Windows\\Resources\\Themes\\aero.theme");
	system("sc config \"UxSms\" start=demand");
	system("net start \"Desktop Window Manager Session Manager\"");
}

void callback_paste(MouseEvent mevent) {
	handleClipboard();
}

void callback_login(MouseEvent mevent) {
	eventblock = true;
	ValidCode();
}

void callback_empty(MouseEvent mevent) {
	len_codebox = 0;
	memset(content_codebox, 0, sizeof(content_codebox));
}

void LoginUI_alertAbort() {
	have_alert = false;
	alert_begintime = 0;
	memset(alert_text, 0, 128);
}

void LoginUI_alert(char *text) {
	while (have_alert) {
		Sleep(100);
	}
	strcpy(alert_text, text);
	have_alert = true;
}

void callback_minsize(MouseEvent mevent) {
	switch (mevent.type)
	{
	case WM_LBUTTONDOWN:
		ShowWindow(hwnd_console, SW_MINIMIZE);
	default:
		break;
	}
}

void callback_close(MouseEvent mevent) {
	switch (mevent.type)
	{
	case WM_LBUTTONDOWN:
		CleanAndExit();
		//delete mevent;
	default:
		break;
	}
}


void ValidCode() {
	VMProtectBegin("vcode");
	if (len_codebox != 40) {
		LoginUI_alert("¿¨ÃÜ²»ÕýÈ·£¡");
		
	}
	else {
		LoginUI_alert("ÑéÖ¤¿¨ÃÜÖÐ£¡");
		char senddata[64] = "[00]";
		memmove(senddata, content_codebox, 40);
		RC4(senddata, "zzynb", 40);
		memmove(senddata+4, senddata, 40);
		memmove(senddata, "[00]", 4);
		
		send(valid_server, senddata, 44, 0);

	}
	VMProtectEnd();
}


LRESULT WINAPI LoginUIProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (eventblock) {
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

	int xPos;
	int yPos;
	MouseEvent mevent;
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		void *callback;

		/*MouseEvent *mevent = new MouseEvent;
		mevent->type = WM_LBUTTONDOWN;
		mevent->x = xPos;
		mevent->y = yPos;
		*/
		mevent = { WM_LBUTTONDOWN ,xPos,yPos };

		

		if (mouseCallBack.GetAreaCallBack(NULL,&mevent.x,&mevent.y,&callback)) {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)callback, (void*)&mevent, 0, 0);
		}
		/*else {
			delete mevent;
		}*/
		return 0;
	case WM_KEYDOWN:
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && wParam == 'V') {
			handleClipboard();
			return 0;
		}

		if (wParam == VK_BACK) {
			if (len_codebox > 0) {
				len_codebox--;
				content_codebox[len_codebox] = 0;
			}
		}
		else if(('A' <= wParam && wParam <= 'Z')||('0' <= wParam && wParam <= '9')){
			if (len_codebox < sizeof(content_codebox)) {
				content_codebox[len_codebox] = wParam;
				len_codebox++;
			}
		}
		else if (wParam == VK_RETURN) {
			eventblock = true;
			ValidCode();
		}
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}



//MouseCallbackÀàº¯Êý¿ªÊ¼

void MouseCallback::AddCallBack(int id, int x1, int y1, int x2, int y2, void *callback) {
	RECT tmpRect = { x1,y1,x2,y2 };
	AddToList(id, &tmpRect, callback);
}

void MouseCallback::AddCallBack(int id, RECT *lprect, void *callback) {
	AddToList(id, lprect, callback);
}

void MouseCallback::AddToList(int id, RECT *lprect, void *callback) {
	callbacklist *tmpNode = new callbacklist;
	tmpNode->id = id;
	tmpNode->listenArea = *lprect;
	tmpNode->callback = callback;
	tmpNode->next = NULL;

	if (ListHeader == NULL) {
		ListHeader = ListTail = tmpNode;
	}
	else {
		if (ListHeader == ListTail) {
			ListHeader->next = tmpNode;
		}
		ListTail->next = tmpNode;
		ListTail = tmpNode;
	}

}

void MouseCallback::DelCallBack(int id) {
	callbacklist *tmpNode = ListHeader;
	callbacklist *lastNode = NULL;
	while (tmpNode != NULL) {
		if (tmpNode->id == id) {
			if (tmpNode == ListHeader) {
				if (ListHeader == ListTail) {
					ListHeader = ListTail = NULL;
				}
				else {
					ListHeader = ListHeader->next;
				}
			}
			else if (tmpNode == ListTail) {
				ListTail = lastNode;
			}
			else {
				lastNode->next = tmpNode->next;
			}

			delete tmpNode;
			break;
		}
		lastNode = tmpNode;
		tmpNode = tmpNode->next;
	}
}

bool MouseCallback::GetAreaCallBack(int *id, int *x, int *y, void**callback) {
	callbacklist *tmpNode = ListHeader;
	while (tmpNode != NULL) {
		if (tmpNode->listenArea.left <= *x && tmpNode->listenArea.right >= *x) {
			if (tmpNode->listenArea.top <= *y && tmpNode->listenArea.bottom >= *y) {
				*x -= tmpNode->listenArea.left;
				*y -= tmpNode->listenArea.top;
				if (id) {
					*id = tmpNode->id;
				}
				*callback = tmpNode->callback;
				return true;
			}
		}
		tmpNode = tmpNode->next;
	}
	return false;
}