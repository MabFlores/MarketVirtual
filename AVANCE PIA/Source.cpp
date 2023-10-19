#include <Windows.h>
#include "resource.h"
#include <string>
#include <ctype.h>
#include <fstream>
#include <ctime>

#define MI_TIMER 999
using namespace std;

#pragma region ESTRUCTURAS

int y;

struct envios {
	bool control = false;
	int IDEnvios;
	int IDProductos;
	int IDUsuario;
	int IDFecha;
	string sDia; //AQUI
	string sMes; //AQUI
	string sAnio;
	string sTotal;
	string sNombreProducto;
	string sCantidad;
	string sCalle;
	string sColonia;
	string sCiudad;
	string sEstado;
	string sMensaje;

	envios* sig;
	envios* prev;
}*origenEnvios, * auxiliarEnvios, *currentEnvio;

struct productos {
	int IDProductos;
	int IDUsuario;
	char foto1[MAX_PATH];
	char foto2[MAX_PATH];
	string sNombreProducto;
	string sCantidad;
	string sCodigo;
	string sMarca;
	string sDescripcion;
	string sPrecio;

	productos* sig;
	productos* prev;
}*origenProducto, * auxiliarProducto, * currentProducto;

struct USUARIO {
	bool control = false;
	int IDVendedor;
	int IDUsuario;
	char foto[MAX_PATH];
	string sNombre;
	string sPassword;
	string sNombreVendedor;
	string sNombreEmpresa;
	USUARIO* sig;
	USUARIO* prev;

}*origen, * auxiliar, * userAccess;
#pragma endregion

#pragma region PROTOTIPOS
void loadUser();
void save(USUARIO*);
void loadProductos();
void saveProductos(productos*);
void loadEnvios();
void saveEnvios(envios*);
void getGlobalId();
void saveGlobalId();
string getText(HWND);
bool fValidarHayContenido(int);
void fInstertarMenu(HWND hwnd);
bool fValidarSoloLetras(string, int);
bool fValidarSoloNumeros(string, int);

BOOL CALLBACK funcionPrincipal(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK funcionRegistro(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK funcionInfoVendedor(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK funcionInfoVendedorStatic(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK funcionInfoVendedorEdit(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK funcionNuevoEnvio(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK funcionModEnvio(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK funcionEliminarEnvio(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK funcionResumenEnvio(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK funcionAltaProducto(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK funcionModProducto(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK funcionBajaProducto(HWND, UINT, WPARAM, LPARAM);
#pragma endregion

#pragma region DECLARACIONES
HINSTANCE ghInst;
bool ExitInicioSesion = false;
bool ExitRegistro = false;
bool exitWindowInfoVendedor = false;
bool exitWindowInfoVendedorStatic = false;
bool exitWindowInfoVendedorEdit = false;
bool exitWindowNuevoEnvio = false;
bool exitWindowModEnvio = false;
bool exitWindowEliminarEnvio = false;
bool exitWindowResumenEnvio = false;
bool exitWindowAltaProducto = false;
bool exitWindowModProdcuto = false;
bool exitWindowBajaProducto = false;
HWND hTxtRegisterUserName;
HWND hTxtRegisterPassword;
HWND hTxtLoginUserName;
HWND hTxtLoginPassword;
HWND hTxtNombreProducto;
HWND hTxtCantidadProd;
HWND hTxtCodigoProd;
HWND hTxtMarcaProd;
HWND hTxtDescripcionProd;
HWND hTxtPrecioProd;
HWND hListBoxProductos;
HWND hTxtModProdNombre;
HWND hTxtModProdProductos;
HWND hTxtModProdCodigo;
HWND hTxtModProdMarca;
HWND hTxtModProdPrecio;
HWND hTxtModProdDescripcion;
HWND hFechaEnvio;
HWND hTxtNuevoProductoEnvio;
HWND hTxtNuevoEnvioCantidad;
HWND hTxtNuevoEnvioCalle;
HWND hTxtNuevoEnvioColonia;
HWND hTxtNuevoEnvioCiudad;
HWND hTxtNuevoEnvioMensaje;
HWND hListBoxEnvios;

HWND hLblMosEnvioCantidad;
HWND hLblMosEnvioCalle;
HWND hLblMosEnvioColonia;
HWND hLblMosEnvioCiudad;
HWND hLblMosEnvioMensaje;


fstream lectorEscritor;
string sRutaImagen = "0";
string sRutaImagen2 = "0";
int GLOBAL_USUARIO_ID = 1;
int GLOBALID_PRODUCTOS = 1;
int GLOBALID_ENVIOS = 1;
#pragma endregion

#pragma region TIEMPO //DE AQUI
time_t longSecondsUnix; //Guardar el tiempo del sistema en base a la fecha que se creo Unix
tm* structOFTime; //Estructura para convertir lo de unix a dia, mes, año

int fechaActual;
bool fechaObtenida = false;
#pragma endregion  //HASTA AQUI

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {
	
	getGlobalId();	
	origen = auxiliar = NULL;
	origenProducto = auxiliarProducto = NULL;
	origenEnvios = auxiliarEnvios = NULL;
	loadUser();
	loadProductos();
	loadEnvios();
	ghInst = hInst;

	HWND hwnd = CreateDialog(hInst, MAKEINTRESOURCE(DLG_INICIO_SESION), NULL, funcionPrincipal);

	ShowWindow(hwnd, SW_SHOW);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return msg.wParam;
}

bool fValidarSoloNumeros(string NUMEROS, int TAMAÑO) {
	bool numeros;
	int contador = 0;
	for (int i = 0; i < TAMAÑO; i++) {
		numeros = isdigit(NUMEROS[i]);
		if (!(numeros)) {//no aumenta si es verdadero, si es falso, lo hace verdadero para aumentar "!"
			contador++;
		}
	}
	if (contador > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool fValidarSoloLetras(string PALABRA, int TAMAÑO) {
	bool letras, espacios;
	int contador = 0;
	for (int i = 0; i < TAMAÑO; i++) {
		
		letras = isalpha(PALABRA[i]);
		espacios = isspace(PALABRA[i]);
		if (!(letras || espacios)) {
			contador++;
		}
	}
	if (contador > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool fValidarHayContenido(int size) {
	if (size < 2)
		return true;
	else
		return false;
}

void fInstertarMenu(HWND hwnd) {
	HMENU hMenu = CreateMenu();
	HMENU hMenuPopUp1 = CreateMenu();
	HMENU hMenuPopUp2 = CreateMenu();

	AppendMenu(hMenu, MF_STRING, ID_INFO_VENDEDOR, "Informacion de vendedor");

	AppendMenu(hMenuPopUp1, MF_STRING, ID_ALTA_PRODUCTOS, "Alta de Productos");
	AppendMenu(hMenuPopUp1, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenuPopUp1, MF_STRING, ID_BAJA_PRODUCTOS, "Resumen de Productos");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hMenuPopUp1, "Productos");


	AppendMenu(hMenuPopUp2, MF_STRING, ID_NUEVO_ENVIO, "Nuevo envio");
	AppendMenu(hMenuPopUp2, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenuPopUp2, MF_STRING, ID_ENVIOS_ELIMINARENVIO, "Resumen de envio");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hMenuPopUp2, "Envios");

	AppendMenu(hMenu, MF_STRING, ID_SALIR, "Salir");
	SetMenu(hwnd, hMenu);
	return;
}

string getText(HWND hwnd) {

	int length = GetWindowTextLength(hwnd);
	if (length < 1)
	return "";
	char cText[50];
	GetWindowText(hwnd, cText, ++length);
	string sText(cText);
	return sText;
}

void getGlobalId() {
	lectorEscritor.open("GlobalId.txt", ios::in);
	if (lectorEscritor.is_open()) {
		lectorEscritor >> GLOBAL_USUARIO_ID;
		lectorEscritor >> GLOBALID_PRODUCTOS;
		lectorEscritor.close();
		MessageBox(NULL, "Carga con exito", "Guardar ID", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo cargar el ID", "Error de carga", MB_ICONWARNING);
		return;
	}
}

void saveGlobalId() {
	lectorEscritor.open("GlobalId.txt", ios::out, ios::trunc);
	if (lectorEscritor.is_open()) {
		lectorEscritor << GLOBAL_USUARIO_ID << endl;
		lectorEscritor << GLOBALID_PRODUCTOS;
		lectorEscritor.close();
		MessageBox(NULL, "Guardado exitoso", "Guardar", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "Error de guardado", MB_ICONWARNING);
		return;
	}
}

void save(USUARIO* pOrigen) {
	lectorEscritor.open("Usuarios.bin", ios::out | ios::trunc | ios::binary);
	if (lectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			lectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(USUARIO));
			pOrigen = pOrigen->sig;
		}
		lectorEscritor.close();
		MessageBox(NULL, "Guardado exitoso", "Guardar", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "Error de guardado", MB_ICONASTERISK);
		return;
	}
}

void loadUser() {
	lectorEscritor.open("Usuarios.bin", ios::binary | ios::in | ios::ate);
	if (lectorEscritor.is_open()) {
		int totalCharacterUsers = lectorEscritor.tellg();
		if (totalCharacterUsers < 1) {
			MessageBox(NULL, "Archivo vacio", "Error de carga", MB_ICONASTERISK);
			return;
		}
		for (int i = 0; i < totalCharacterUsers / sizeof(USUARIO); i++) {
			if (origen == NULL) {
				USUARIO* temp = new USUARIO;
				origen = new USUARIO;
				lectorEscritor.seekg(i * sizeof(USUARIO));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(USUARIO));
				origen->sNombre = temp->sNombre;
				origen->sPassword = temp->sPassword;
				origen->IDUsuario = temp->IDUsuario;
				origen->sNombreVendedor = temp->sNombreVendedor;
				origen->sNombreEmpresa = temp->sNombreEmpresa;
				strcpy(origen->foto, temp->foto);
				origen->control = temp->control;
				origen->sig = NULL;
				origen->prev = NULL;
				auxiliar = origen;
				delete reinterpret_cast<char*>(temp);
				
			}
			else {
				while (auxiliar->sig != NULL) {
					auxiliar = auxiliar->sig;
				}
				USUARIO* temp = new USUARIO;
				auxiliar->sig = new USUARIO;
				lectorEscritor.seekg(i * sizeof(USUARIO));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(USUARIO));
				auxiliar->sig->prev = auxiliar;
				auxiliar = auxiliar->sig;
				auxiliar->sNombre = temp->sNombre;
				auxiliar->sPassword = temp->sPassword;
				auxiliar->IDUsuario = temp->IDUsuario;
				auxiliar->sNombreVendedor = temp->sNombreVendedor;
				auxiliar->sNombreEmpresa = temp->sNombreEmpresa;
				strcpy(auxiliar->foto, temp->foto);
				auxiliar->control = temp->control;
				auxiliar->sig = NULL;
				auxiliar = origen;
				delete reinterpret_cast<char*>(temp);
				
			}
		}
		lectorEscritor.close();
		MessageBox(NULL, "Carga exitosa", "Cargar", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "Error de carga", MB_ICONASTERISK);
		return;
	}
}

void saveProductos(productos* pOrigen) {
	lectorEscritor.open("Producto.bin", ios::binary | ios::out | ios::trunc);
	if (lectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			lectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(productos));
			pOrigen = pOrigen ->sig;
		}
		lectorEscritor.close();
		MessageBox(NULL, "Productos guardados exitosamente", "Guardado de productos", MB_OK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo de productos", "Error de guardado de productos", MB_ICONERROR);
		return;

	}
}

void loadProductos() {
	lectorEscritor.open("Producto.bin", ios::binary | ios::in | ios::ate);
	if (lectorEscritor.is_open()) {
		int totalChar = lectorEscritor.tellg();
		if (totalChar < 1) {
			MessageBox(NULL, "Archivo de producto vacio", "Error de carga", MB_ICONERROR);
			lectorEscritor.close();
			return;
		}
		for (int i = 0; i < totalChar / sizeof(productos); i++) {
			if (origenProducto == NULL) {
				productos* temp = new productos;
				origenProducto = new productos;
				lectorEscritor.seekg(i * sizeof(productos));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(productos));
				origenProducto->sNombreProducto.append(temp->sNombreProducto);
				origenProducto->IDUsuario = temp->IDUsuario;
				origenProducto->IDProductos = temp->IDProductos;
				strcpy(origenProducto->foto1, temp->foto1);
				strcpy(origenProducto->foto2, temp->foto2);
				origenProducto->sCantidad.append(temp->sCantidad);
				origenProducto->sCodigo.append(temp->sCodigo);
				origenProducto->sMarca.append(temp->sMarca);
				origenProducto->sPrecio.append(temp->sPrecio);
				origenProducto->sDescripcion.append(temp->sDescripcion);
				origenProducto->IDProductos = temp->IDProductos;
				origenProducto->prev = NULL;
				origenProducto->sig = NULL;
				auxiliarProducto = origenProducto;
				delete reinterpret_cast<char*>(temp);
			}
			else {
				while (auxiliarProducto->sig != NULL)
				auxiliarProducto = auxiliarProducto->sig;
				productos* temp = new productos;
				auxiliarProducto->sig = new productos;
				lectorEscritor.seekg(i * sizeof(productos));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(productos));
				auxiliarProducto->sig->prev = auxiliarProducto;
				auxiliarProducto = auxiliarProducto->sig;
				auxiliarProducto->IDUsuario = temp->IDUsuario;
				auxiliarProducto->IDProductos = temp->IDProductos;
				auxiliarProducto->sNombreProducto.append(temp->sNombreProducto);
				auxiliarProducto->sCantidad.append(temp->sCantidad);
				auxiliarProducto->sCodigo.append(temp->sCodigo);
				auxiliarProducto->sMarca.append(temp->sMarca);
				auxiliarProducto->sPrecio.append(temp->sPrecio);
				auxiliarProducto->sDescripcion.append(temp->sDescripcion);
				strcpy(auxiliarProducto->foto1, temp->foto1);
				strcpy(auxiliarProducto->foto2, temp->foto2);
				auxiliarProducto->IDProductos = temp->IDProductos;
				auxiliarProducto->sig = NULL;
				auxiliarProducto = origenProducto;
				delete reinterpret_cast<char*>(temp);
			}
		}
		lectorEscritor.close();
		MessageBox(NULL, "Carga de productos con exito", "Carga de productos", MB_OK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo de productos", "Error de carga", MB_ICONERROR);
		return;
	}
}

void saveEnvios(envios* pOrigen) {
	lectorEscritor.open("Envios.bin", ios::binary | ios::out | ios::trunc);
	if (lectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			lectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(envios));
			pOrigen = pOrigen->sig;
		}
		lectorEscritor.close();
		MessageBox(NULL, "Envios guardados exitosamente", "Guardado de envios", MB_OK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo de envios", "Error de guardado de envios", MB_ICONERROR);
		return;

	}
}

void loadEnvios() {
	lectorEscritor.open("Envios.bin", ios::binary | ios::in | ios::ate);
	if (lectorEscritor.is_open()) {
		int totalChar = lectorEscritor.tellg();
		if (totalChar < 1) {
			MessageBox(NULL, "Archivo de envios vacio", "Error de carga", MB_ICONERROR);
			lectorEscritor.close();
			return;
		}
		for (int i = 0; i < totalChar / sizeof(envios); i++) {
			if (origenEnvios == NULL) {
				envios* temp = new envios;
				origenEnvios = new envios;
				lectorEscritor.seekg(i * sizeof(envios));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(envios));				
				origenEnvios->IDUsuario = temp->IDUsuario;
				origenEnvios->IDProductos = temp->IDProductos;
				origenEnvios->IDEnvios = temp->IDEnvios;
				origenEnvios->sNombreProducto.append(temp->sNombreProducto);
				origenEnvios->sCantidad.append(temp->sCantidad);
				origenEnvios->sCalle.append(temp->sCalle);
				origenEnvios->sColonia.append(temp->sColonia);
				origenEnvios->sCiudad.append(temp->sCiudad);
				origenEnvios->sEstado.append(temp->sEstado);
				origenEnvios->sMensaje.append(temp->sMensaje);
				origenEnvios->sDia = temp->sDia;  // DE AQUI
				origenEnvios->sMes = temp->sMes;
				origenEnvios->sAnio = temp->sAnio;
				origenEnvios->IDFecha = temp->IDFecha;
				origenEnvios->IDProductos = temp->IDProductos;
				origenEnvios->prev = NULL;
				origenEnvios->sig = NULL;
				auxiliarEnvios = origenEnvios;
				delete reinterpret_cast<char*>(temp);
			}

			else {
				while (auxiliarEnvios->sig != NULL)
				auxiliarEnvios = auxiliarEnvios->sig;
				envios* temp = new envios;
				auxiliarEnvios->sig = new envios;
				lectorEscritor.seekg(i * sizeof(envios));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(envios));
				auxiliarEnvios->sig->prev = auxiliarEnvios;
				auxiliarEnvios = auxiliarEnvios->sig;
				auxiliarEnvios->IDUsuario = temp->IDUsuario;
				auxiliarEnvios->IDProductos = temp->IDProductos;
				auxiliarEnvios->IDEnvios = temp->IDEnvios;
				auxiliarEnvios->sNombreProducto.append(temp->sNombreProducto);
				auxiliarEnvios->sCantidad.append(temp->sCantidad);
				auxiliarEnvios->sCalle.append(temp->sCalle);
				auxiliarEnvios->sColonia.append(temp->sColonia);
				auxiliarEnvios->sCiudad.append(temp->sCiudad);
				auxiliarEnvios->sEstado.append(temp->sEstado);
				auxiliarEnvios->sMensaje.append(temp->sMensaje);
				auxiliarEnvios->sDia = temp->sDia;  //DE AQUI
				auxiliarEnvios->sMes = temp->sMes;
				auxiliarEnvios->sAnio = temp->sAnio;
				auxiliarEnvios->IDFecha = temp->IDFecha;
				auxiliarEnvios->IDProductos = temp->IDProductos;
				auxiliarEnvios->sig = NULL;
				auxiliarEnvios = origenEnvios;
				delete reinterpret_cast<char*>(temp);
			}
		}
		lectorEscritor.close();
		MessageBox(NULL, "Carga de productos con exito", "Carga de productos", MB_OK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo de envios", "Error de carga", MB_ICONERROR);
		return;
	}
}

BOOL CALLBACK funcionPrincipal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		case WM_INITDIALOG: {
			
			hTxtLoginUserName = GetDlgItem(hwnd, TXT_USUARIO);
			hTxtLoginPassword = GetDlgItem(hwnd, TXT_CONTRASENA);
		}
		 break;
		case WM_COMMAND: {
			switch (LOWORD(wparam)) {
				case BTN_INICIAR_SESION: {
					string sNombre = getText(hTxtLoginUserName);
					string sPassword = getText(hTxtLoginPassword);
					if (sNombre.compare("") == 0 || sPassword.compare("") == 0) {
						MessageBox(NULL, "No se llenaron todos los campos", "No login", MB_ICONEXCLAMATION);
						break;
					}
					if (origen != NULL) {
						bool found = true;
						while (auxiliar != NULL) {
							if (auxiliar->sNombre.compare(sNombre) == 0 && auxiliar->sPassword.compare(sPassword) == 0) {
								break;
							}
							if (auxiliar->sig == NULL)
							found = false;
							auxiliar = auxiliar->sig;
						}
						if (found) {
							userAccess = auxiliar;
							if (userAccess->control == false) {
								HWND HInfoVendedor = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INF_VENDEDOR), NULL, funcionInfoVendedor);
								ShowWindow(HInfoVendedor, SW_SHOW);
								ShowWindow(hwnd, SW_HIDE);								
							}
							else if (userAccess->control == true) {

								HWND HInfoVendedor = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INFO_VENDEDOR), NULL, funcionInfoVendedorStatic);
								ShowWindow(HInfoVendedor, SW_SHOW);
								ShowWindow(hwnd, SW_HIDE);
								break;
							}						
						}
						else {
							auxiliar = origen;
							MessageBox(NULL, "Ningun usuario coincide con los datos", "Error de inicio", MB_ICONWARNING);							
						}
					}
					else {
						MessageBox(NULL, "No hay ningun usuario registrado", "Error de inicio", MB_ICONASTERISK);
						break;
					}

				}
				break;

				case BTN_REGISTRAR_USUARIO: {
					HWND HRegistrar = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_REGISTRO), NULL, funcionRegistro);
					ShowWindow(HRegistrar, SW_SHOW);
					ExitInicioSesion = false;
					DestroyWindow(hwnd);
				}
				  break;
				case BTN_SALIR_INICIO_SESION: {

					int retornoSiNo = MessageBox(hwnd, "¿Esta seguro que quiere salir?", "Aviso", MB_ICONQUESTION | MB_YESNO);
					if (retornoSiNo == 6) {

						ExitInicioSesion = true;
						DestroyWindow(hwnd);
					}
					else {
						if (retornoSiNo == 7)
						break;
					}
				}
				break;
			}
		}
		 break;
		case WM_DESTROY:

			if (ExitInicioSesion)
			PostQuitMessage(9);
		break;
	}
	return FALSE;
}

