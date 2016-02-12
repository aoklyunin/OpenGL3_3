#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

#include "common_header.h"
#include "OpenGLControl.h"
#include <gl/wglew.h>

bool COpenGLControl::bClassRegistered = false, COpenGLControl::bGlewInitialized = false;
/*-----------------------------------------------
Имя:		InitGLEW
Результат:	Создаёт временное окно и контекст OpenGL рисования,
			при помози которых будет инициализирован GLEW.
/*---------------------------------------------*/
bool COpenGLControl::InitGLEW(HINSTANCE hInstance){
	// если GLEW уже был инициализирован, возвращаем true
	if(bGlewInitialized)return true;
	// регистрируем OpenGL класс
	RegisterSimpleOpenGLClass(hInstance);
	// создаём временное окно
	HWND hWndFake = CreateWindow(SIMPLE_OPENGL_CLASS_NAME, "FAKE", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, hInstance, NULL);
	// получаем контекст окна
	hDC = GetDC(hWndFake);
	// задаём формат пикселей
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
	// если не получилось задать формат пикселей, выдаём false
	if (iPixelFormat == 0)return false;
	if(!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;
	// Чтобы получить доступ к функциям OpenGL, 
	// Получаем временный старый контекст
	HGLRC hRCFake = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRCFake);
	bool bResult = true;
	// инициализируем GLEW
	if(!bGlewInitialized){
		if(glewInit() != GLEW_OK){
			MessageBox(*hWnd, "Couldn't initialize GLEW!", "Fatal Error", MB_ICONERROR);
			bResult = false;
		}
		bGlewInitialized = true;
	}
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRCFake);
	DestroyWindow(hWndFake);
	return bResult;
}
/*-----------------------------------------------
Название:	InitOpenGL
Параметры:	hInstance - экземпляр приложения
			a_hWnd - окно, в котором будет инициализирован OpenGL
			a_iMajorVersion - максимальная версия OpenGL
			a_iMinorVersion - миниимальная версия OpenGL
			a_initScene - указатель на функцию инициализации
			a_renderScene - указатель на функцию рисования
			a_releaseScene - указатель на функцию завершения
Результат:	Инициализирует контекст OpenGL для рисования.
			Возвращает true в случае удачи
/*---------------------------------------------*/
bool COpenGLControl::InitOpenGL(HINSTANCE hInstance, HWND* a_hWnd, int iMajorVersion, int iMinorVersion, void (*a_InitScene)(LPVOID), void (*a_RenderScene)(LPVOID), void(*a_ReleaseScene)(LPVOID), LPVOID lpParam){
	// если не получилось инициализировать GLEW, возвращаем false
	if(!InitGLEW(hInstance))return false;
	// контексты окна
	hWnd = a_hWnd;
	hDC = GetDC(*hWnd);
	// флаг ошибки
	bool bError = false;
	// задаём формат пикселей
	PIXELFORMATDESCRIPTOR pfd;
	// если версия OpenGL меньше 2
	if(iMajorVersion <= 2){
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion   = 1;
		pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
		if (iPixelFormat == 0)return false;

		if(!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;

		// Создаём старый контекст рисования
		hRC = wglCreateContext(hDC);
		if(hRC)wglMakeCurrent(hDC, hRC);
		else bError = true;
	}else if(WGLEW_ARB_create_context && WGLEW_ARB_pixel_format){
		const int iPixelFormatAttribList[] ={
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0 // Конец списка атрибутов
		};
		int iContextAttribs[] ={
			WGL_CONTEXT_MAJOR_VERSION_ARB, iMajorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, iMinorVersion,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0 // конец списка атрибутов
		};
		int iPixelFormat, iNumFormats;
		wglChoosePixelFormatARB(hDC, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);
		// Если не получилось задать формат пикселей возвращаем false
		if(!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;
		hRC = wglCreateContextAttribsARB(hDC, 0, iContextAttribs);
		//Если всё идёт как надо
		if(hRC) wglMakeCurrent(hDC, hRC);
		else bError = true;
	}else 
		bError = true;
	if(bError){
		// Формируем сообщение об ошибке
		char sErrorMessage[255], sErrorTitle[255];
		sprintf(sErrorMessage, "OpenGL %d.%d is not supported! Please download latest GPU drivers!", iMajorVersion, iMinorVersion);
		sprintf(sErrorTitle, "OpenGL %d.%d Not Supported", iMajorVersion, iMinorVersion);
		MessageBox(*hWnd, sErrorMessage, sErrorTitle, MB_ICONINFORMATION);
		return false;
	}
	RenderScene = a_RenderScene;
	InitScene = a_InitScene;
	ReleaseScene = a_ReleaseScene;
	if(InitScene != NULL)InitScene(lpParam);
	return true;
}
/*-----------------------------------------------
Имя:	ResizeOpenGLViewportFull
Результат:	Пересчитывает viewport и перспективу
/*---------------------------------------------*/
void COpenGLControl::ResizeOpenGLViewportFull(){
	if(hWnd == NULL)return;
	RECT rRect; GetClientRect(*hWnd, &rRect);
	glViewport(0, 0, rRect.right, rRect.bottom);
}
/*-----------------------------------------------
Имя:		RegisterSimpleOpenGLClass
Параметры:	hInstance - экземпляр приложения
Результат:	Регистрирует OpenGL класс
/*---------------------------------------------*/
void COpenGLControl::RegisterSimpleOpenGLClass(HINSTANCE hInstance){
	if(bClassRegistered)return;
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style =  CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = (WNDPROC)MsgHandlerSimpleOpenGLClass;
	wc.cbClsExtra = 0; wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_MENUBAR+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = SIMPLE_OPENGL_CLASS_NAME;

	RegisterClassEx(&wc);

	bClassRegistered = true;
}
/*-----------------------------------------------
Имя:		UnregisterSimpleOpenGLClass
Параметры:	hInstance - экземпляр приложения
Результат:	Дерегистрирует OpenGL класс
/*---------------------------------------------*/
void COpenGLControl::UnregisterSimpleOpenGLClass(HINSTANCE hInstance){
	if(bClassRegistered){
		UnregisterClass(SIMPLE_OPENGL_CLASS_NAME, hInstance);
		bClassRegistered = false;
	}
}
/*-----------------------------------------------
Имя:	MsgHandlerSimpleOpenGLClass
Параметры:	Переменные сообщения Windows
Результат:	обработчик сообщений Windows
/*---------------------------------------------*/
LRESULT CALLBACK MsgHandlerSimpleOpenGLClass(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT ps;
	switch(uiMsg){
		case WM_PAINT:									
			BeginPaint(hWnd, &ps);							
			EndPaint(hWnd, &ps);					
			break;
		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam); // Default window procedure
	}
	return 0;
}
/*-----------------------------------------------
Имя:	SwapBuffersM
Результат:	меняет передний и задний буфер местами
/*---------------------------------------------*/
void COpenGLControl::SwapBuffersM(){
	SwapBuffers(hDC);
}
/*-----------------------------------------------
Имя:	MakeCurrent
Результат:	задаёт текущий контекст рисования
/*---------------------------------------------*/
void COpenGLControl::MakeCurrent(){
	wglMakeCurrent(hDC, hRC);
}
/*-----------------------------------------------
Имя:	Render
Параметр:	lpParam - указатель на что угодно
Результат:	Вызывает заданную ранее функцию рисования
/*---------------------------------------------*/
void COpenGLControl::Render(LPVOID lpParam){
	if(RenderScene)RenderScene(lpParam);
}
/*-----------------------------------------------
Имя:	ReleaseOpenGLControl
Параметр:	lpParam - указатель на что угодно
Результат:  Вызывает заданную ранее функцию завершения
/*---------------------------------------------*/
void COpenGLControl::ReleaseOpenGLControl(LPVOID lpParam){
	if(ReleaseScene)ReleaseScene(lpParam);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(*hWnd, hDC);

	hWnd = NULL;
}