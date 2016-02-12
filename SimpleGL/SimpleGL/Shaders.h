#pragma once

/********************************
�����:	CShader
����:	�������� OpenGL �������� � �� ����������
********************************/
class CShader{
public:
	bool LoadShader(string sFile, int a_iType);
	void DeleteShader();

	bool IsLoaded();
	UINT GetShaderID();

	CShader();
private:
	UINT uiShader; // ID  �������
	int iType;     // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
	bool bLoaded;  // ���� ������ ��� �������� � �������������
};
/********************************
�����:	CShaderProgram
Purpose: OpenGL ��������� ��������
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
	UINT uiProgram; // ID ���������
	bool bLinked;   // ���������� �� ��������� � ������ �� � �������
};