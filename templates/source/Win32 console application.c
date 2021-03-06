/*==========================================================================*/
/*
@@INCLUDE:$\templates\source\header-base@@
*/
/*--------------------------------------------------------------------------*/
/*
@@INCLUDE:$\templates\source\header-gnu@@
*/
/*--------------------------------------------------------------------------*/
/*
@@INCLUDE:$\templates\source\header-cvs@@
*/
/*==========================================================================*/
/*
 Remarks:
  To compile an application that uses this function, define the
  _WIN32_WINNT macro as 0x0500 or later. For more information,
  see Using the Windows Headers.
*/
/*==========================================================================*/
#ifndef __@@UPPERFILE_NAME@@_@@UPPERFILE_EXT@@__
#define __@@UPPERFILE_NAME@@_@@UPPERFILE_EXT@@__
/*==========================================================================*/
#include <windows.h>
/*==========================================================================*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	RECT rect;

	switch (iMsg)
	{
		case WM_CREATE:
		{
			return 0;
		}
		case WM_PAINT:
		{
			hDC = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rect);
			DrawText(hDC, "Hello, Windows!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			EndPaint(hWnd, &ps);
			return 0;
		}
		case WM_KEYDOWN:
		{
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		case WM_KEYUP:
		{
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
/*==========================================================================*/
int main(int argc, char* argv[])
{
	@@HERE@@const char ClassName[] = "Win32 class";
	INPUT_RECORD InBuf[128];
	CONSOLE_CURSOR_INFO cci;
	HANDLE hStdin, hStdout;
	DWORD Length, Temp, i;
	WNDCLASSEX WndClass;
	HWND hConsole, hWnd;
	RECT Rect;
	MSG Msg;

	hConsole = GetConsoleWindow();
	if (hConsole == NULL)
		return -1;
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hStdout, &cci);

	WndClass.cbSize			= sizeof(WndClass);
	WndClass.style			= CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc	= WndProc;
	WndClass.cbClsExtra		= 0;
	WndClass.cbWndExtra		= 0;
	WndClass.hInstance		= GetModuleHandle(NULL);
	WndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName	= NULL;
	WndClass.lpszClassName	= ClassName;
	WndClass.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(ClassName,	// Window class name
		"The Hello Program",		// Window caption
		WS_CHILD | WS_CLIPSIBLINGS,	// Window style
		CW_USEDEFAULT,				// Initial x position
		CW_USEDEFAULT,				// Initial y position
		CW_USEDEFAULT,				// Initial x size
		CW_USEDEFAULT,				// Initial y size
		hConsole,					// Parent window handle
		NULL,						// Window menu handle
		GetModuleHandle(NULL),		// Program instance handle
		NULL);						// Creation parameters

	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hWnd);

	ZeroMemory(&Msg, sizeof(Msg));
	while (Msg.message != WM_QUIT)
	{
		if (PeekMessage(&Msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else
		{
			PeekConsoleInput(hStdin, InBuf, 128, &Length);
			for (i = 0; i < Length; i++)
			{
				if (InBuf[i].EventType == KEY_EVENT)
				{
					if (InBuf[i].Event.KeyEvent.bKeyDown)
						SendMessage(hWnd, WM_KEYDOWN, (WPARAM)InBuf[i].Event.KeyEvent.wVirtualKeyCode, 0);
					else
						SendMessage(hWnd, WM_KEYUP, (WPARAM)InBuf[i].Event.KeyEvent.wVirtualKeyCode, 0);
				}
				ReadConsoleInput(hStdin, &InBuf[i], 1, &Temp);
			}
		}
	}
	return (int)Msg.wParam;
}
/*==========================================================================*/
#endif
