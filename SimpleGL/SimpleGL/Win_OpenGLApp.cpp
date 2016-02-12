#include "common_header.h"

#include "win_OpenGLApp.h"

// ���������� ���������� OpenGl
COpenGLWinApp appMain;
// ������ ����������
char Keys::kp[256] = {0};

/*-----------------------------------------------
���:	Key
���������:	iKey - ����������� ��� �������
���������:	���������� true, ���� ������ ������
/*---------------------------------------------*/
int Keys::Key(int iKey){
	return (GetAsyncKeyState(iKey)>>15)&1;
}
/*-----------------------------------------------
���:	Onekey
���������:	iKey - ����������� ��� �������
���������:	���������� true, ���� ������ ������ ������� ���
/*---------------------------------------------*/
int Keys::Onekey(int iKey){
	if(Key(iKey) && !kp[iKey]){kp[iKey] = 1; return 1;}
	if(!Key(iKey))kp[iKey] = 0;
	return 0;
}
/*-----------------------------------------------
���:	����� �������
���������:	���������� ������ ����������
/*---------------------------------------------*/
void COpenGLWinApp::ResetTimer(){
	dwLastFrame = GetTickCount();
	fFrameInterval = 0.0f;
}
/*-----------------------------------------------
���:	UpdateTimer
���������:	none
���������:	���������� �������
/*---------------------------------------------*/
void COpenGLWinApp::UpdateTimer(){
	DWORD dwCur = GetTickCount();
	fFrameInterval = float(dwCur-dwLastFrame)*0.001f;
	dwLastFrame = dwCur;
}
/*-----------------------------------------------
���:	sof
���������:	fVal - ��������
���������:	������������ �������� ��� ��������
/*---------------------------------------------*/
float COpenGLWinApp::sof(float fVal){
	return fVal*fFrameInterval;
}
/*-----------------------------------------------
���:	InitializeApp
���������:	a_sAppName - ��� ����������
���������:	������������� ����������
/*---------------------------------------------*/
bool COpenGLWinApp::InitializeApp(string a_sAppName){
	sAppName = a_sAppName;
	hMutex = CreateMutex(NULL, 1, sAppName.c_str());
	if(GetLastError() == ERROR_ALREADY_EXISTS){
		MessageBox(NULL, "This application already runs!", "Multiple Instances Found.", MB_ICONINFORMATION | MB_OK);
		return 0;
	}
	return 1;
}
/*-----------------------------------------------
���:	RegisterAppClass
���������:	a_hInstance - ��������� ����������
���������:	������������ ����� ���������� window
/*---------------------------------------------*/
LRESULT CALLBACK GlobalMessageHandler(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam){
	return appMain.MsgHandlerMain(hWnd, uiMsg, wParam, lParam);
}

void COpenGLWinApp::RegisterAppClass(HINSTANCE a_hInstance){
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;

	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	wcex.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hIconSm = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hInstance = hInstance;

	wcex.lpfnWndProc = GlobalMessageHandler;
	wcex.lpszClassName = sAppName.c_str();

	wcex.lpszMenuName = NULL;

	RegisterClassEx(&wcex);
}
/*-----------------------------------------------
���:		CreateAppWindow
���������:	sTitle - ��������� ����
���������:	������ ������� ���� ����������
/*---------------------------------------------*/
bool COpenGLWinApp::CreateAppWindow(string sTitle){
	hWnd = CreateWindowEx(0, sAppName.c_str(), sTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, hInstance, NULL);
	if(!oglControl.InitOpenGL(hInstance, &hWnd, 3, 1, InitScene, RenderScene, NULL, &oglControl))return false;
	ShowWindow(hWnd, SW_SHOW);
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hWnd);
	return true;
}
/*-----------------------------------------------
���:	AppBody
���������:	������� ���� ����������
/*---------------------------------------------*/
void COpenGLWinApp::AppBody(){
	MSG msg;
	while(1){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message == WM_QUIT)break; // ���� ��������� WM_QUIT ������� �� ����������
			else{
				TranslateMessage(&msg); // ��������� ����������
				DispatchMessage(&msg);
			}
		}
		else if(bAppActive){
			UpdateTimer();
			oglControl.Render(&oglControl);
		}
		else Sleep(200); // ������ ��������, ����� �� ��������� ��������
	}
}
/*-----------------------------------------------
���:		����������
���������:	��������� ���������� � ���������� �����
/*---------------------------------------------*/
void COpenGLWinApp::Shutdown(){
	oglControl.ReleaseOpenGLControl(&oglControl);

	DestroyWindow(hWnd);
	UnregisterClass(sAppName.c_str(), hInstance);
	COpenGLControl::UnregisterSimpleOpenGLClass(hInstance);
	ReleaseMutex(hMutex);
}
/*-----------------------------------------------
���:		MsgHandlerMain
��������:	��������� windows 
���������:	���������� ��������� windows
/*---------------------------------------------*/
LRESULT CALLBACK COpenGLWinApp::MsgHandlerMain(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT ps;
	switch(uiMsg){
		case WM_PAINT:
			BeginPaint(hWnd, &ps);					
			EndPaint(hWnd, &ps);
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_ACTIVATE:{
			switch(LOWORD(wParam)){
				case WA_ACTIVE:
				case WA_CLICKACTIVE:
					bAppActive = true;
					break;
				case WA_INACTIVE:
					bAppActive = false;
					break;
			}
			break;
		}
		case WM_SIZE:
			oglControl.ResizeOpenGLViewportFull();
			break;
		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
	return 0;
}
/*-----------------------------------------------
���:	GetInstance
���������:	���������� �������� ����������
/*---------------------------------------------*/
HINSTANCE COpenGLWinApp::GetInstance(){
	return hInstance;
}
/*-----------------------------------------------
���:		WinMain
���������:	������� Windows  �������
/*---------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow){
	if(!appMain.InitializeApp("simple_opengl"))
		return 0;
	appMain.RegisterAppClass(hInstance);

	if(!appMain.CreateAppWindow("Simple OpenGL"))
		return 0;
	appMain.ResetTimer();

	appMain.AppBody();
	appMain.Shutdown();

	return 0;
}

