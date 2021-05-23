#include <Windows.h>
#include <Windowsx.h>
#include <tchar.h>
#include <commctrl.h>
#include <string>
#include "resource.h"

#define FIRST_TIMER 1

bool timerIsActive = false;
int minutes = 0;
int seconds = 0;
int milliseconds = 0;
//double timer = 0;
HWND hButtonStart, hButtonReset;
//HMENU hMenu;

LRESULT CALLBACK HelloWordWndProc(HWND, UINT, WPARAM, LPARAM);
/*TBBUTTON tbb[] = {
					 { 1, 181,  TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
					 { 2, 182,  TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
};
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	HMENU hMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU1));
	WNDCLASS WndClass;
	MSG Msg;
	TCHAR szClassName[] = _T("MainFrame");
	TCHAR szTitel[] = _T("SecondProg");

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = HelloWordWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = szClassName;


	if (!RegisterClass(&WndClass))
	{
		MessageBox(NULL, _T("Cannot register class"), _T("Error"), MB_OK);
		return 0;
	}
	hWnd = CreateWindow(szClassName, szTitel, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	hButtonStart = CreateWindow(L"button", L"Старт / Стоп", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 90, 30, hWnd, (HMENU)10000, hInstance, NULL);
	hButtonReset = CreateWindow(L"button", L"Сброс", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 10, 90, 30, hWnd, (HMENU)10001, hInstance, NULL);
	//hMenu = CreateMenu();
	//SetMenu(hWnd, hMenu);
	//LoadMenu(hInstance, L"aaa");

	SetMenu(hWnd, hMenu);

	if (!hWnd)
	{
		MessageBox(NULL, _T("Cannot create window"), _T("Error"), MB_OK);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

LRESULT CALLBACK HelloWordWndProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam)
{
	switch (Message)
	{
	case WM_COMMAND:
		if (lParam == (LPARAM)hButtonStart)
			timerIsActive = !timerIsActive;
		if (lParam == (LPARAM)hButtonReset)
		{
			//timer = 0;
			minutes = 0;
			seconds = 0;
			milliseconds = 0;
			InvalidateRect(hWnd, NULL, 0);
		}
		switch (LOWORD(wParam))
		{
		case ID_40001:
			timerIsActive = !timerIsActive;
			break;
		case ID_40003:
			minutes = 0;
			seconds = 0;
			milliseconds = 0;
			InvalidateRect(hWnd, NULL, 0);
			break;
		}
		return 0;
	case WM_CREATE:
		SetTimer(hWnd, FIRST_TIMER, 100, NULL);
		return 0;
	case WM_TIMER:
		if (timerIsActive) {
			milliseconds += 1;
			//timer += 0.1;
			if (milliseconds == 10) {
				seconds += 1;
				milliseconds = 0;
				if (seconds == 60) {
					minutes += 1;
					seconds = 0;
				}
			}
		}
		InvalidateRect(hWnd, NULL, 0);
		return 0;
	case WM_PAINT: {
		PAINTSTRUCT PaintStruct;
		HDC hDC = BeginPaint(hWnd, &PaintStruct);

		RECT Rect;
		GetClientRect(hWnd, &Rect);
		if ((seconds < 10)&&(minutes < 10))
			TextOut(hDC, (Rect.right - Rect.left) / 2, (Rect.bottom - Rect.top) / 2, (LPCTSTR)(L"0" + std::to_wstring(minutes) + L":0" + std::to_wstring(seconds) + L":" + std::to_wstring(milliseconds) + L"0").c_str(), (L"0" + std::to_wstring(minutes) + L":0" + std::to_wstring(seconds) + L":" + std::to_wstring(milliseconds) + L"0").length());
		else
			if (seconds < 10)
				TextOut(hDC, (Rect.right - Rect.left) / 2 + 100, (Rect.bottom - Rect.top) / 2, (LPCTSTR)(std::to_wstring(minutes) + L":0" + std::to_wstring(seconds) + L":" + std::to_wstring(milliseconds) + L"0").c_str(), (std::to_wstring(minutes) + L":0" + std::to_wstring(seconds) + L":" + std::to_wstring(milliseconds) + L"0").length());
			else
				if (minutes < 10)
					TextOut(hDC, (Rect.right - Rect.left) / 2 + 100, (Rect.bottom - Rect.top) / 2, (LPCTSTR)(L"0" + std::to_wstring(minutes) + L":" + std::to_wstring(seconds) + L":" + std::to_wstring(milliseconds) + L"0").c_str(), (L"0" + std::to_wstring(minutes) + L":" + std::to_wstring(seconds) + L":" + std::to_wstring(milliseconds) + L"0").length());
				else
					TextOut(hDC, (Rect.right - Rect.left) / 2 + 100, (Rect.bottom - Rect.top) / 2, (LPCTSTR)(std::to_wstring(minutes) + L":" + std::to_wstring(seconds) + L":" + std::to_wstring(milliseconds) + L"0").c_str(), (std::to_wstring(minutes) + L":" + std::to_wstring(seconds) + L":" + std::to_wstring(milliseconds) + L"0").length());
		//DrawText(hDC, ((LPWSTR)std::to_wstring(minutes).c_str() + L":") , -1, &Rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		//DrawText(hDC, (LPWSTR) std::to_wstring(timer).c_str(), -1, &Rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &PaintStruct);
		return 0;
	}

	case WM_DESTROY:
		KillTimer(hWnd, FIRST_TIMER);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, Message, wParam, lParam);
}