BOOL CALLBACK funcionRegistro(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {

		case WM_INITDIALOG: {
			hTxtRegisterUserName = GetDlgItem(hwnd, TXT_REGISTRAR_USUARIO);
			hTxtRegisterPassword = GetDlgItem(hwnd, TXT_CONTRASENA_REGISTRAR);
		}
					  break;
		case WM_COMMAND: {
			switch (LOWORD(wparam)) {
				case BTN_AGREGAR_USUARIO: {

					auxiliar = origen;
					string sNombre = getText(hTxtRegisterUserName);
					string sPassword = getText(hTxtRegisterPassword);
					int iTamanio = GetWindowTextLength(hTxtRegisterUserName);
					bool validarCorrecto = false;

					if (sNombre.compare("") == 0 || sPassword.compare("") == 0) {
						MessageBox(NULL, "No se han llenado todos los campos", "Error", MB_ICONASTERISK);
						validarCorrecto = true;
					}
					else if (fValidarSoloLetras(sNombre, iTamanio)) {
						MessageBox(hwnd, "Ingresar solamente letras en el nombre", "Error", MB_ICONEXCLAMATION);
						validarCorrecto = true;

					}

					else if (origen != NULL) {
						bool found = true;
						while (auxiliar->sNombre.compare(sNombre) != 0) {
							if (auxiliar->sig == NULL) {
								found = false;
								validarCorrecto = false;
								break;
							}
							auxiliar = auxiliar->sig;
						}
						if (found == true) {
							MessageBox(NULL, "Ya existe ese usuario", "Error de registro", MB_ICONWARNING);
							auxiliar = origen;
							validarCorrecto = true;
							break;
						}				
					}
					if (validarCorrecto == false) {
						if (origen == NULL) {
							origen = new USUARIO;						
							origen->sNombre.append(sNombre);
							origen->sPassword.append(sPassword);
							origen->IDUsuario = GLOBAL_USUARIO_ID++;
							origen->sig = NULL;
							origen->prev = NULL;
							auxiliar = origen;
						
						}
						else {
							while (auxiliar->sig != NULL)
							auxiliar = auxiliar->sig;
							auxiliar->sig = new USUARIO;
							auxiliar->sig->prev = auxiliar;
							auxiliar = auxiliar->sig;
							auxiliar->IDUsuario = GLOBAL_USUARIO_ID++;
							auxiliar->sNombre.append(sNombre);
							auxiliar->sPassword.append(sPassword);
							auxiliar->sig = NULL;
							auxiliar = origen;
						}
						save(auxiliar);
						saveGlobalId();					
						auxiliar = origen;
					
					

						HWND hPrincipal = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INICIO_SESION), NULL, funcionPrincipal);
						ShowWindow(hPrincipal, SW_SHOW);
						ShowWindow(hwnd, SW_HIDE);
					}
				}
				break;
				case BTN_CANCELAR_ENREGISTRO: {
					HWND hPrincipal = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INICIO_SESION), NULL, funcionPrincipal);
					ShowWindow(hPrincipal, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
				}
				break;
				case BTN_SALIR_REGISTRO: {

					int retornoSiyNo = MessageBox(hwnd, "¿Esta seguro que quiere salir?", "Aviso", MB_ICONQUESTION | MB_YESNO);
					if (retornoSiyNo == 6) {

						save(auxiliar);

						ExitRegistro = true;
						DestroyWindow(hwnd);
					}
					else {
						if (retornoSiyNo == 7)
						break;
					}
				}
				break;
			}
		}
		break;
		case WM_DESTROY:
			if (ExitRegistro)
			PostQuitMessage(9);
		break;
	}
	return FALSE;

}

