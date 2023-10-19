#include <Windows.h>
#include<string.h>
#include "resource.h"
using namespace std;

BOOL CALLBACK funcionPrincipal(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {

	HWND hwnd = CreateDialogW(hInst, MAKEINTRESOURCE(IDD_PRINCIPAL), NULL, funcionPrincipal);
	ShowWindow(hwnd, SW_SHOW);
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 1;
}
BOOL CALLBACK funcionPrincipal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	switch (msg) {

	case WM_INITDIALOG: {
		HWND hBtnMostrar = GetDlgItem(hwnd, BTN_ENTRAR);
		EnableWindow(hBtnMostrar, false);

	}
		break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_ENTRAR: {

		}
		 break;
		case BTN_REGISTER: {
			if (HIWORD(wparam) == BN_CLICKED) {

				char txtIniciarSesion[50];
				HWND hTxtIniciarSesion = GetDlgItem(hwnd, TXT_USUARIO);
				int txtLength = GetWindowTextLength(hTxtIniciarSesion);
				GetWindowText(hTxtIniciarSesion, txtIniciarSesion, ++txtLength);
				string siniciarsesion(txtIniciarSesion);
				MessageBox(hwnd, siniciarsesion.c_str(), "REGISTRATE", MB_OK);


			}
		}
						 break;
		}
	}


				   break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(90);
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return FALSE;
}