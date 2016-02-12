#pragma once

#include "openGLControl.h"

// класс приложени€
class COpenGLWinApp
{
public:
	HWND hWnd; // экземпл€р окна
	COpenGLControl oglControl; // класс OpenGL контролл€

	void ResetTimer();
	void UpdateTimer();
	float sof(float fVal);

	bool InitializeApp(string a_sAppName);
	void RegisterAppClass(HINSTANCE hAppInstance);
	bool CreateAppWindow(string sTitle);
	
	void AppBody();
	void Shutdown();

	HINSTANCE GetInstance();

	LRESULT CALLBACK MsgHandlerMain(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE hInstance; // Ёкземпл€р приложени€
	string sAppName;
	HANDLE hMutex;

	bool bAppActive; // ƒл€ праверки, активно ли приложение
	DWORD dwLastFrame;
	float fFrameInterval;
};

namespace Keys
{
	int Key(int iKey);
	int Onekey(int iKey);
	extern char kp[256];
}

extern COpenGLWinApp appMain;

void InitScene(LPVOID), RenderScene(LPVOID), ReleaseScene(LPVOID);