BOOL CALLBACK funcionInfoVendedor(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {	
	switch (msg) {
		case WM_INITDIALOG: {
			fInstertarMenu(hwnd);
		}
		 break;
	
		case WM_COMMAND: {
			
			switch (LOWORD(wparam)) {
				case BTN_SUBIR_IMAGEN: {
					OPENFILENAME ofn;
					ZeroMemory(&ofn, sizeof(OPENFILENAME));
					char RutaImagen[MAX_PATH] = "";
					ofn.hwndOwner = hwnd;
					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.lpstrFile = RutaImagen;
					ofn.nMaxFile = MAX_PATH;
					ofn.lpstrDefExt = "txt";
					ofn.Flags = OFN_EXPLORER||OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
					ofn.lpstrFilter = "solamente imagenes (.bmp)\0*.bmp\0";

					MessageBox(hwnd, "Si llegue", "si", MB_OK);
					if (GetOpenFileName(&ofn)) {
						sRutaImagen = RutaImagen;
						HWND hImagen = GetDlgItem(hwnd, PB_VENDEDOR);
						HBITMAP hImagenBMP = (HBITMAP)LoadImage(NULL, RutaImagen, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
						SendMessage(hImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagenBMP);
					}
					else {
						sRutaImagen == "0";
						MessageBox(hwnd, "Falto agregar la imagen", "Error", MB_ICONERROR);
					}
				}
				break;
				case ID_ALTA_PRODUCTOS: {
					HWND hPrimera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ALTA_PRODUCTOS), NULL, funcionAltaProducto);
					ShowWindow(hPrimera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				
				case ID_BAJA_PRODUCTOS: {
					HWND hTercera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
					ShowWindow(hTercera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				
				case ID_NUEVO_ENVIO: {
					HWND hQuinta = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_NUEVO_ENVIO), NULL, funcionNuevoEnvio);
					ShowWindow(hQuinta, SW_SHOW);
					SetTimer(hQuinta, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				 break;
				
				case ID_ENVIOS_ELIMINARENVIO: {
					HWND hSeptima = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionEliminarEnvio);
					ShowWindow(hSeptima, SW_SHOW);
					SetTimer(hSeptima, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_SALIR:{
					
					int retornoSioNo = MessageBox(hwnd, "¿Esta seguro que quiere salir?", "Aviso", MB_ICONQUESTION|MB_YESNO);
					if (retornoSioNo==6) {
						save(auxiliar);
						saveProductos(auxiliarProducto);
						saveEnvios(auxiliarEnvios);
						exitWindowInfoVendedor = true;
						DestroyWindow(hwnd);
					}
					else {
						if (retornoSioNo == 7)
						break;
					}


				}
				 break;

				case BTN_CONFIRMAR_INFO_VENDEDOR: {
					HWND hConfirmarInfoVendedor = GetDlgItem(hwnd, TXT_NOMBRE_USUARIO);
					string sConfirmarInfoVendedor = getText(hConfirmarInfoVendedor);

					HWND hEmpresaInfoVendedor = GetDlgItem(hwnd, TXT_EMPRESA);
					string sEmpresaInfoVendedor=getText(hEmpresaInfoVendedor);

					bool contador = false;
					int iConfirmarInfoVendedor = GetWindowTextLength(hConfirmarInfoVendedor);
					
					if (sConfirmarInfoVendedor.compare("") == 0 || sEmpresaInfoVendedor.compare("") == 0 || sRutaImagen.compare("0") == 0) {
						MessageBox(NULL, "Falto llenar informacion", "ERROR", MB_ICONASTERISK);
						contador = true;
					}
					else if (fValidarSoloLetras(sConfirmarInfoVendedor, iConfirmarInfoVendedor)) {
						MessageBox(hwnd, "Ingresar solo letras en el nombre de vendedor", "ERROR", MB_ICONEXCLAMATION);
						contador = true;
					}

					userAccess->sNombreVendedor = sConfirmarInfoVendedor;
					userAccess->sNombreEmpresa = sEmpresaInfoVendedor;
					strcpy(userAccess-> foto, sRutaImagen.c_str());
					userAccess->control = true;

					if (contador == false) {
						HWND hTwelfthWindow = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INFO_VENDEDOR), NULL, funcionInfoVendedorStatic);
						ShowWindow(hTwelfthWindow, SW_SHOW);
						ShowWindow(hwnd, SW_HIDE);
					}

				}
				break;
				
				
			}
		}	
		case WM_DESTROY:
			if (exitWindowInfoVendedor)
			PostQuitMessage(9);
		break;
	}
	return FALSE;
}

BOOL CALLBACK funcionInfoVendedorStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		case WM_INITDIALOG: {
			fInstertarMenu(hwnd);
			loadUser();

			HWND hVendedorStatic = GetDlgItem(hwnd, LBL_VENDEDORSTATIC);
			HWND hEmpresaStatic = GetDlgItem(hwnd, LBL_EMPRESASTATIC);
			HWND hImagenStatic = GetDlgItem(hwnd, PB_USUARIO);
			HBITMAP hImagenBMPStatic = (HBITMAP) LoadImage(NULL,userAccess->foto, IMAGE_BITMAP,70,70,LR_LOADFROMFILE);//HBITMAP ANTES DEL LOADIMAGE PARA INDICAR QUE SOLO QUIERO CARGAR BITMAP.

			SendMessage(hImagenStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagenBMPStatic);
			SetWindowText(hVendedorStatic, userAccess->sNombreVendedor.c_str());
			SetWindowText(hEmpresaStatic, userAccess->sNombreEmpresa.c_str());
		}
		 break;

		case WM_COMMAND: {
			switch (LOWORD(wparam)) {
				case BTN_EDIT_INFO_VENDEDOR: {
					HWND hEditarinfo = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INF_VENDEDOR_EDIT), NULL, funcionInfoVendedorEdit);
					ShowWindow(hEditarinfo, SW_SHOW);
					DestroyWindow(hwnd);
				}
				break;
				case ID_ALTA_PRODUCTOS: {
					HWND hPrimera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ALTA_PRODUCTOS), NULL, funcionAltaProducto);
					ShowWindow(hPrimera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				 break;
				
				case ID_BAJA_PRODUCTOS: {
					HWND hTercera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
					ShowWindow(hTercera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				
				case ID_NUEVO_ENVIO: {
					HWND hQuinta = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_NUEVO_ENVIO), NULL, funcionNuevoEnvio);
					ShowWindow(hQuinta, SW_SHOW);
					SetTimer(hQuinta, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				
				case ID_ENVIOS_ELIMINARENVIO: {
					HWND hSeptima = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionEliminarEnvio);
					ShowWindow(hSeptima, SW_SHOW);
					SetTimer(hSeptima, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_SALIR: {

					int retornoSioNo = MessageBox(hwnd, "¿Esta seguro que quiere salir?", "Aviso", MB_ICONQUESTION | MB_YESNO);
					if (retornoSioNo == 6) {
						save(auxiliar);
						saveProductos(auxiliarProducto);
						saveEnvios(auxiliarEnvios);
						exitWindowInfoVendedorStatic = true;
						DestroyWindow(hwnd);
					}
					else {
						if (retornoSioNo == 7)
						break;
					}


				}
				break;
			}
		}
		break;
		case WM_DESTROY: 
			if (exitWindowInfoVendedorStatic)
			PostQuitMessage(9);
			break;
		
	}


	return FALSE;
}

BOOL CALLBACK funcionInfoVendedorEdit(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		case WM_INITDIALOG: {
			fInstertarMenu(hwnd);

			HWND hVendedorStatic = GetDlgItem(hwnd, TXT_NOMBRE_EDIT_INFO); //DE AQUÍ
			HWND hEmpresaStatic = GetDlgItem(hwnd, TXT_EMPRESA_EDIT_INFO);
			HWND hImagenStatic = GetDlgItem(hwnd, PB_EDIT_IMAGEN_INFO);
			HBITMAP hBmImagen = (HBITMAP)LoadImage(NULL, userAccess->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
			sRutaImagen = userAccess->foto; //IMPORTANTEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
			SendMessage(hImagenStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmImagen);
			SetWindowText(hVendedorStatic, userAccess->sNombreVendedor.c_str());
			SetWindowText(hEmpresaStatic, userAccess->sNombreEmpresa.c_str());
		}
		break;
		case WM_COMMAND: {
			if (LOWORD(wparam) == BTN_CARGAR_IMAGEN_EDIT_INFO && HIWORD(wparam) == BN_CLICKED) {
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				char RutaImagen[MAX_PATH] = "";
				ofn.hwndOwner = hwnd;
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.lpstrFile = RutaImagen;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrDefExt = "txt";
				ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
				ofn.lpstrFilter = "solamente imagenes (.bmp)\0*.bmp\0";

				if (GetOpenFileName(&ofn)) {
					sRutaImagen = RutaImagen;

					HWND hImagen = GetDlgItem(hwnd, PB_EDIT_IMAGEN_INFO);
					HBITMAP hImagenBMP = (HBITMAP)LoadImage(NULL, sRutaImagen.c_str(), IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);//de string a char c.str()
					SendMessage(hImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagenBMP);
				}
				else {
					sRutaImagen == "0";
					MessageBox(hwnd, "Falto agregar la imagen", "Error", MB_ICONERROR);
				}
			}
			switch (LOWORD(wparam)) {
				case BTN_CONFIRMAR_EDICION_INFO: {
					HWND hConfirmarInfoVendedorE = GetDlgItem(hwnd, TXT_NOMBRE_EDIT_INFO);
					string sConfirmarInfoVendedorE = getText(hConfirmarInfoVendedorE);

					HWND hEmpresaInfoVendedor = GetDlgItem(hwnd, TXT_EMPRESA_EDIT_INFO);
					string sEmpresaInfoVendedor = getText(hEmpresaInfoVendedor);

					bool contador = false;
					int iConfirmarInfoVendedor = GetWindowTextLength(hConfirmarInfoVendedorE);

					if (sConfirmarInfoVendedorE.compare("") == 0 || sEmpresaInfoVendedor.compare("") == 0 || sRutaImagen.compare("0") == 0) {
						MessageBox(NULL, "Falto llenar informacion", "ERROR", MB_ICONASTERISK);
						contador = true;
					}
					else if (fValidarSoloLetras(sConfirmarInfoVendedorE, iConfirmarInfoVendedor)) {
						MessageBox(hwnd, "Ingresar solo letras en el nombre de vendedor", "ERROR", MB_ICONEXCLAMATION);
						contador = true;
					}
					userAccess->sNombreVendedor = sConfirmarInfoVendedorE; //DE AQUÍ
					userAccess->sNombreEmpresa = sEmpresaInfoVendedor;
					strcpy(userAccess->foto, sRutaImagen.c_str());
					userAccess->control = true; //HASTA AQUÍ

					if (contador == false) {

						HWND hInfoVendedorStatico = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INFO_VENDEDOR), NULL, funcionInfoVendedorStatic);
						ShowWindow(hInfoVendedorStatico, SW_SHOW);
						DestroyWindow(hwnd);
					}					
				}
				break;
				case ID_ALTA_PRODUCTOS: {
					HWND hPrimera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ALTA_PRODUCTOS), NULL, funcionAltaProducto);
					ShowWindow(hPrimera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				
				case ID_BAJA_PRODUCTOS: {
					HWND hTercera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
					ShowWindow(hTercera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				
				case ID_NUEVO_ENVIO: {
					HWND hQuinta = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_NUEVO_ENVIO), NULL, funcionNuevoEnvio);
					ShowWindow(hQuinta, SW_SHOW);
					SetTimer(hQuinta, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				
				case ID_ENVIOS_ELIMINARENVIO: {
					HWND hSeptima = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionEliminarEnvio);
					ShowWindow(hSeptima, SW_SHOW);
					SetTimer(hSeptima, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_SALIR: {
	
					int retornoSioNo = MessageBox(hwnd, "¿Esta seguro que quiere salir?", "Aviso", MB_ICONQUESTION | MB_YESNO);
					if (retornoSioNo == 6) {
						save(auxiliar);
						saveProductos(auxiliarProducto);
						saveEnvios(auxiliarEnvios);
						exitWindowInfoVendedorEdit = true;
						DestroyWindow(hwnd);
					}
					else {
						if (retornoSioNo == 7)
						break;
					}


				}
				break;
			}
		}
	    break;
		case WM_DESTROY:
			
			if (exitWindowInfoVendedorEdit)
				PostQuitMessage(9);
			break;

	}
	return FALSE;
}

BOOL CALLBACK funcionAltaProducto(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		case WM_INITDIALOG: {
			fInstertarMenu(hwnd);
			hTxtNombreProducto = GetDlgItem(hwnd, TXT_NOMBRE_PROD);
			hTxtCantidadProd = GetDlgItem(hwnd, TXT_CANT_PROD);
			hTxtCodigoProd = GetDlgItem(hwnd, TXT_CODE_PROD);
			hTxtMarcaProd = GetDlgItem(hwnd, TXT_MARCA_PROD);
			hTxtDescripcionProd = GetDlgItem(hwnd, TXT_DESC_PROD);
			hTxtPrecioProd = GetDlgItem(hwnd, TXT_PRECIO_PROD);

			HWND hImagenStatic = GetDlgItem(hwnd, PB_FOTOENALTA);
			HBITMAP hImagenBMPStatic = (HBITMAP)LoadImage(NULL, userAccess->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);//HBITMAP ANTES DEL LOADIMAGE PARA INDICAR QUE SOLO QUIERO CARGAR BITMAP.
			SendMessage(hImagenStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagenBMPStatic);

			HWND hVendedorStatic = GetDlgItem(hwnd, LBL_NOMBREENALTA);
			SetWindowText(hVendedorStatic, userAccess->sNombreVendedor.c_str());

		}
		 break;
		case WM_COMMAND: {

			if (LOWORD(wparam) == BTN_IMAGEN1PROD && HIWORD(wparam) == BN_CLICKED) {
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				char RutaImagen[MAX_PATH] = "";
				ofn.hwndOwner = hwnd;
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.lpstrFile = RutaImagen;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrDefExt = "txt";
				ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
				ofn.lpstrFilter = "solamente imagenes (.bmp)\0*.bmp\0";

				if (GetOpenFileName(&ofn)) {
					sRutaImagen = RutaImagen;

					HWND hImagen = GetDlgItem(hwnd, PB_IMAGEN1PROD);
					HBITMAP hImagenBMP = (HBITMAP)LoadImage(NULL, RutaImagen, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);//de string a char c.str()
					SendMessage(hImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagenBMP);
				}
				else {
					sRutaImagen == "0";
					MessageBox(hwnd, "Falto agregar la imagen", "Error", MB_ICONERROR);
				}
			}
			if (LOWORD(wparam) == BTN_IMAGEN2PROD && HIWORD(wparam) == BN_CLICKED) {
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				char RutaImagen2[MAX_PATH] = "";
				ofn.hwndOwner = hwnd;
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.lpstrFile = RutaImagen2;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrDefExt = "txt";
				ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
				ofn.lpstrFilter = "solamente imagenes (.bmp)\0*.bmp\0";

				if (GetOpenFileName(&ofn)) {
					sRutaImagen2 = RutaImagen2;

					HWND hImagen = GetDlgItem(hwnd, PB_IMAGEN2PROD);
					HBITMAP hImagenBMP = (HBITMAP)LoadImage(NULL, RutaImagen2, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);//de string a char c.str()
					SendMessage(hImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagenBMP);
				}
				else {
					sRutaImagen2 == "0";
					MessageBox(hwnd, "Falto agregar la imagen", "Error", MB_ICONERROR);
				}
			}

			switch (LOWORD(wparam)) {


				case BTN_CANCELARPROD: {

					sRutaImagen = "0";
					sRutaImagen2 = "0";

					HWND hPrimera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
					SetTimer(hPrimera, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hPrimera, SW_SHOW);
					DestroyWindow(hwnd);
				}
				case BTN_CONFIRM_PROD: {
					string sNombreProd = getText(hTxtNombreProducto);
					string sCantidadProd = getText(hTxtCantidadProd);
					string sCodigoProd = getText(hTxtCodigoProd);
					string sMarcaProd = getText(hTxtMarcaProd);
					string sDescProd = getText(hTxtDescripcionProd);
					string sPrecioProd = getText(hTxtPrecioProd);
					int iTamanio = GetWindowTextLength(hTxtCantidadProd);
					int iTamanio1 = GetWindowTextLength(hTxtCodigoProd);
					int iTamanio2 = GetWindowTextLength(hTxtPrecioProd);
					bool validarCorrecto = false;

					if (sNombreProd.compare("") == 0 || sCantidadProd.compare("") == 0 || sCodigoProd.compare("") == 0 || sMarcaProd.compare("") == 0 || sDescProd.compare("") == 0 || sPrecioProd.compare("") == 0 || sRutaImagen.compare("0") == 0 || sRutaImagen2.compare("0") == 0) {
						MessageBox(NULL, "No se llenaron todos los campos", "No alta", MB_ICONEXCLAMATION);

						validarCorrecto = true;
					}
					else if (fValidarSoloNumeros(sCantidadProd, iTamanio)) {
						MessageBox(hwnd, "Ingresar solamente numeros en la cantidad", "Error", MB_ICONEXCLAMATION);
						validarCorrecto = true;
					}
					else if (fValidarSoloNumeros(sCodigoProd, iTamanio1)) {
						MessageBox(hwnd, "Ingresar solamente numeros en el codigo", "Error", MB_ICONEXCLAMATION);
						validarCorrecto = true;

					}
					else if (fValidarSoloNumeros(sPrecioProd, iTamanio2)) {
						MessageBox(hwnd, "Ingresar solamente numeros en el precio", "Error", MB_ICONEXCLAMATION);
						validarCorrecto = true;

					}

					else if (origenProducto != NULL) {
						bool found = true;
						while (auxiliarProducto->sNombreProducto.compare(sNombreProd) != 0) {
							if (auxiliarProducto->sig == NULL) {
								found = false;
								validarCorrecto = false;
								break;
							}
							auxiliarProducto = auxiliarProducto->sig;
						}
						if (found == true) {
							MessageBox(NULL, "Ya existe ese producto", "Error de alta", MB_ICONWARNING);
							auxiliarProducto = origenProducto;
							validarCorrecto = true;
							break;
						}
					}
					if (validarCorrecto == false) {
						if (origenProducto == NULL) {
							origenProducto = new productos;
							origenProducto->sNombreProducto.append(sNombreProd);
							origenProducto->sCantidad = sCantidadProd;
							origenProducto->sCodigo = sCodigoProd;
							origenProducto->sMarca.append(sMarcaProd);
							origenProducto->sPrecio.append(sPrecioProd);
							origenProducto->sDescripcion.append(sDescProd);
							strcpy(origenProducto->foto1, sRutaImagen.c_str());
							strcpy(origenProducto->foto2, sRutaImagen2.c_str());
							origenProducto->IDUsuario = userAccess->IDUsuario;
							origenProducto->IDProductos = GLOBALID_PRODUCTOS++;
							origenProducto->prev = NULL;
							origenProducto->sig = NULL;
							auxiliarProducto = origenProducto;
						}
						else {
							while (auxiliarProducto->sig != NULL)
							auxiliarProducto = auxiliarProducto->sig;
							auxiliarProducto->sig = new productos;
							auxiliarProducto->sig->prev = auxiliarProducto;
							auxiliarProducto = auxiliarProducto->sig;
							auxiliarProducto->IDUsuario = userAccess->IDUsuario;
							auxiliarProducto->IDProductos = GLOBALID_PRODUCTOS++;
							auxiliarProducto->sNombreProducto.append(sNombreProd);
							auxiliarProducto->sCantidad = sCantidadProd;
							auxiliarProducto->sCodigo = sCodigoProd;
							auxiliarProducto->sMarca.append(sMarcaProd);
							auxiliarProducto->sPrecio.append(sPrecioProd);
							auxiliarProducto->sDescripcion.append(sDescProd);
							strcpy(auxiliarProducto->foto1, sRutaImagen.c_str());
							strcpy(auxiliarProducto->foto2, sRutaImagen2.c_str());
							auxiliarProducto->sig = NULL;
							auxiliarProducto = origenProducto;
						}
						saveGlobalId();
						auxiliarProducto = origenProducto;
						sRutaImagen = "0";
						sRutaImagen2 = "0";

						if (validarCorrecto == false) {

						
							HWND hBaja = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
							SetTimer(hBaja, MI_TIMER, 1000, (TIMERPROC)NULL);
							ShowWindow(hBaja, SW_SHOW);
							DestroyWindow(hwnd);
						}
					}

				}break;
				case ID_INFO_VENDEDOR: {

					sRutaImagen = "0";
					sRutaImagen2 = "0";

					HWND hOctava = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INFO_VENDEDOR), NULL, funcionInfoVendedorStatic);
					ShowWindow(hOctava, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				 break;

				case ID_BAJA_PRODUCTOS: {

					sRutaImagen = "0";
					sRutaImagen2 = "0";

					HWND hTercera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
					ShowWindow(hTercera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				 break;

				case ID_NUEVO_ENVIO: {

					sRutaImagen = "0";
					sRutaImagen2 = "0";

					HWND hQuinta = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_NUEVO_ENVIO), NULL, funcionNuevoEnvio);
					ShowWindow(hQuinta, SW_SHOW);
					SetTimer(hQuinta, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}	
				break;

				case ID_ENVIOS_ELIMINARENVIO: {

					sRutaImagen = "0";
					sRutaImagen2 = "0";

					HWND hSeptima = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionNuevoEnvio);
					ShowWindow(hSeptima, SW_SHOW);
					SetTimer(hSeptima, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_SALIR: {

					int retornoSioNo = MessageBox(hwnd, "¿Esta seguro que quiere salir?", "Aviso", MB_ICONQUESTION | MB_YESNO);
					if (retornoSioNo == 6) {

						sRutaImagen = "0";
						sRutaImagen2 = "0";

						save(auxiliar);
						saveProductos(auxiliarProducto);
						saveEnvios(auxiliarEnvios);
						exitWindowAltaProducto = true;
						DestroyWindow(hwnd);
					}
					else {
						if (retornoSioNo == 7)
						break;
					}


				}
				break;

			}

		}
		break;
		case WM_DESTROY:
			if (exitWindowAltaProducto)
			PostQuitMessage(9);
		break;
	}
	return FALSE;
}

BOOL CALLBACK funcionModProducto(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		case WM_INITDIALOG: {
			fInstertarMenu(hwnd);
			hTxtModProdNombre = GetDlgItem(hwnd, TXT_MOD_NOM_PROD);
			hTxtModProdProductos = GetDlgItem(hwnd, TXT_MOD_CANT_PROD);
			hTxtModProdCodigo = GetDlgItem(hwnd, TXT_MOD_CODE_PROD);
			hTxtModProdMarca = GetDlgItem(hwnd, TXT_MOD_MARCA_PROD);
			hTxtModProdPrecio = GetDlgItem(hwnd, TXT_MOD_PRECIO_PROD);
			hTxtModProdDescripcion = GetDlgItem(hwnd, TXT_MOD_DESC_PROD);
			
			HWND hImagenStatic = GetDlgItem(hwnd, PB_FOTOMODPROD);
			HBITMAP hImagenBMPStatic = (HBITMAP)LoadImage(NULL, userAccess->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);//HBITMAP ANTES DEL LOADIMAGE PARA INDICAR QUE SOLO QUIERO CARGAR BITMAP.
			SendMessage(hImagenStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagenBMPStatic);

			HWND hVendedorStatic = GetDlgItem(hwnd, LBL_NOMBREMODPROD);
			SetWindowText(hVendedorStatic, userAccess->sNombreVendedor.c_str());

			HWND hImagenModProd1 = GetDlgItem(hwnd, PB_MOD_IMAGEN1_PROD);
			HWND hImagenModProd2 = GetDlgItem(hwnd, PB_MOD_IMAGEN2_PROD);

			HBITMAP hBmImagen = (HBITMAP)LoadImage(NULL, currentProducto->foto1, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE); //DE AQUÍ
			sRutaImagen = currentProducto->foto1;
			HBITMAP hBmImagen2 = (HBITMAP)LoadImage(NULL, currentProducto->foto2, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE); //DE AQUÍ
			sRutaImagen2 = currentProducto->foto2;
			SendMessage(hImagenModProd1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmImagen);
			SendMessage(hImagenModProd2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmImagen2);
			SetWindowText(hTxtModProdNombre, currentProducto->sNombreProducto.c_str());
			SetWindowText(hTxtModProdProductos, currentProducto->sCantidad.c_str());
			SetWindowText(hTxtModProdCodigo, currentProducto->sCodigo.c_str());
			SetWindowText(hTxtModProdMarca, currentProducto->sMarca.c_str());
			SetWindowText(hTxtModProdPrecio, currentProducto->sPrecio.c_str());
			SetWindowText(hTxtModProdDescripcion, currentProducto->sDescripcion.c_str()); //HASTA AQUÍ

		}
		 break;
		case WM_COMMAND: {
			if (LOWORD(wparam) == BTN_MOD_IMAGEN1_PROD && HIWORD(wparam) == BN_CLICKED) {
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				char cRutaDelArchivo[MAX_PATH] = "";

				ofn.hwndOwner = hwnd;
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.lpstrFile = cRutaDelArchivo;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrDefExt = "txt";
				ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
				ofn.lpstrFilter = "Solo imagenes\0*.bmp\0Solo texto\0*.txt\0Todos los archivos\0*.*\0";
				if (GetOpenFileName(&ofn)) {

					sRutaImagen = cRutaDelArchivo;
					HWND hPicImagen = GetDlgItem(hwnd, PB_MOD_IMAGEN1_PROD);
					HBITMAP hBmImagen = (HBITMAP)LoadImage(NULL, cRutaDelArchivo, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
					SendMessage(hPicImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmImagen);

				}
				else {
					sRutaImagen = "0";
					MessageBox(hwnd, "Falto agregar la imagen", "Error", MB_ICONERROR);
				}

			}
			if (LOWORD(wparam) == BTN_MOD_IMAGEN2_PROD && HIWORD(wparam) == BN_CLICKED) {
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				char cRutaDelArchivo2[MAX_PATH] = "";

				ofn.hwndOwner = hwnd;
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.lpstrFile = cRutaDelArchivo2;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrDefExt = "txt";
				ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
				ofn.lpstrFilter = "Solo imagenes\0*.bmp\0Solo texto\0*.txt\0Todos los archivos\0*.*\0";
				if (GetOpenFileName(&ofn)) {

					sRutaImagen2 = cRutaDelArchivo2;
					HWND hPicImagen = GetDlgItem(hwnd, PB_MOD_IMAGEN2_PROD);
					HBITMAP hBmImagen = (HBITMAP)LoadImage(NULL, cRutaDelArchivo2, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
					SendMessage(hPicImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmImagen);

				}
				else {
					sRutaImagen2 = "0";
					MessageBox(hwnd, "Falto agregar la imagen", "Error", MB_ICONERROR);
				}

			}
			switch (LOWORD(wparam)) {
				case BTN_ACEPTAR_MOD_PROD: {

					string nombre = getText(hTxtModProdNombre);
					string productos = getText(hTxtModProdProductos);
					string codigo = getText(hTxtModProdCodigo);
					string marca = getText(hTxtModProdMarca);
					string precio = getText(hTxtModProdPrecio);
					string descripcion = getText(hTxtModProdDescripcion);
					bool contador = false;

					if (contador == false) {

						if (nombre.compare("") == 0 || productos.compare("") == 0 || codigo.compare("") == 0 || marca.compare("") == 0 || precio.compare("") == 0 || descripcion.compare("") == 0 || sRutaImagen.compare("0") == 0 || sRutaImagen2.compare("0") == 0) { //COMENTEN LO DE LA FOTO
							MessageBox(hwnd, "Falto llenar la información", "Error", MB_ICONERROR);
							contador = true;
						}	

						currentProducto->sNombreProducto = nombre; //DE AQUÍ
						currentProducto->sCantidad = productos;
						currentProducto->sCodigo = codigo;
						currentProducto->sMarca = marca;
						currentProducto->sPrecio = precio;
						currentProducto->sDescripcion = descripcion;
						strcpy(currentProducto->foto1, sRutaImagen.c_str()); //HASTA AQUÍ
						strcpy(currentProducto->foto2, sRutaImagen2.c_str());

						HWND hProductos = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
						ShowWindow(hProductos, SW_SHOW);
						DestroyWindow(hwnd);
					}

				}break;

				case ID_INFO_VENDEDOR: {
					HWND hOctava = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INFO_VENDEDOR), NULL, funcionInfoVendedorStatic);
					ShowWindow(hOctava, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_ALTA_PRODUCTOS: {
					HWND hPrimera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ALTA_PRODUCTOS), NULL, funcionAltaProducto);
					ShowWindow(hPrimera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_BAJA_PRODUCTOS: {
					HWND hTercera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
					ShowWindow(hTercera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;

				case ID_NUEVO_ENVIO: {
					HWND hQuinta = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_NUEVO_ENVIO), NULL, funcionNuevoEnvio);
					ShowWindow(hQuinta, SW_SHOW);
					SetTimer(hQuinta, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;

				case ID_ENVIOS_ELIMINARENVIO: {
					HWND hSeptima = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionNuevoEnvio);
					ShowWindow(hSeptima, SW_SHOW);
					SetTimer(hSeptima, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_SALIR: {

					int retornoSioNo = MessageBox(hwnd, "¿Esta seguro que quiere salir?", "Aviso", MB_ICONQUESTION | MB_YESNO);
					if (retornoSioNo == 6) {

						save(auxiliar);
						saveProductos(auxiliarProducto);
						saveEnvios(auxiliarEnvios);
						exitWindowModProdcuto = true;

						DestroyWindow(hwnd);
					}
					else {
						if (retornoSioNo == 7)
						break;
					}


				}
				break;
			}
		}
		break;
		case WM_DESTROY:
			if (exitWindowModProdcuto)
			PostQuitMessage(9);
		break;
	}
	return FALSE;
}

BOOL CALLBACK funcionBajaProducto(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		fInstertarMenu(hwnd);

		HWND hImagenStatic = GetDlgItem(hwnd, PB_FOTOPRODUCTOSS);
		HBITMAP hImagenBMPStatic = (HBITMAP)LoadImage(NULL, userAccess->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);//HBITMAP ANTES DEL LOADIMAGE PARA INDICAR QUE SOLO QUIERO CARGAR BITMAP.
		SendMessage(hImagenStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagenBMPStatic);

		HWND hVendedorStatic = GetDlgItem(hwnd, LBL_NOMBREPRODUCTOSS);
		SetWindowText(hVendedorStatic, userAccess->sNombreVendedor.c_str());

		hListBoxProductos = GetDlgItem(hwnd, LBX_PRODUCTOS);

		if (origenProducto != NULL) {
			int indice = 0;
			while (auxiliarProducto != NULL) {
				if (userAccess->IDUsuario == auxiliarProducto->IDUsuario) {
					string auxiliarListBox;
					auxiliarListBox.append("   Nombre:");
					auxiliarListBox.append(auxiliarProducto->sNombreProducto);
					auxiliarListBox.append("   Cantidad:");
					auxiliarListBox.append(auxiliarProducto->sCantidad);
					auxiliarListBox.append("   Marca:");
					auxiliarListBox.append(auxiliarProducto->sMarca);
					auxiliarListBox.append("   Codigo:");
					auxiliarListBox.append(auxiliarProducto->sCodigo);
					auxiliarListBox.append("   Descripcion:");
					auxiliarListBox.append(auxiliarProducto->sDescripcion);
					auxiliarListBox.append("     $");
					auxiliarListBox.append(auxiliarProducto->sPrecio);
					SendMessage(hListBoxProductos, LB_ADDSTRING, NULL, (LPARAM)auxiliarListBox.c_str());
					SendMessage(hListBoxProductos, LB_SETITEMDATA, (WPARAM)indice, (LPARAM)auxiliarProducto->IDProductos);
					indice++;
				}
				if (auxiliarProducto->sig == NULL) {
					break;
				}
				auxiliarProducto = auxiliarProducto->sig;

			}
			auxiliarProducto = origenProducto;
		}

	}
					  break;
		case WM_COMMAND: {

			if (LOWORD(wparam) == LBX_PRODUCTOS && HIWORD(wparam) == LBN_SELCHANGE) {

				HWND hListBox = GetDlgItem(hwnd, LBX_PRODUCTOS);
				int indiceSeleccionado = SendMessage(hListBox, LB_GETCURSEL, NULL, NULL);

				int auxiliarID = SendMessage(hListBox, LB_GETITEMDATA, (WPARAM)indiceSeleccionado, (LPARAM)NULL);

				HWND hPictureControl1 = GetDlgItem(hwnd, PB_IMAGEN1PRODUC);
				HWND hPictureControl2 = GetDlgItem(hwnd, PB_IMAGEN2PRODUC);

				while (auxiliarProducto->IDProductos != auxiliarID) {
					auxiliarProducto = auxiliarProducto->sig;
				}

				HBITMAP hImagen1 = (HBITMAP)LoadImage(NULL, auxiliarProducto->foto1, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
				HBITMAP hImagen2 = (HBITMAP)LoadImage(NULL, auxiliarProducto->foto2, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);

				SendMessage(hPictureControl1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen1);
				SendMessage(hPictureControl2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen2);

				auxiliarProducto = origenProducto;
			}

			switch (LOWORD(wparam)) {
				case ID_INFO_VENDEDOR: {
					HWND hOctava = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INFO_VENDEDOR), NULL, funcionInfoVendedorStatic);
					ShowWindow(hOctava, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
							 break;
				case ID_ALTA_PRODUCTOS: {
					HWND hPrimera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ALTA_PRODUCTOS), NULL, funcionAltaProducto);
					ShowWindow(hPrimera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;

				case ID_NUEVO_ENVIO: {
					HWND hQuinta = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_NUEVO_ENVIO), NULL, funcionNuevoEnvio);
					ShowWindow(hQuinta, SW_SHOW);
					SetTimer(hQuinta, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
						   break;

				case ID_ENVIOS_ELIMINARENVIO: {
					HWND hSeptima = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionNuevoEnvio);
					ShowWindow(hSeptima, SW_SHOW);
					SetTimer(hSeptima, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
									break;
				case ID_SALIR: {

					int retornoSioNo = MessageBox(hwnd, "¿Esta seguro que quiere salir?", "Aviso", MB_ICONQUESTION | MB_YESNO);
					if (retornoSioNo == 6) {

						save(auxiliar);
						saveProductos(auxiliarProducto);
						saveEnvios(auxiliarEnvios);

						exitWindowBajaProducto = true;

						DestroyWindow(hwnd);
					}
					else {
						if (retornoSioNo == 7)
						break;
					}

		
				}
				break;

				case BTN_MOD_PROD: {
					int posicion = SendMessage(hListBoxProductos, LB_GETCURSEL, 0, 0); //DE AQUÍ
					int codigo = SendMessage(hListBoxProductos, LB_GETITEMDATA, posicion, 0);

					if (posicion < 0) {
						MessageBox(hwnd, "No se pudo encontrar un producto", "Error", MB_ICONERROR);
					}
					else {

						while (auxiliarProducto != NULL) {
							if (codigo == auxiliarProducto->IDProductos) {
								currentProducto = auxiliarProducto;
								auxiliarProducto = origenProducto;

								HWND hModificarProducto = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_MOD_PRODUCTO), NULL, funcionModProducto);
								SetTimer(hModificarProducto, MI_TIMER, 1000, (TIMERPROC)NULL);
								ShowWindow(hModificarProducto, SW_SHOW);
								DestroyWindow(hwnd);
								break;

							}
							auxiliarProducto = auxiliarProducto->sig;
						}

						auxiliarProducto = origenProducto;

					}

				}
				break;

				case BTN_BAJA_PROD: {

					auxiliarProducto = origenProducto;
					int indice = SendMessage(hListBoxProductos, LB_GETCURSEL, NULL, NULL);
					int identificador = SendMessage(hListBoxProductos, LB_GETITEMDATA, indice, NULL);

					if (indice < 0) {
						MessageBox(hwnd, "Seleccionar un producto", "Error", MB_ICONERROR);
					}
					else {					
						
						int decidir = MessageBox(hwnd, "¿Esta seguro que quiere eliminar el producto?", "Advertencia", MB_YESNO | MB_ICONWARNING);
						if (decidir == 6) {
							bool searchFound = true;

							while (auxiliarProducto->IDProductos!= identificador) { //Busco todas las matriculas hasta que una haga match
								if (auxiliarProducto->sig == NULL) { //Llegue al final de la lista
									searchFound = false; //Es el unico lugar donde me indica que NO ENCONTRE lo que buscaba
									break; //Rompe un ciclo
								}
								auxiliarProducto = auxiliarProducto->sig; //Avanzo la lista
							}
							if (searchFound == false) { //Sabemos que el unico lugar donde se hace falso es en el if dentro del while ya que no lo encontro															
								auxiliarProducto = origenProducto;
								break;
							}
#pragma endregion
							bool foundEnvio = false;
							while (auxiliarEnvios != NULL) {
								if (auxiliarEnvios->sNombreProducto == auxiliarProducto->sNombreProducto) {
									foundEnvio = true;
								}
								auxiliarEnvios = auxiliarEnvios->sig;
							}
							auxiliarEnvios = origenEnvios;

							if (foundEnvio == false) {

								if (auxiliarProducto->prev == NULL && auxiliarProducto->sig == NULL) { //El unico
									delete auxiliarProducto; //Borramos el unico elemento
									auxiliarProducto = origenProducto = NULL; //Regresamos todo a NULL para que se cumpla el primer if de ALTA
								}
								else if (auxiliarProducto->prev == NULL) { //Primero
									origenProducto = origenProducto->sig; //Movemos origen al siguiente elemento que ahora se convertira en el nuevo origen
									origenProducto->prev = NULL; //El previo de este nuevo origen es null para indicarnos que es el primer elemento
									delete auxiliarProducto; //Borramos aux
									auxiliarProducto = origenProducto; //Lo regresamos al nuevo origen
								}
								else if (auxiliarProducto->sig == NULL) { //Ultimo
									auxiliarProducto->prev->sig = NULL; // De aux (Ultimo) viajo a prev (Elemento anterior) y su sig lo apunto a NULL para indicar que ese es el nuevo ultimo elemento
									delete auxiliarProducto; //Borro aux
									auxiliarProducto = origenProducto; //Lo regresamos a origen
								}
								else { //Cualquier otro
									//Santiago->sig = Carlos ?
									auxiliarProducto->prev->sig = auxiliarProducto->sig;
									//Carlos->prev = Santiago ?
									auxiliarProducto->sig->prev = auxiliarProducto->prev;
									delete auxiliarProducto;
									auxiliarProducto = origenProducto;
								}
								HWND hProductos = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
								SetTimer(hProductos, MI_TIMER, 1000, (TIMERPROC)NULL);
								ShowWindow(hProductos, SW_SHOW);
								ShowWindow(hwnd, SW_HIDE);
								DestroyWindow(hwnd);
							}
							else if (foundEnvio == true) {
								MessageBox(hwnd, "No se puede eliminar el producto porque ya esta asignado a un envio", "Error", MB_ICONERROR);
								auxiliarProducto = origenProducto;
							}
							

						}
						else {
							if (decidir == 7)
								break;
						}
					
					}

				}
			}
		}
		break;
		case WM_DESTROY:
			if (exitWindowBajaProducto)
			PostQuitMessage(9);
		break;
	}
	return FALSE;
}

BOOL CALLBACK funcionNuevoEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		case WM_INITDIALOG: {
			fInstertarMenu(hwnd);
			
			hTxtNuevoEnvioCantidad = GetDlgItem(hwnd, TXT_CANTIDAD);
			hTxtNuevoEnvioCalle = GetDlgItem(hwnd, TXT_CALLE);
			hTxtNuevoEnvioColonia = GetDlgItem(hwnd, TXT_COLONIA);
			hTxtNuevoEnvioCiudad = GetDlgItem(hwnd, TXT_CIUDAD);
			hTxtNuevoEnvioMensaje = GetDlgItem(hwnd, TXT_MENSAJE_ENVIO);

			HWND hImagenStatic = GetDlgItem(hwnd, PB_FOTONUEVOENVIO);
			HBITMAP hImagenBMPStatic = (HBITMAP)LoadImage(NULL, userAccess->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);//HBITMAP ANTES DEL LOADIMAGE PARA INDICAR QUE SOLO QUIERO CARGAR BITMAP.
			SendMessage(hImagenStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagenBMPStatic);

			HWND hVendedorStatic = GetDlgItem(hwnd, LBL_NOMBRENUEVOENVIO);
			SetWindowText(hVendedorStatic, userAccess->sNombreVendedor.c_str());

			HWND hNuevoEnvioProducto = GetDlgItem(hwnd, LBL_SELEC_PROD);
			SendMessage(hNuevoEnvioProducto, CB_ADDSTRING, NULL, (LPARAM)"Seleccione un producto");

			while (auxiliarProducto != NULL) {

				if (auxiliarProducto->IDUsuario==userAccess->IDUsuario) {
					SendMessage(hNuevoEnvioProducto, CB_ADDSTRING, NULL, (LPARAM)auxiliarProducto->sNombreProducto.c_str());
				}
				if (auxiliarProducto->sig==NULL) {
					break;
				}
				auxiliarProducto = auxiliarProducto->sig;

			}
			auxiliarProducto = origenProducto;

			SendMessage(hNuevoEnvioProducto, CB_SETCURSEL, 0, NULL);
			
		
			HWND hNuevoEnvioEstados = GetDlgItem(hwnd, LBL_ESTADO);
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Seleccione un estado");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Aguascalientes");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Baja California");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Baja California Sur");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Campeche");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Chiapas");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Chihuahua");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Coahuila");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Colima");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"CDMX");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Durango");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Estado de México");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Guanajuato");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Guerrero");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Hidalgo");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Jalisco");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Michoacán");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Morelos");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Nayarit");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Nuevo León");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Oaxaca");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Puebla");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Querétaro");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Quintana Roo");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"San Luis Potosí");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Sinaloa");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Sonora");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Tabasco");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Tamaulipas");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Veracruz");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Yucatán");
			SendMessage(hNuevoEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Zacatecas");

			SendMessage(hNuevoEnvioEstados, CB_SETCURSEL, 0, NULL);



		}
		 break;
		case WM_TIMER: {  //DE AQUI
			if (fechaObtenida) {
				break;
			}
			time(&longSecondsUnix);
			structOFTime = localtime(&longSecondsUnix);

			char dia[5], mes[5], anio[5]; //El arreglo esta en 5 porque el dia y mes tienen 2 numeros y anio 4

			_itoa(structOFTime->tm_mday, dia, 10);
			_itoa(++structOFTime->tm_mon, mes, 10);
			_itoa(structOFTime->tm_year + 1900, anio, 10); //Se cuenta de 1900 en adelante (tiempo en base a unix)

			string sDia = dia;
			if (sDia.length() < 2) {
				sDia.clear();
				sDia.append("0");
				sDia.append(dia);
			}
			string sMes = mes;
			if (sMes.length() < 2) {
				sMes.clear();
				sMes.append("0");
				sMes.append(mes);
			}
			string sAnio = anio;
			string sFechaAcual = sAnio + sMes + sDia;
			fechaActual = stoi(sFechaAcual); //El stoi es para pasar de string a entero
			fechaObtenida = true;
		}break;  //HASTA AQUI
		case WM_COMMAND: {
			switch (LOWORD(wparam)) {
				case BTN_CANCELAR_ENVIO: {
					if (fechaObtenida) {  //AQUI
						HWND hSecondWindow = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionEliminarEnvio);
						SetTimer(hSecondWindow, MI_TIMER, 1000, (TIMERPROC)NULL);  //AQUI
						ShowWindow(hSecondWindow, SW_SHOW);
						KillTimer(hwnd, MI_TIMER);  //AQUI
						DestroyWindow(hwnd);
					}
				}
				case BTN_ACEPTAR_ENVIO: {

					HWND hBtnAceptar = GetDlgItem(hwnd, BTN_ACEPTAR_ENVIO);

					HWND hNuevoEnvioProducto = GetDlgItem(hwnd, LBL_SELEC_PROD);
					int iSeleccion = SendMessage(hNuevoEnvioProducto, CB_GETCURSEL, NULL, NULL);

					char productos[50];
					SendMessage(hNuevoEnvioProducto, CB_GETLBTEXT, iSeleccion, (LPARAM)productos);					

					HWND hTxtNuevoEnvioCantidad = GetDlgItem(hwnd, TXT_CANTIDAD);
					string cantidad = getText(hTxtNuevoEnvioCantidad);

					HWND hConfirmarcalle = GetDlgItem(hwnd, TXT_CALLE);
					string calle = getText(hTxtNuevoEnvioCalle);

					HWND hConfirmarColonia = GetDlgItem(hwnd, TXT_COLONIA);
					string colonia = getText(hTxtNuevoEnvioColonia);

					HWND hConfirmarCiudad = GetDlgItem(hwnd, TXT_CIUDAD);
					string ciudad = getText(hTxtNuevoEnvioCiudad);

					HWND hNuevoEnvioEstados = GetDlgItem(hwnd, LBL_ESTADO);
					string estados = getText(hNuevoEnvioEstados);

					HWND hConfirmarMensaje = GetDlgItem(hwnd, TXT_MENSAJE_ENVIO);
					string mensaje = getText(hTxtNuevoEnvioMensaje);

					HWND hTotal = GetDlgItem(hwnd, LBL_TOTALENVIO);
					string total = getText(hTotal);

					HWND hFechaDia = GetDlgItem(hwnd, IDC_DATETIMEPICKER1);
					string fecha = getText(hFechaEnvio);


					HWND hFecha = GetDlgItem(hwnd, IDC_DATETIMEPICKER1);  //DE AQUI
					string sFecha = getText(hFecha);
					string diaDTP = sFecha.substr(0, 2); //substr -> extrae una parte del string
					string mesDTP = sFecha.substr(3, 2);
					string anioDTP = sFecha.substr(6, 4);
					string sFechaSelec = anioDTP + mesDTP + diaDTP; //Acomoda año, mes, dia
					int iFechaSelec = stoi(sFechaSelec);  //HASTA AQUI

					int iTamanio = GetWindowTextLength(hTxtNuevoEnvioCantidad);
					int indice = SendMessage(hNuevoEnvioProducto, CB_GETCURSEL, NULL, NULL); //PONER
					int indice2 = SendMessage(hNuevoEnvioEstados, CB_GETCURSEL, NULL, NULL);
					
					bool validarCorrecto = false;

					if (indice == 0 || cantidad.compare("") == 0 || calle.compare("") == 0 || colonia.compare("") == 0 || ciudad.compare("") == 0 || indice2 == 0 || mensaje.compare("") == 0) {
						MessageBox(hwnd, "Falto llenar la información", "Error", MB_ICONERROR);
						break;
					}
					else {  //DE AQUI
						if (iFechaSelec <= fechaActual) {
							MessageBox(hwnd, "La fecha de envio no puede ser igual o anterior a la actual", "EROOR", MB_ICONWARNING);
							validarCorrecto = true;
						}
					  //HASTA AQUI
					
						if (fValidarSoloNumeros(cantidad, iTamanio)) {
							MessageBox(hwnd, "Solo puedes ingresar números en la cantidad de envios", "Error", MB_ICONERROR);
							validarCorrecto = true;
						}
						else {
							while (auxiliarProducto != NULL) {
								if (auxiliarProducto->IDUsuario == userAccess->IDUsuario) {
									if (strcmp(productos, auxiliarProducto->sNombreProducto.c_str()) == 0) {
										int iCantInv = stoi(auxiliarProducto->sCantidad);
										int iCantEnv = stoi(cantidad);
										if (iCantInv - iCantEnv < 0 || iCantEnv == 0) {
											validarCorrecto = true;
											EnableWindow(hBtnAceptar, false);
											MessageBox(hwnd, "No hay suficiente cantidad de inventario", "Error", MB_ICONWARNING);
										}
										break;
									}
								}
								if (auxiliarProducto->sig == NULL) {
									break;
								}
								auxiliarProducto = auxiliarProducto->sig;
							}
							auxiliarProducto = origenProducto;
						}
					}
					
					if (validarCorrecto == false) {
						while (auxiliarProducto != NULL) {
							if (auxiliarProducto->IDUsuario == userAccess->IDUsuario) {
								if (strcmp(productos, auxiliarProducto->sNombreProducto.c_str()) == 0) {
									int iCantInv = stoi(auxiliarProducto->sCantidad);
									int iCantEnv = stoi(cantidad);
									int iNuevoInv = iCantInv - iCantEnv;

									char cNuevoInv[10];
									_itoa(iNuevoInv, cNuevoInv, 10);
									auxiliarProducto->sCantidad = cNuevoInv;
									break;
								}
							}
							if (auxiliarProducto->sig == NULL) {
								break;
							}
							auxiliarProducto = auxiliarProducto->sig;
						}
						auxiliarProducto = origenProducto;

						if (origenEnvios == NULL) {
							origenEnvios = new envios;
							origenEnvios->sNombreProducto = productos;
							origenEnvios->sCantidad = cantidad;
							origenEnvios->sCalle = calle;
							origenEnvios->sColonia.append(colonia);
							origenEnvios->sCiudad.append(ciudad);
							origenEnvios->sEstado = estados;
							origenEnvios->sMensaje.append(mensaje);
							origenEnvios->sTotal = total;
							origenEnvios->sDia = diaDTP;
							origenEnvios->sMes = mesDTP;
							origenEnvios->sAnio = anioDTP;
							origenEnvios->IDUsuario = userAccess->IDUsuario;
							origenEnvios->IDEnvios = GLOBALID_ENVIOS++;
							origenEnvios->IDFecha = iFechaSelec;
							origenEnvios->prev = NULL;
							origenEnvios->sig = NULL;
							auxiliarEnvios = origenEnvios;
						}
						else {
							while (auxiliarEnvios->sig != NULL)
							auxiliarEnvios = auxiliarEnvios->sig;
							auxiliarEnvios->sig = new envios;
							auxiliarEnvios->sig->prev = auxiliarEnvios;
							auxiliarEnvios = auxiliarEnvios->sig;
							auxiliarEnvios->IDUsuario = userAccess->IDUsuario;
							auxiliarEnvios->IDEnvios = GLOBALID_ENVIOS++;
							auxiliarEnvios->sNombreProducto = productos; //PONER
							auxiliarEnvios->sCantidad = cantidad;
							auxiliarEnvios->sCalle = calle;
							auxiliarEnvios->sColonia.append(colonia);
							auxiliarEnvios->sCiudad.append(ciudad);
							auxiliarEnvios->sEstado = estados;
							auxiliarEnvios->sMensaje.append(mensaje);
							auxiliarEnvios->sTotal = total;
							auxiliarEnvios->sDia = diaDTP;  //DE AQUI
							auxiliarEnvios->sMes = mesDTP;
							auxiliarEnvios->sAnio = anioDTP;
							auxiliarEnvios->IDFecha = iFechaSelec;
							auxiliarEnvios->sig = NULL;
							auxiliarEnvios = origenEnvios;
						}
						saveGlobalId();
						auxiliarProducto = origenProducto;
						sRutaImagen = "0";
						sRutaImagen2 = "0";

					}
					if (validarCorrecto == false) {
						HWND hSecondWindow = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionEliminarEnvio);
						SetTimer(hSecondWindow, MI_TIMER, 1000, (TIMERPROC)NULL);  //AQUI
						ShowWindow(hSecondWindow, SW_SHOW);
						KillTimer(hwnd, MI_TIMER);  //AQUI
						DestroyWindow(hwnd);
					}

				}break;

				case BTN_ASIGNARTOTAL: {

					HWND hNuevoEnvioProducto = GetDlgItem(hwnd, LBL_SELEC_PROD);
					HWND hCantidad = GetDlgItem(hwnd, TXT_CANTIDAD);
					HWND hTotal = GetDlgItem(hwnd, LBL_TOTALENVIO);

					string cantidad = getText(hCantidad);
					int indice = SendMessage(hNuevoEnvioProducto, CB_GETCURSEL, NULL, NULL);

					if (cantidad.compare("") == 0 || indice == 0) {
						MessageBox(hwnd, "Falta agregar información", "Error", MB_ICONWARNING);
						break;
					}
					else if (fValidarSoloNumeros(cantidad, cantidad.length())) {
						MessageBox(hwnd, "Solo puedes ingresar números en la cantidad de envios", "Error", MB_ICONERROR);
						break;
					}
					else {
						char producto[100];
						SendMessage(hNuevoEnvioProducto, CB_GETLBTEXT, indice, (LPARAM)producto);
						int precio;
						while (auxiliarProducto != NULL) {
							if (auxiliarProducto->IDUsuario == userAccess->IDUsuario) {
								if (strcmp(producto, auxiliarProducto->sNombreProducto.c_str()) == 0) {

									precio = atof(auxiliarProducto->sPrecio.c_str());
									break;

								}
							}
							if (auxiliarProducto->sig == NULL) {
								break;
							}
							auxiliarProducto = auxiliarProducto->sig;
						}
						auxiliarProducto = origenProducto;

						int cantidadNueva = stoi(cantidad);
						int total = precio * cantidadNueva;
						char totalFinal[20];
						_itoa(total, totalFinal, 10);
						SendMessage(hTotal, WM_SETTEXT, NULL, (LPARAM)totalFinal);
					}


				}
				break;

				case ID_INFO_VENDEDOR: {
					HWND hOctava = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INFO_VENDEDOR), NULL, funcionInfoVendedorStatic);
					ShowWindow(hOctava, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_ALTA_PRODUCTOS: {
					HWND hPrimera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ALTA_PRODUCTOS), NULL, funcionAltaProducto);
					ShowWindow(hPrimera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				
				break;
				case ID_BAJA_PRODUCTOS: {
					HWND hTercera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
					ShowWindow(hTercera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				

				
				case ID_ENVIOS_ELIMINARENVIO: {
					HWND hSeptima = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionEliminarEnvio);
					SetTimer(hSeptima, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hSeptima, SW_SHOW);
					KillTimer(hwnd, MI_TIMER);
					
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_SALIR: {

					if (fechaObtenida) {
						int retornoSioNo = MessageBox(hwnd, "¿Esta seguro que quiere salir?", "Aviso", MB_ICONQUESTION | MB_YESNO);
						if (retornoSioNo == 6) {

							save(auxiliar);
							saveProductos(auxiliarProducto);
							saveEnvios(auxiliarEnvios);
							exitWindowNuevoEnvio = true;
							DestroyWindow(hwnd);
						}
						else {
							if (retornoSioNo == 7)
								break;
						}
					}

				}
				break;
			}
		}
		break;
		case WM_DESTROY:
			if (exitWindowNuevoEnvio)
				PostQuitMessage(9);
			break;

	}
	return FALSE;
}

