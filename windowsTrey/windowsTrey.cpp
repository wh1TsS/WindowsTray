#include <iostream>
#include <windows.h>
#include <shellapi.h>

#define WM_SYSICON (WM_USER + 1)
#define WM_COMMAND_EXIT (WM_USER + 2)
#define WM_COMMAND_NOTIFY (WM_USER + 3)
#define WM_COMMAND_NOTIF (WM_USER + 4)


using namespace std;

NOTIFYICONDATA nid;

void InitNotifyIconData(HWND hWnd) {
	nid.cbSize = sizeof(NOTIFYICONDATA);

	nid.hWnd = hWnd;

	nid.uID = 1;
	
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;

	nid.uCallbackMessage = WM_SYSICON;

	nid.hIcon = (HICON)LoadImage(NULL, TEXT("C:\\Users\\User\\Downloads\\chase_badge_canine_patrol_paw_patrol_icon_263831.ico"),
		IMAGE_ICON, 32, 32,
		LR_LOADFROMFILE | LR_SHARED);

	memcpy(nid.szTip, TEXT("ON"), 2 * sizeof(TCHAR));

	Shell_NotifyIcon(NIM_ADD, &nid);
}

void CleanUp() {
	Shell_NotifyIcon(NIM_DELETE, &nid); 
}

void ShowNotification(const wchar_t* title, const wchar_t* message) {
	NOTIFYICONDATA nidNotify = {};
	nidNotify.cbSize = sizeof(NOTIFYICONDATA);
	nidNotify.hWnd = nid.hWnd;
	nidNotify.uID = nid.uID;
	nidNotify.uFlags = NIF_INFO;
	wcscpy_s(nidNotify.szInfoTitle, title);
	wcscpy_s(nidNotify.szInfo, message);
	nidNotify.uTimeout = 5000;
	Shell_NotifyIcon(NIM_MODIFY, &nidNotify);
	}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam) {
	switch (uMsg) {
	case WM_SYSICON:
		if (lParam == WM_RBUTTONDOWN) {
			HMENU hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING, WM_COMMAND_NOTIFY, TEXT("Show notification"));
			AppendMenu(hMenu, MF_STRING, WM_COMMAND_NOTIF, TEXT("Show Notification2"));
			AppendMenu(hMenu, MF_STRING, WM_COMMAND_EXIT, TEXT("Exit"));


			POINT pt;
			GetCursorPos(&pt);
			SetForegroundWindow(hWnd);
			TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
			DestroyMenu(hMenu);
		}
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == WM_COMMAND_NOTIFY) {
			ShowNotification(TEXT("Title"), TEXT("Description"));
		}
		else if (LOWORD(wParam) == WM_COMMAND_EXIT) {
			CleanUp();
			PostQuitMessage(0);
		}
		else if (LOWORD(wParam) == WM_COMMAND_NOTIF) {
			ShowNotification(TEXT("Helloooooooooooo"), TEXT("World"));
		}
		break;
	case WM_DESTROY:
		CleanUp();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
	WNDCLASS wc = {}; 
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("windowsTrey");

	RegisterClass(&wc);
	HWND hWnd = CreateWindow(wc.lpszClassName, TEXT("Tray app"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, NULL, NULL, hInstance, NULL);    

	InitNotifyIconData(hWnd);

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return  0;

}


