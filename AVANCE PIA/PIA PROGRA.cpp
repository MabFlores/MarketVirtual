#include<Windows.h>
#include"resource.h"

BOOL CALLBACK funcionPrincipal(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {

	HWND hwnd=CreateDialog(hInst,MAKEINTRESOURCE(DLG_INICIO_SESION),NULL, funcionPrincipal);

	ShowWindow(hwnd,cShow);

	MSG msg;
	ZeroMemory(&msg,sizeoff(MSG));

	while (GetMessage(&msg,NULL,0,0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 1;
}
BOOL CALLBACK funcionPrincipal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(9);
		break;
	default:
			return DefWindowProc(hwnd, msg, wparam, lparam);

	}
	return FALSE;
}