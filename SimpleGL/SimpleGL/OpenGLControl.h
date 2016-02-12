#pragma once

#define SIMPLE_OPENGL_CLASS_NAME "Simple_openGL_class"

/********************************
	 ласс дл€ работы OpenGL
********************************/
class COpenGLControl{
public:
	// инициализаци€ OpenGL
	bool InitOpenGL(HINSTANCE hInstance, HWND* a_hWnd, int iMajorVersion, int iMinorVersion, void (*a_InitScene)(LPVOID), void (*a_RenderScene)(LPVOID), void(*a_ReleaseScene)(LPVOID), LPVOID lpParam);
	// viewport OpenGL'а делает на весь экран
	void ResizeOpenGLViewportFull();
	// рисование
	void Render(LPVOID lpParam);
	// очищение OpenGL
	void ReleaseOpenGLControl(LPVOID lpParam);
	// регистраци€ класса OpenGL
	static void RegisterSimpleOpenGLClass(HINSTANCE hInstance);
	// дерегистраци€ класса OpenGL
	static void UnregisterSimpleOpenGLClass(HINSTANCE hInstance);
	// задаЄт текущий контекст рисовани€
	void MakeCurrent();
	// мен€ет передний и задний буфер местами
	void SwapBuffersM();
private:
	// инициализаци€ GLEW библиотеки
	bool InitGLEW(HINSTANCE hInstance);

	HDC hDC; 
	HWND* hWnd;
	HGLRC hRC;
	static bool bClassRegistered;
	static bool bGlewInitialized;
	int iMajorVersion, iMinorVersion;

	void (*InitScene)(LPVOID lpParam), (*RenderScene)(LPVOID lpParam), (*ReleaseScene)(LPVOID lpParam);
};
// обработчик сообщений Windows
LRESULT CALLBACK MsgHandlerSimpleOpenGLClass(HWND, UINT, WPARAM, LPARAM);