BOOL CALLBACK funcionResumenEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		case WM_INITDIALOG: {
			fInstertarMenu(hwnd);
			HWND hNuevoEnvioProducto = GetDlgItem(hwnd, LBL_SELEC_PROD);
			HWND hNuevoEnvioEstados = GetDlgItem(hwnd, LBL_ESTADO);
			HWND hFecha = GetDlgItem(hwnd, IDC_DATETIMEPICKER1);
			HWND hTotal = GetDlgItem(hwnd, LBL_TOTALENVIO);

			HWND hImagenStatic = GetDlgItem(hwnd, PB_FOTORESUMENENVIO);
			HBITMAP hImagenBMPStatic = (HBITMAP)LoadImage(NULL, userAccess->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);//HBITMAP ANTES DEL LOADIMAGE PARA INDICAR QUE SOLO QUIERO CARGAR BITMAP.
			SendMessage(hImagenStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagenBMPStatic);

			HWND hVendedorStatic = GetDlgItem(hwnd, LBL_NOMBRERESUMENENVIO);
			SetWindowText(hVendedorStatic, userAccess->sNombreVendedor.c_str());
			

			hNuevoEnvioProducto = GetDlgItem(hwnd, LBL_PRODUCTOSTATIC);
			hLblMosEnvioCantidad = GetDlgItem(hwnd, LBL_CANTIDADSTATIC);
			hLblMosEnvioCalle = GetDlgItem(hwnd, LBL_CALLESTATIC);
			hLblMosEnvioColonia = GetDlgItem(hwnd, LBL_COLONIASTATIC);
			hLblMosEnvioCiudad = GetDlgItem(hwnd, LBL_CIUDADSTATIC);
			hNuevoEnvioEstados = GetDlgItem(hwnd, LBL_ESTADOSTATIC);
			hLblMosEnvioMensaje = GetDlgItem(hwnd, LBL_MENSAJESTATIC);
			hFecha = GetDlgItem(hwnd, LBL_FECHASTATIC);
			hTotal = GetDlgItem(hwnd, LBL_TOTALSTATIC);

			string fecha = auxiliarEnvios->sDia + "/" + auxiliarEnvios->sMes + "/" + auxiliarEnvios->sAnio;

			SetWindowText(hNuevoEnvioProducto, currentEnvio->sNombreProducto.c_str());
			SetWindowText(hLblMosEnvioCantidad, currentEnvio->sCantidad.c_str());
			SetWindowText(hLblMosEnvioCalle, currentEnvio->sCalle.c_str());
			SetWindowText(hLblMosEnvioColonia, currentEnvio->sColonia.c_str());
			SetWindowText(hLblMosEnvioCiudad, currentEnvio->sCiudad.c_str());
			SetWindowText(hNuevoEnvioEstados, currentEnvio->sEstado.c_str());
			SetWindowText(hLblMosEnvioMensaje, currentEnvio->sMensaje.c_str());
			SetWindowText(hTotal, currentEnvio->sTotal.c_str());
			SendMessage(hFecha, WM_SETTEXT, NULL, (LPARAM)fecha.c_str());

		}
					  break;
		case WM_COMMAND: {
			switch (LOWORD(wparam)) {

				case BTN_ACEPTAR_RESUM_ENVIO: {
					HWND hTercera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
					ShowWindow(hTercera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
											break;
				case ID_INFO_VENDEDOR: {
					HWND hOctava = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INFO_VENDEDOR), NULL, funcionInfoVendedorStatic);
					ShowWindow(hOctava, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
							 break;
				case ID_ALTA_PRODUCTOS: {
					HWND hPrimera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ALTA_PRODUCTOS), NULL, funcionAltaProducto);
					ShowWindow(hPrimera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
							  break;

							  break;
				case ID_BAJA_PRODUCTOS: {
					HWND hTercera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
					ShowWindow(hTercera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
							  break;
				case ID_NUEVO_ENVIO: {
					HWND hQuinta = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_NUEVO_ENVIO), NULL, funcionNuevoEnvio);
					ShowWindow(hQuinta, SW_SHOW);
					SetTimer(hQuinta, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
						   break;

				case ID_ENVIOS_ELIMINARENVIO: {
					HWND hSeptima = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionNuevoEnvio);
					ShowWindow(hSeptima, SW_SHOW);
					SetTimer(hSeptima, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
									break;
				case ID_SALIR: {

					int retornoSioNo = MessageBox(hwnd, "¿Esta seguro que quiere salir?", "Aviso", MB_ICONQUESTION | MB_YESNO);
					if (retornoSioNo == 6) {

						save(auxiliar);
						saveProductos(auxiliarProducto);
						saveEnvios(auxiliarEnvios);
						exitWindowResumenEnvio = true;
						DestroyWindow(hwnd);
					}
					else {
					if (retornoSioNo == 7)
						break;
					}


				}
					 break;
			}
		}
				   break;
		case WM_DESTROY:
			if (exitWindowResumenEnvio)
				PostQuitMessage(9);
			break;
	}
	return FALSE;
}

BOOL CALLBACK funcionModEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		case WM_INITDIALOG: {
			fInstertarMenu(hwnd);


			HWND hTotal = GetDlgItem(hwnd, LBL_MODTOTAL);
			HWND hCantidadStatic = GetDlgItem(hwnd, TXT_CANTIDADMOD);
			HWND hCalleStatic = GetDlgItem(hwnd, TXT_MOD_CALLE); //DE AQUÍ
			HWND hColoniaStatic = GetDlgItem(hwnd, TXT_MOD_COLONIA_ENVIO);
			HWND hCiudadStatic = GetDlgItem(hwnd, TXT_MOD_CIUDAD_ENVIO);
			HWND hMensajeStatic = GetDlgItem(hwnd, TXT_MOD_MENSAJE_ENVIO);

			HWND hImagenStatic = GetDlgItem(hwnd, PB_FOTOMODENVIO);
			HBITMAP hImagenBMPStatic = (HBITMAP)LoadImage(NULL, userAccess->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);//HBITMAP ANTES DEL LOADIMAGE PARA INDICAR QUE SOLO QUIERO CARGAR BITMAP.		
			SendMessage(hImagenStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagenBMPStatic);

			HWND hVendedorStatic = GetDlgItem(hwnd, LBL_NOMBREMODENVIO);
			SetWindowText(hVendedorStatic, userAccess->sNombreVendedor.c_str());

			/*SetWindowText(hCalleStatic, currentEnvio->sCalle.c_str());
			SetWindowText(hColoniaStatic, currentEnvio->sColonia.c_str());
			SetWindowText(hCiudadStatic, currentEnvio->sCiudad.c_str());
			SetWindowText(hMensajeStatic, currentEnvio->sMensaje.c_str());*/

			HWND hModificarEnvioProducto = GetDlgItem(hwnd, LBL_MOD_PROD_ENVIO);
			
			while(auxiliarProducto!=NULL){
				SendMessage(hModificarEnvioProducto, CB_ADDSTRING, NULL, (LPARAM)"Seleccione un producto");
				auxiliarProducto = auxiliarProducto->sig;
			}
			auxiliarProducto = origenProducto;

			SendMessage(hModificarEnvioProducto, CB_SETCURSEL, 0, NULL);

			HWND hModificarEnvioEstados = GetDlgItem(hwnd, CMB_MOD_ESTADO_ENVIO);
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Seleccione un estado");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Seleccione un estado");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Aguascalientes");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Baja California");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Baja California Sur");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Campeche");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Chiapas");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Chihuahua");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Coahuila");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Colima");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"CDMX");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Durango");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Estado de México");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Guanajuato");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Guerrero");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Hidalgo");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Jalisco");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Michoacán");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Morelos");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Nayarit");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Nuevo León");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Oaxaca");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Puebla");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Querétaro");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Quintana Roo");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"San Luis Potosí");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Sinaloa");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Sonora");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Tabasco");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Tamaulipas");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Veracruz");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Yucatán");
			SendMessage(hModificarEnvioEstados, CB_ADDSTRING, NULL, (LPARAM)"Zacatecas");

			SendMessage(hModificarEnvioEstados, CB_SETCURSEL, 0, NULL);

			SetWindowText(hTotal, currentEnvio->sTotal.c_str());
			SetWindowText(hCantidadStatic, currentEnvio->sCantidad.c_str());
			SetWindowText(hCalleStatic, currentEnvio->sCalle.c_str());
			SetWindowText(hColoniaStatic, currentEnvio->sColonia.c_str());
			SetWindowText(hCiudadStatic, currentEnvio->sCiudad.c_str());
			SetWindowText(hMensajeStatic, currentEnvio->sMensaje.c_str());
			SetWindowText(hModificarEnvioProducto, currentEnvio->sNombreProducto.c_str());
			SetWindowText(hModificarEnvioEstados, currentEnvio->sEstado.c_str());

		}
		break;
		case WM_COMMAND: {
			switch (LOWORD(wparam)) {

				case BTN_ACEPTAR_MOD_ENVIO: {

					HWND hBtnAceptar = GetDlgItem(hwnd, BTN_ACEPTAR_MOD_ENVIO);

					HWND hModificarEnvioProducto = GetDlgItem(hwnd, LBL_MOD_PROD_ENVIO);
					int iSeleccion = SendMessage(hModificarEnvioProducto, CB_GETCURSEL, NULL, NULL);

					char producto[50];

					SendMessage(hModificarEnvioProducto, CB_GETLBTEXT, iSeleccion, (LPARAM)producto);

					HWND hConfirmarCantidadMod = GetDlgItem(hwnd, TXT_CANTIDADMOD);
					string sConfirmarCantidadMod = getText(hConfirmarCantidadMod);

					HWND hConfirmarCalleMod = GetDlgItem(hwnd, TXT_NOMBRE_EDIT_INFO);
					string sConfirmarCalleMod = getText(hConfirmarCalleMod);

					HWND hConfirmarColoniaMod = GetDlgItem(hwnd, TXT_EMPRESA_EDIT_INFO);
					string sConfirmarColoniaMod = getText(hConfirmarColoniaMod);

					HWND hConfirmarCiudadMod = GetDlgItem(hwnd, TXT_EMPRESA_EDIT_INFO);
					string sConfirmarCiudadMod = getText(hConfirmarCiudadMod);

					HWND hConfirmarMensajeMod = GetDlgItem(hwnd, TXT_EMPRESA_EDIT_INFO);
					string sConfirmarMensajeMod = getText(hConfirmarMensajeMod);


					HWND hTotal = GetDlgItem(hwnd, LBL_MODTOTAL);
					
					HWND hModificarEnvioEstados = GetDlgItem(hwnd, CMB_MOD_ESTADO_ENVIO);
					string total = getText(hTotal);				
					string estados = getText(hModificarEnvioEstados);

					bool contador = false;
					int iConfirmarInfoVendedor = GetWindowTextLength(hConfirmarCalleMod);

					if (sConfirmarCantidadMod.compare("")==0||sConfirmarCalleMod.compare("") == 0 || sConfirmarColoniaMod.compare("") == 0 || sConfirmarCiudadMod.compare("")==0||sConfirmarMensajeMod.compare("0")==0) {
						MessageBox(NULL, "Falto llenar informacion", "ERROR", MB_ICONASTERISK);
						contador = true;
						break;
					}
					else {
						if (auxiliarEnvios != NULL) {
							while (auxiliarEnvios->IDEnvios != currentEnvio->IDEnvios) {
								auxiliarEnvios = auxiliarEnvios->sig;
							}
						}
						while (auxiliarProducto != NULL) {
							if (auxiliarProducto->IDUsuario == userAccess->IDUsuario) {
								if (strcmp(producto, auxiliarProducto->sNombreProducto.c_str()) == 0) {
									int iCantInv = stoi(auxiliarProducto->sCantidad);
									int iCantEnv = stoi(sConfirmarCantidadMod);
									int iCantEstablecida = stoi(auxiliarEnvios->sCantidad);

									if (iCantEnv - iCantEstablecida < 0 && iCantEnv != 0) {
										break;
									}

									if (iCantInv - iCantEnv < 0 || iCantEnv == 0) {
										contador = true;
										HWND hBtnAceptar = GetDlgItem(hwnd, BTN_ACEPTAR_MOD_ENVIO);
										EnableWindow(hBtnAceptar, false);
										MessageBox(hwnd, "No hay suficientes existencias en el inventario", "Error", MB_ICONWARNING);
									}
									break;
								}
							}
							if (auxiliarProducto->sig == NULL) {
								break;
							}
							auxiliarProducto = auxiliarProducto->sig;
						}
						auxiliarProducto = origenProducto;
						auxiliarEnvios = origenEnvios;
						EnableWindow(hBtnAceptar, true);
					}
					if (contador == false) {

						while (auxiliarProducto != NULL) {
							if (auxiliarProducto->IDUsuario == userAccess->IDUsuario) {
								if (strcmp(producto, auxiliarProducto->sNombreProducto.c_str()) == 0) {
									int iCantInv = stoi(auxiliarProducto->sCantidad);
									int iCantEnv = stoi(sConfirmarCantidadMod);
									int iCantEstablecida = stoi(auxiliarEnvios->sCantidad);

									if (iCantEnv < iCantEstablecida) {
										int iSumaInv = iCantEstablecida - iCantEnv;
										iCantInv = iCantInv + iSumaInv;

										char cNuevoInv[10];
										_itoa(iCantInv, cNuevoInv, 10);
										auxiliarProducto->sCantidad = cNuevoInv;
									}
									else if (iCantEnv > iCantEstablecida) {
										int iRestaInv = iCantEnv - iCantEstablecida;
										iCantInv = iCantInv - iRestaInv;

										char cNuevoInv[10];
										_itoa(iCantInv, cNuevoInv, 10);
										auxiliarProducto->sCantidad = cNuevoInv;
									}
									break;

								}
							}
							if (auxiliarProducto->sig == NULL) {
								break;
							}
							auxiliarProducto = auxiliarProducto->sig;
						}
						auxiliarProducto = origenProducto;

						currentEnvio->sCantidad = sConfirmarCantidadMod;
						currentEnvio->sCalle = sConfirmarCalleMod;
						currentEnvio->sColonia = sConfirmarColoniaMod;
						currentEnvio->sCiudad = sConfirmarCiudadMod;
						currentEnvio->sMensaje = sConfirmarMensajeMod;
						currentEnvio->sNombreProducto = producto;
						currentEnvio->sEstado = estados;
						currentEnvio->sTotal = total;

						HWND hEnviosStatic = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_RESUMEN_ENVIOS), NULL, funcionResumenEnvio);
						ShowWindow(hEnviosStatic, SW_SHOW);
						DestroyWindow(hwnd);
					}
				}
										  break;
				case BTN_CALCULARTOTALMOD: {
					HWND hModificarEnvioProducto = GetDlgItem(hwnd, LBL_MOD_PROD_ENVIO);
					HWND hCantidad = GetDlgItem(hwnd, TXT_CANTIDADMOD);
					HWND hTotal = GetDlgItem(hwnd, LBL_MODTOTAL);

					string cantidad = getText(hCantidad);
					int indice = SendMessage(hModificarEnvioProducto, CB_GETCURSEL, NULL, NULL);

					if (cantidad.compare("") == 0) {
						MessageBox(hwnd, "Falta agregar información", "Error", MB_ICONWARNING);
						break;
					}
					else if (fValidarSoloNumeros(cantidad, cantidad.length())) {
						MessageBox(hwnd, "Solo puedes ingresar números en la cantidad de envios", "Error", MB_ICONERROR);
						break;
					}
					else {
						char producto[100];
						SendMessage(hModificarEnvioProducto, CB_GETLBTEXT, indice, (LPARAM)producto);
						int precio;
						while (auxiliarProducto != NULL) {
							if (auxiliarProducto->IDUsuario == userAccess->IDUsuario) {
								if (strcmp(producto, auxiliarProducto->sNombreProducto.c_str()) == 0) {

									precio = atof(auxiliarProducto->sPrecio.c_str());
									break;

								}
							}
							if (auxiliarProducto->sig == NULL) {
								break;
							}
							auxiliarProducto = auxiliarProducto->sig;
						}
						auxiliarProducto = origenProducto;

						int cantidadNueva = stoi(cantidad);
						int total = precio * cantidadNueva;
						char totalFinal[20];
						_itoa(total, totalFinal, 10);
						SendMessage(hTotal, WM_SETTEXT, NULL, (LPARAM)totalFinal);
					}
				}
				break;
				case BTN_CANCELAR_MOD_ENVIO: {
					HWND hSeptima = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionNuevoEnvio);
					ShowWindow(hSeptima, SW_SHOW);					
					DestroyWindow(hwnd);
				}
				break;
				case ID_INFO_VENDEDOR: {
					HWND hOctava = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INFO_VENDEDOR), NULL, funcionInfoVendedorStatic);
					ShowWindow(hOctava, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_ALTA_PRODUCTOS: {
					HWND hPrimera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ALTA_PRODUCTOS), NULL, funcionAltaProducto);
					ShowWindow(hPrimera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_BAJA_PRODUCTOS: {
					HWND hTercera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
					ShowWindow(hTercera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				
				case ID_NUEVO_ENVIO: {
					HWND hQuinta = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_NUEVO_ENVIO), NULL, funcionNuevoEnvio);
					ShowWindow(hQuinta, SW_SHOW);
					SetTimer(hQuinta, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_ENVIOS_ELIMINARENVIO: {
					HWND hSeptima = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionNuevoEnvio);
					ShowWindow(hSeptima, SW_SHOW);
					SetTimer(hSeptima, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_SALIR: {

					int retornoSioNo = MessageBox(hwnd, "¿Esta seguro que quiere salir?", "Aviso", MB_ICONQUESTION | MB_YESNO);
					if (retornoSioNo == 6) {

						save(auxiliar);
						saveProductos(auxiliarProducto);
						saveEnvios(auxiliarEnvios);

						exitWindowModEnvio = true;
						DestroyWindow(hwnd);
					}
					else {
						if (retornoSioNo == 7)
						break;
					}


				}
				break;
			}
		}
		 break;
		case WM_DESTROY:
			if (exitWindowModEnvio)
				PostQuitMessage(9);
			break;
	}
	return FALSE;
}

