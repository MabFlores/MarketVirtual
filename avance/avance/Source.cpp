#include <windows.h>
#include <Commctrl.h> 
#include "resource.h"

HINSTANCE H_DIALOG2, H_DIALOG3, H_DIALOG4, H_DIALOG5;
BOOL CALLBACK fSecundario2(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fTerciario1(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fPrincipal(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {

	H_DIALOG2 = hInst;
	H_DIALOG3 = hInst;
	H_DIALOG4 = hInst;
	H_DIALOG5 = hInst;
	HWND hwnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PRINCIPAL), NULL, fPrincipal);
	ShowWindow(hwnd, SW_SHOW);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 1;
}

BOOL CALLBACK fPrincipal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	switch (msg) {

	case WM_COMMAND: {

		if (LOWORD(wparam) == BTN_ENTRAR && HIWORD(wparam) == BN_CLICKED) {
			HWND hSecondWindow = CreateDialog(H_DIALOG2, MAKEINTRESOURCE(IDD_SECUNDARIO1), NULL, fSecundario1);
			ShowWindow(hSecondWindow, SW_SHOW);
			ShowWindow(hwnd, SW_HIDE);
			break;
		}
		if (LOWORD(wparam) == BTN_REGISTRO_USUARIO && HIWORD(wparam) == BN_CLICKED) {
			HWND hThirdWindow = CreateDialog(H_DIALOG3, MAKEINTRESOURCE(IDD_SECUNDARIO2), NULL, fSecundario2);
			ShowWindow(hThirdWindow, SW_SHOW);
			ShowWindow(hwnd, SW_HIDE);
			break;
		}
		break;
	}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(9);
		break;
	}
	return FALSE;
}
BOOL CALLBACK fSecundario2(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	switch (msg) {

	case WM_COMMAND: {

		if (LOWORD(wparam) == BTN_REGISTRARTE && HIWORD(wparam) == BN_CLICKED) {
			HWND hFourthWindow = CreateDialog(H_DIALOG4, MAKEINTRESOURCE(IDD_TERCIARIO), NULL, fTerciario1);
			ShowWindow(hFourthWindow, SW_SHOW);
			ShowWindow(hwnd, SW_HIDE);
		}
		else if (LOWORD(wparam) == BTN_ATRAS_REGISTRARTE && HIWORD(wparam) == BN_CLICKED) {
			HWND hwnd = CreateDialog(H_DIALOG5, MAKEINTRESOURCE(IDD_PRINCIPAL), NULL, fPrincipal);
			ShowWindow(hwnd, SW_SHOW);
		}
		break;
	}
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
BOOL CALLBACK fTerciario1(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	switch (msg) {

	case WM_COMMAND: {

		if (LOWORD(wparam) == BTN_CARGAR_IMAGEN && HIWORD(wparam) == BN_CLICKED) {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			char cDirFile[MAX_PATH] = "";

			ofn.hwndOwner = hwnd;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFile = cDirFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrFilter = "Archivos de texto\0*.txt\0Todos los archivos\0*.*\0Imagenes bmp\0*.bmp";

			if (GetOpenFileName(&ofn)) {
				HBITMAP hbFotoRegistro = (HBITMAP)LoadImage(NULL, cDirFile, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
				HWND hPbImagen = GetDlgItem(hwnd, PB_FOTO);
				SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbFotoRegistro);
			}
		}
	}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(9);
		break;
	}
	return FALSE;
}
//=z4Jv%'RZ*LRkwX