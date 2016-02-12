#pragma once

/********************************
 ласс:	CShader
÷ель:	загрузка OpenGL шейдеров и их компил€ци€
********************************/
class CShader{
public:
	bool LoadShader(string sFile, int a_iType);
	void DeleteShader();

	bool IsLoaded();
	UINT GetShaderID();

	CShader();
private:
	UINT uiShader; // ID  шейдера
	int iType;     // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
	bool bLoaded;  // если шейдер был загружен и скомпилирован
};
/********************************
 ласс:	CShaderProgram
Purpose: OpenGL программа шейдеров
********************************/
class CShaderProgram{
public:
	void CreateProgram();
	void DeleteProgram();
	bool AddShaderToProgram(CShader* shShader);
	bool LinkProgram();
	void UseProgram();
	UINT GetProgramID();
	CShaderProgram();
private:
	UINT uiProgram; // ID программы
	bool bLinked;   // —линкована ли программа и готова ли к запуску
};