BOOL CALLBACK funcionEliminarEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		case WM_INITDIALOG: {
			fInstertarMenu(hwnd);
			HWND hNuevoEnvioProducto = GetDlgItem(hwnd, LBL_SELEC_PROD); //PONER
			HWND hNuevoEnvioEstados = GetDlgItem(hwnd, LBL_ESTADO); //PONER
			HWND hFecha = GetDlgItem(hwnd, IDC_DATETIMEPICKER1);
			HWND hTotal = GetDlgItem(hwnd, LBL_TOTALSTATIC);

			HWND hImagenStatic = GetDlgItem(hwnd, PB_FOTOENVIOS);
			HBITMAP hImagenBMPStatic = (HBITMAP)LoadImage(NULL, userAccess->foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);//HBITMAP ANTES DEL LOADIMAGE PARA INDICAR QUE SOLO QUIERO CARGAR BITMAP.
			SendMessage(hImagenStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagenBMPStatic);

			HWND hVendedorStatic = GetDlgItem(hwnd, LBL_NOMBREENVIOS);
			SetWindowText(hVendedorStatic, userAccess->sNombreVendedor.c_str());

			hNuevoEnvioProducto = GetDlgItem(hwnd, LBL_SELEC_PROD); //PONER
			hTxtNuevoEnvioCantidad = GetDlgItem(hwnd, LBL_CANT_PROD);
			hTxtNuevoEnvioCalle = GetDlgItem(hwnd, TXT_CALLE);
			hTxtNuevoEnvioColonia = GetDlgItem(hwnd, TXT_COLONIA);
			hTxtNuevoEnvioCiudad = GetDlgItem(hwnd, TXT_CIUDAD);
			hNuevoEnvioEstados = GetDlgItem(hwnd, LBL_ESTADO); //PONER
			hTxtNuevoEnvioMensaje = GetDlgItem(hwnd, TXT_MENSAJE_ENVIO);

			hListBoxEnvios = GetDlgItem(hwnd, LBX_ENVIOS);

			if (fechaObtenida) {

				if (origenEnvios != NULL) {
					int indice = 0;
					while (auxiliarEnvios != NULL) {
						if (userAccess->IDUsuario == auxiliarEnvios->IDUsuario) {
							string auxiliarListBox1;
							auxiliarListBox1.append("   Producto:");
							auxiliarListBox1.append(auxiliarEnvios->sCiudad);
							auxiliarListBox1.append("   Cantidad:");
							auxiliarListBox1.append(auxiliarEnvios->sCantidad);
							auxiliarListBox1.append("   Total:"); //AQUI
							auxiliarListBox1.append(auxiliarEnvios->sTotal);
							auxiliarListBox1.append("   Calle:");
							auxiliarListBox1.append(auxiliarEnvios->sCalle);
							auxiliarListBox1.append("   Colonia:");
							auxiliarListBox1.append(auxiliarEnvios->sColonia);
							auxiliarListBox1.append("   Ciudad:");
							auxiliarListBox1.append(auxiliarEnvios->sCiudad);
							auxiliarListBox1.append("   Estado:");
							auxiliarListBox1.append(auxiliarEnvios->sEstado);
							auxiliarListBox1.append("   Mensaje:");
							auxiliarListBox1.append(auxiliarEnvios->sMensaje);
							if (auxiliarEnvios->IDFecha > fechaActual) { //DE AQUI
								auxiliarListBox1.append("  Por enviar: ");
								string sFecha = auxiliarEnvios->sDia + "/" + auxiliarEnvios->sMes + "/" + auxiliarEnvios->sAnio;
								auxiliarListBox1.append(sFecha);
							}
							else if (auxiliarEnvios->IDFecha <= fechaActual) {
								auxiliarListBox1.append("  Enviado: ");
								string sFecha = auxiliarEnvios->sDia + "/" + auxiliarEnvios->sMes + "/" + auxiliarEnvios->sAnio;
								auxiliarListBox1.append(sFecha);
							}// HASTA AQUI
							SendMessage(hListBoxEnvios, LB_ADDSTRING, NULL, (LPARAM)auxiliarListBox1.c_str());
							SendMessage(hListBoxEnvios, LB_SETITEMDATA, (WPARAM)indice, (LPARAM)auxiliarEnvios->IDEnvios);
							indice++;
						}
						if (auxiliarEnvios->sig == NULL) {
							break;
						}
						auxiliarEnvios = auxiliarEnvios->sig;

					}
					auxiliarEnvios = origenEnvios;
				}
			}
		}
		break;
		case WM_TIMER: {  //DE AQUI
			if (fechaObtenida) {
				break;
			}
			time(&longSecondsUnix);
			structOFTime = localtime(&longSecondsUnix);

			char dia[5], mes[5], anio[5]; //El arreglo esta en 5 porque el dia y mes tienen 2 numeros y anio 4

			_itoa(structOFTime->tm_mday, dia, 10);
			_itoa(++structOFTime->tm_mon, mes, 10);
			_itoa(structOFTime->tm_year + 1900, anio, 10); //Se cuenta de 1900 en adelante (tiempo en base a unix)

			string sDia = dia;
			if (sDia.length() < 2) {
				sDia.clear();
				sDia.append("0");
				sDia.append(dia);
			}
			string sMes = mes;
			if (sMes.length() < 2) {
				sMes.clear();
				sMes.append("0");
				sMes.append(mes);
			}
			string sAnio = anio;
			string sFechaAcual = sAnio + sMes + sDia;
			fechaActual = stoi(sFechaAcual); //El stoi es para pasar de string a entero
			fechaObtenida = true;

			HWND hEnvios = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionEliminarEnvio);
			ShowWindow(hEnvios, SW_SHOW);
			SetTimer(hEnvios, MI_TIMER, 1000, (TIMERPROC)NULL);
			DestroyWindow(hwnd);

		}break;  //HASTA AQUI
		case WM_COMMAND: {
			switch (LOWORD(wparam)) {

				case BTN_ELIMINAR_ENVIOS: {

					auxiliarEnvios = origenEnvios;
					int indice = SendMessage(hListBoxEnvios, LB_GETCURSEL, NULL, NULL);
					int identificador = SendMessage(hListBoxEnvios, LB_GETITEMDATA, indice, NULL);

					if (indice < 0) {
						MessageBox(hwnd, "Seleccionar un envio", "Error", MB_ICONERROR);
					}
					else {

						int decidir = MessageBox(hwnd, "¿Esta seguro que quiere eliminar el envio?", "Advertencia", MB_YESNO | MB_ICONWARNING);
						if (decidir == 6) {
							bool searchFound = true;

							while (auxiliarEnvios->IDEnvios != identificador) { //Busco todas las matriculas hasta que una haga match
								if (auxiliarEnvios->sig == NULL) { //Llegue al final de la lista
									searchFound = false; //Es el unico lugar donde me indica que NO ENCONTRE lo que buscaba
									break; //Rompe un ciclo
								}
								auxiliarEnvios = auxiliarEnvios->sig; //Avanzo la lista
							}
							if (searchFound == false) { //Sabemos que el unico lugar donde se hace falso es en el if dentro del while ya que no lo encontro															
								auxiliarEnvios = origenEnvios;
								break;
							}
#pragma endregion
							if (auxiliarEnvios->prev == NULL && auxiliarEnvios->sig == NULL) { //El unico
								delete auxiliarEnvios; //Borramos el unico elemento
								auxiliarEnvios = origenEnvios = NULL; //Regresamos todo a NULL para que se cumpla el primer if de ALTA
							}
							else if (auxiliarEnvios->prev == NULL) { //Primero
								origenEnvios = origenEnvios->sig; //Movemos origen al siguiente elemento que ahora se convertira en el nuevo origen
								origenEnvios->prev = NULL; //El previo de este nuevo origen es null para indicarnos que es el primer elemento
								delete auxiliarEnvios; //Borramos aux
								auxiliarEnvios = origenEnvios; //Lo regresamos al nuevo origen
							}
							else if (auxiliarEnvios->sig == NULL) { //Ultimo
								auxiliarEnvios->prev->sig = NULL; // De aux (Ultimo) viajo a prev (Elemento anterior) y su sig lo apunto a NULL para indicar que ese es el nuevo ultimo elemento
								delete auxiliarEnvios; //Borro aux
								auxiliarEnvios = origenEnvios; //Lo regresamos a origen
							}
							else { //Cualquier otro
								auxiliarEnvios->prev->sig = auxiliarEnvios->sig;
								auxiliarEnvios->sig->prev = auxiliarEnvios->prev;
								delete auxiliarEnvios;
								auxiliarEnvios = origenEnvios;
							}

							HWND hEnvios = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ENVIOS), NULL, funcionEliminarEnvio);
							ShowWindow(hEnvios, SW_SHOW);
							SetTimer(hEnvios, MI_TIMER, 1000, (TIMERPROC)NULL);
							DestroyWindow(hwnd);

						}
						else {
							if (decidir == 7)
								break;
						}

					}

				}
				break;
				case BTN_EDITARENVIOS: {
					int posicion = SendMessage(hListBoxEnvios, LB_GETCURSEL, 0, 0);
					int codigo = SendMessage(hListBoxEnvios, LB_GETITEMDATA, posicion, 0);

					if (posicion < 0) {
						MessageBox(hwnd, "No ha seleccionado un producto", "Error", MB_ICONWARNING);
					}
					else {

						while (auxiliarEnvios != NULL) {
							if (codigo == auxiliarEnvios->IDEnvios) {
								currentEnvio = auxiliarEnvios;
								auxiliarEnvios = origenEnvios;
								HWND hOctava = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_MOD_ENVIO), NULL, funcionModEnvio);
								SetTimer(hOctava, MI_TIMER, 1000, (TIMERPROC)NULL);
								ShowWindow(hOctava, SW_SHOW);
								ShowWindow(hwnd, SW_HIDE);
								break;
							}
							auxiliarEnvios = auxiliarEnvios->sig;
						}

						auxiliarEnvios = origenEnvios;

					}
					
				}
				break;
				case BTN_MOSTRARENVIO: {

					int posicion = SendMessage(hListBoxEnvios, LB_GETCURSEL, 0, 0);
					int codigo = SendMessage(hListBoxEnvios, LB_GETITEMDATA, posicion, 0);

					if (posicion < 0) {
						MessageBox(hwnd, "No ha seleccionado un producto", "Error", MB_ICONWARNING);
					}
					else {

						while (auxiliarEnvios != NULL) {
							if (codigo == auxiliarEnvios->IDEnvios) {
								currentEnvio = auxiliarEnvios;
								auxiliarEnvios = origenEnvios;
								HWND hMostrarEnvio = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_RESUMEN_ENVIOS), NULL, funcionResumenEnvio);
								SetTimer(hMostrarEnvio, MI_TIMER, 1000, (TIMERPROC)NULL);
								ShowWindow(hMostrarEnvio, SW_SHOW);
								DestroyWindow(hwnd);
								break;
							}
							auxiliarEnvios = auxiliarEnvios->sig;
						}

						auxiliarEnvios = origenEnvios;

					}

				}
				break;
				case ID_INFO_VENDEDOR: {
					HWND hOctava = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_INFO_VENDEDOR), NULL, funcionInfoVendedorStatic);
					ShowWindow(hOctava, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				case ID_ALTA_PRODUCTOS: {
					HWND hPrimera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_ALTA_PRODUCTOS), NULL, funcionAltaProducto);
					ShowWindow(hPrimera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				
				break;
				case ID_BAJA_PRODUCTOS: {
					HWND hTercera = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_PRODUCTOS), NULL, funcionBajaProducto);
					ShowWindow(hTercera, SW_SHOW);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				
				case ID_NUEVO_ENVIO: {
					HWND hQuinta = CreateDialog(ghInst, MAKEINTRESOURCE(DLG_NUEVO_ENVIO), NULL, funcionNuevoEnvio);
					ShowWindow(hQuinta, SW_SHOW);
					SetTimer(hQuinta, MI_TIMER, 1000, (TIMERPROC)NULL);
					ShowWindow(hwnd, SW_HIDE);
					DestroyWindow(hwnd);
				}
				break;
				
				case ID_SALIR: {

					int retornoSioNo = MessageBox(hwnd, "¿Esta seguro que quiere salir?", "Aviso", MB_ICONQUESTION | MB_YESNO);
					if (retornoSioNo == 6) {


						save(auxiliar);
						saveProductos(auxiliarProducto);
						saveEnvios(auxiliarEnvios);
						exitWindowEliminarEnvio = true;
						DestroyWindow(hwnd);
					}
					else {
						if (retornoSioNo == 7)
						break;
					}


				}
				break;
			}
		}
		break;
		case WM_DESTROY:
			if (exitWindowEliminarEnvio)
				PostQuitMessage(9);
			break;
	}
	return FALSE;
}

