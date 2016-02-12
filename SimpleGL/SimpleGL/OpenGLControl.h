#pragma once

#define SIMPLE_OPENGL_CLASS_NAME "Simple_openGL_class"

/********************************
	����� ��� ������ OpenGL
********************************/
class COpenGLControl{
public:
	// ������������� OpenGL
	bool InitOpenGL(HINSTANCE hInstance, HWND* a_hWnd, int iMajorVersion, int iMinorVersion, void (*a_InitScene)(LPVOID), void (*a_RenderScene)(LPVOID), void(*a_ReleaseScene)(LPVOID), LPVOID lpParam);
	// viewport OpenGL'� ������ �� ���� �����
	void ResizeOpenGLViewportFull();
	// ���������
	void Render(LPVOID lpParam);
	// �������� OpenGL
	void ReleaseOpenGLControl(LPVOID lpParam);
	// ����������� ������ OpenGL
	static void RegisterSimpleOpenGLClass(HINSTANCE hInstance);
	// ������������� ������ OpenGL
	static void UnregisterSimpleOpenGLClass(HINSTANCE hInstance);
	// ����� ������� �������� ���������
	void MakeCurrent();
	// ������ �������� � ������ ����� �������
	void SwapBuffersM();
private:
	// ������������� GLEW ����������
	bool InitGLEW(HINSTANCE hInstance);

	HDC hDC; 
	HWND* hWnd;
	HGLRC hRC;
	static bool bClassRegistered;
	static bool bGlewInitialized;
	int iMajorVersion, iMinorVersion;

	void (*InitScene)(LPVOID lpParam), (*RenderScene)(LPVOID lpParam), (*ReleaseScene)(LPVOID lpParam);
};
// ���������� ��������� Windows
LRESULT CALLBACK MsgHandlerSimpleOpenGLClass(HWND, UINT, WPARAM, LPARAM);