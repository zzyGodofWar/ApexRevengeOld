#pragma once
#include "main.h"

extern HWND hwnd_loginui;
extern IDirect3D9 *d3dObject_loginui;
extern IDirect3DDevice9 *d3dDevice_loginui;

void LoadLoginUI();

void LoginUI_alertAbort();

void LoginUI_alert(char * text);

extern char content_codebox[46];
extern int len_codebox;
extern bool remembercode;
extern bool login_quit;

typedef struct MouseEvent{
	int type;
	int x;
	int y;
}MouseEvent;


class MouseCallback {
private:
	typedef struct callbacklist {
		int id;
		RECT listenArea;
		void* callback;
		struct callbacklist *next;
	}callbacklist;

	callbacklist *ListHeader = NULL;
	callbacklist *ListTail = NULL;

	void AddToList(int id, RECT *lprect, void *callback);
public:
	void AddCallBack(int id,int x1,int y1,int x2,int y2,void *callback);
	void AddCallBack(int id,RECT *lprect, void *callback);
	void DelCallBack(int id);
	bool GetAreaCallBack(int *id, int *x, int *y,void**callback);
};

