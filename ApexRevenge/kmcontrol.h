#pragma once

#ifdef _USRDLL
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT
#endif

#define VK_NUMRETURN 0x8F // 用来定义小键盘回车


EXPORT void __cdecl SetHandle(HANDLE handle);
EXPORT void __cdecl KeyDown(USHORT VirtualKey);
EXPORT void __cdecl KeyUp(USHORT VirtualKey);
EXPORT void __cdecl MouseLeftButtonDown();
EXPORT void __cdecl MouseLeftButtonUp();
EXPORT void __cdecl MouseRightButtonDown();
EXPORT void __cdecl MouseRightButtonUp();
EXPORT void __cdecl MouseMiddleButtonDown();
EXPORT void __cdecl MouseMiddleButtonUp();
EXPORT void __cdecl MouseButton4Down();
EXPORT void __cdecl MouseButton4Up();
EXPORT void __cdecl MouseButton5Down();
EXPORT void __cdecl MouseButton5Up();
EXPORT void __cdecl MouseWheelDown();
EXPORT void __cdecl MouseWheelUp();
EXPORT void __cdecl MouseMoveRELATIVE(LONG dx, LONG dy);
EXPORT void __cdecl MouseMoveABSOLUTE(LONG dx, LONG dy);

BOOL LoadNTDriver(char * lpszDriverName, char * lpszDriverPath);

BOOL UnloadNTDriver(char * szSvrName);


