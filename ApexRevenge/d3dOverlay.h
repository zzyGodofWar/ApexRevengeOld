#pragma once
#pragma once
#include <time.h>
#include <Windows.h>
#include <dwmapi.h>
#include <d3d9.h>
#include <d3dx9core.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


#define d3dcolor_Red D3DCOLOR_ARGB(255, 255, 0, 0)
#define d3dcolor_Green D3DCOLOR_ARGB(255, 0, 255, 0)
#define d3dcolor_Blue D3DCOLOR_ARGB(255, 0, 0, 255)
#define d3dcolor_Black D3DCOLOR_ARGB(255, 0, 0, 0)
#define d3dcolor_White D3DCOLOR_ARGB(255, 255, 255, 255)
#define d3dcolor_Yellow D3DCOLOR_ARGB(255, 255, 255, 0)


bool ´°¿Ú´´½¨(int width, int height, HWND * dxhwnd, IDirect3D9 ** lpD3d9Object, IDirect3DDevice9 ** lpD3d9Device);

void D3DÉèÖÃ´°¿Ú´©Í¸(HWND wHwnd, bool state);

void D3D»­¾ØÐÎ(IDirect3DDevice9 **lpd3d9device, LPD3DXLINE * lpd3dline, int x1, int y1, int x2, int y2, bool OutLine, int LineWidth, D3DCOLOR LineColor, bool Filled, D3DCOLOR FillColor);

void D3DÐ´×Ö(LPD3DXFONT *lpd3dfont, int x, int y, int fontHeight, D3DCOLOR color, char * string);

void D3DÃè±ßÐ´×Ö(LPD3DXFONT *lpd3dfont, int x, int y, int fontHeight, D3DCOLOR color, D3DCOLOR backcolor, char * string);

void GetRandomString(int length, wchar_t * string);

int Random(int x, int y);