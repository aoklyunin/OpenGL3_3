#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

#include "common_header.h"
#include "OpenGLControl.h"
#include <gl/wglew.h>

bool COpenGLControl::bClassRegistered = false, COpenGLControl::bGlewInitialized = false;
/*-----------------------------------------------
���:		InitGLEW
���������:	������ ��������� ���� � �������� OpenGL ���������,
			��� ������ ������� ����� ��������������� GLEW.
/*---------------------------------------------*/
bool COpenGLControl::InitGLEW(HINSTANCE hInstance){
	// ���� GLEW ��� ��� ���������������, ���������� true
	if(bGlewInitialized)return true;
	// ������������ OpenGL �����
	RegisterSimpleOpenGLClass(hInstance);
	// ������ ��������� ����
	HWND hWndFake = CreateWindow(SIMPLE_OPENGL_CLASS_NAME, "FAKE", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, hInstance, NULL);
	// �������� �������� ����
	hDC = GetDC(hWndFake);
	// ����� ������ ��������
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
	// ���� �� ���������� ������ ������ ��������, ����� false
	if (iPixelFormat == 0)return false;
	if(!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;
	// ����� �������� ������ � �������� OpenGL, 
	// �������� ��������� ������ ��������
	HGLRC hRCFake = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRCFake);
	bool bResult = true;
	// �������������� GLEW
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
��������:	InitOpenGL
���������:	hInstance - ��������� ����������
			a_hWnd - ����, � ������� ����� ��������������� OpenGL
			a_iMajorVersion - ������������ ������ OpenGL
			a_iMinorVersion - ������������ ������ OpenGL
			a_initScene - ��������� �� ������� �������������
			a_renderScene - ��������� �� ������� ���������
			a_releaseScene - ��������� �� ������� ����������
���������:	�������������� �������� OpenGL ��� ���������.
			���������� true � ������ �����
/*---------------------------------------------*/
bool COpenGLControl::InitOpenGL(HINSTANCE hInstance, HWND* a_hWnd, int iMajorVersion, int iMinorVersion, void (*a_InitScene)(LPVOID), void (*a_RenderScene)(LPVOID), void(*a_ReleaseScene)(LPVOID), LPVOID lpParam){
	// ���� �� ���������� ���������������� GLEW, ���������� false
	if(!InitGLEW(hInstance))return false;
	// ��������� ����
	hWnd = a_hWnd;
	hDC = GetDC(*hWnd);
	// ���� ������
	bool bError = false;
	// ����� ������ ��������
	PIXELFORMATDESCRIPTOR pfd;
	// ���� ������ OpenGL ������ 2
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

		// ������ ������ �������� ���������
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
			0 // ����� ������ ���������
		};
		int iContextAttribs[] ={
			WGL_CONTEXT_MAJOR_VERSION_ARB, iMajorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, iMinorVersion,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0 // ����� ������ ���������
		};
		int iPixelFormat, iNumFormats;
		wglChoosePixelFormatARB(hDC, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);
		// ���� �� ���������� ������ ������ �������� ���������� false
		if(!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;
		hRC = wglCreateContextAttribsARB(hDC, 0, iContextAttribs);
		//���� �� ��� ��� ����
		if(hRC) wglMakeCurrent(hDC, hRC);
		else bError = true;
	}else 
		bError = true;
	if(bError){
		// ��������� ��������� �� ������
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
���:	ResizeOpenGLViewportFull
���������:	������������� viewport � �����������
/*---------------------------------------------*/
void COpenGLControl::ResizeOpenGLViewportFull(){
	if(hWnd == NULL)return;
	RECT rRect; GetClientRect(*hWnd, &rRect);
	glViewport(0, 0, rRect.right, rRect.bottom);
}
/*-----------------------------------------------
���:		RegisterSimpleOpenGLClass
���������:	hInstance - ��������� ����������
���������:	������������ OpenGL �����
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
���:		UnregisterSimpleOpenGLClass
���������:	hInstance - ��������� ����������
���������:	�������������� OpenGL �����
/*---------------------------------------------*/
void COpenGLControl::UnregisterSimpleOpenGLClass(HINSTANCE hInstance){
	if(bClassRegistered){
		UnregisterClass(SIMPLE_OPENGL_CLASS_NAME, hInstance);
		bClassRegistered = false;
	}
}
/*-----------------------------------------------
���:	MsgHandlerSimpleOpenGLClass
���������:	���������� ��������� Windows
���������:	���������� ��������� Windows
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
���:	SwapBuffersM
���������:	������ �������� � ������ ����� �������
/*---------------------------------------------*/
void COpenGLControl::SwapBuffersM(){
	SwapBuffers(hDC);
}
/*-----------------------------------------------
���:	MakeCurrent
���������:	����� ������� �������� ���������
/*---------------------------------------------*/
void COpenGLControl::MakeCurrent(){
	wglMakeCurrent(hDC, hRC);
}
/*-----------------------------------------------
���:	Render
��������:	lpParam - ��������� �� ��� ������
���������:	�������� �������� ����� ������� ���������
/*---------------------------------------------*/
void COpenGLControl::Render(LPVOID lpParam){
	if(RenderScene)RenderScene(lpParam);
}
/*-----------------------------------------------
���:	ReleaseOpenGLControl
��������:	lpParam - ��������� �� ��� ������
���������:  �������� �������� ����� ������� ����������
/*---------------------------------------------*/
void COpenGLControl::ReleaseOpenGLControl(LPVOID lpParam){
	if(ReleaseScene)ReleaseScene(lpParam);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(*hWnd, hDC);

	hWnd = NULL;
}