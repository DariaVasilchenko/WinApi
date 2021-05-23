#include <Windows.h>
#include <Windowsx.h>
#include <tchar.h>
#include <commctrl.h>
#include <string>

#define FIRST_TIMER 1
bool month = false;
HANDLE hMutex = CreateMutex(0, 0, 0);
int x1 = 110, x3 = 90;
int v = 2000;
HWND hEdit;

LRESULT CALLBACK HelloWordWndProc(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI ThreadMonth(LPVOID lpParam);
DWORD WINAPI ThreadMemory(LPVOID lpParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	WNDCLASS WndClass;
	MSG Msg;
	TCHAR szClassName[] = _T("MainFrame");
	TCHAR szTitel[] = _T("SecondProg");

	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
	WndClass.lpfnWndProc = HelloWordWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = CreateSolidBrush(0x00FFFFFF);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = szClassName;


	if (!RegisterClass(&WndClass))
	{
		MessageBox(NULL, _T("Cannot register class"), _T("Error"), MB_OK);
		return 0;
	}
	hWnd = CreateWindow(szClassName, szTitel, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	//hEdit = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 56, 10, 50, 18, hWnd, (HMENU)2, hInstance, 0);
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
	case WM_CREATE:
		SetTimer(hWnd, FIRST_TIMER, v, NULL);
		SetTimer(hWnd, 2, 60, NULL);
		SetTimer(hWnd, 3, v/34, NULL);
		return 0;
	case WM_TIMER:
		if (wParam == FIRST_TIMER)
			month = !month;
		if (wParam == 3) {
			if (month == false) {
				x1 += 3;
				x3 -= 3;
			}
			else {
				x1 -= 3;
				x3 += 3;
			}
			//InvalidateRect(hWnd, NULL, 0);
		}
		InvalidateRect(hWnd, NULL, 0);
		return 0;
	case WM_PAINT: {
		CreateThread(NULL, 0, &ThreadMonth, hWnd, 0, NULL);
		CreateThread(NULL, 0, &ThreadMemory, hWnd, 0, NULL);
		return 0;
	}
	case WM_DESTROY:
		KillTimer(hWnd, FIRST_TIMER);
		KillTimer(hWnd, 2);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, Message, wParam, lParam);
}


DWORD WINAPI ThreadMonth(LPVOID lpParam)
{
	HWND hWnd = (HWND)lpParam;
	PAINTSTRUCT PaintStruct;
	WaitForSingleObject(hMutex, INFINITY);
	HDC hDC = BeginPaint(hWnd, &PaintStruct);
	RECT rc;
	GetClientRect(hWnd, &rc);
	HPEN hPen;
	HBRUSH hBrush;
	hPen = CreatePen(PS_SOLID, 1, 0x00FFFFFF);
	hBrush = CreateSolidBrush(0x0000D0F0);
	SelectObject(hDC, hBrush);
	SelectObject(hDC, hPen);
	RoundRect(hDC, 100, 100, 200, 200, 100, 100);
	hBrush = CreateSolidBrush(0x00FFFFFF);
	SelectObject(hDC, hBrush);
	RoundRect(hDC, x1, 100, 200, 200, x3, 100);
	/*if (month)
		RoundRect(hDC, 110, 100, 200, 200, 90, 100);
	else
		RoundRect(hDC, 120, 100, 200, 200, 80, 100);*/
	EndPaint(hWnd, &PaintStruct);
	ReleaseMutex(hMutex);
	return 0;
}

DWORD WINAPI ThreadMemory(LPVOID lpParam)
{
	MEMORYSTATUSEX buff = MEMORYSTATUSEX();
	buff.dwLength = sizeof(buff);
	HWND hWnd = (HWND)lpParam;
	WaitForSingleObject(hMutex, INFINITY);
	PAINTSTRUCT PaintStruct;
	HDC hDC = BeginPaint(hWnd, &PaintStruct);
	GlobalMemoryStatusEx(&buff);
	RECT Rect;
	GetClientRect(hWnd, &Rect);
	TextOut(hDC, (Rect.right - Rect.left) / 2 + 100, (Rect.bottom - Rect.top) / 2, (L"Total: " + std::to_wstring(buff.ullTotalPhys / 1024) + L" Κα " + L" Avail: " + std::to_wstring(buff.ullAvailPhys / 1024) + L" Κα ").c_str(), (L"Total: " + std::to_wstring(buff.ullTotalPhys / 1024) + L" Κα " + L" Avail: " + std::to_wstring(buff.ullAvailPhys / 1024) + L" Κα ").length());
	//DrawText(hDC, (LPWSTR) (L"Total: " + std::to_wstring(buff.ullTotalPhys / 1024) + L" Κα " + L" Avail: " + std::to_wstring(buff.ullAvailPhys / 1024) + L" Κα ").c_str(), -1, &Rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	EndPaint(hWnd, &PaintStruct);
	ReleaseMutex(hMutex);
	return 0;
}
