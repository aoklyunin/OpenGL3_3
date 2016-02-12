#include "common_header.h"

#include "shaders.h"

// �����������
CShader::CShader(){
	bLoaded = false;
}
/*-----------------------------------------------
���:	loadShader
���������:	sFile - ���� � �����
			a_iType - ��� ������� (��������, �����, ���������)
���������:	��������� � ����������� �������
/*---------------------------------------------*/
bool CShader::LoadShader(string sFile, int a_iType){
	FILE* fp = fopen(sFile.c_str(), "rt");
	if(!fp)return false;
	// �������� ������ �� �����
	vector<string> sLines;
	char sLine[255];
	while(fgets(sLine, 255, fp))sLines.push_back(sLine);
	fclose(fp);

	const char** sProgram = new const char*[ESZ(sLines)];
	FOR(i, ESZ(sLines))sProgram[i] = sLines[i].c_str();
	
	uiShader = glCreateShader(a_iType);

	glShaderSource(uiShader, ESZ(sLines), sProgram, NULL);
	glCompileShader(uiShader);

	delete[] sProgram;

	int iCompilationStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iCompilationStatus);

	if(iCompilationStatus == GL_FALSE)return false;
	iType = a_iType;
	bLoaded = true;

	return 1;
}
/*-----------------------------------------------
���:	IsLoaded
���������:	True ���� ������ ��������
/*---------------------------------------------*/
bool CShader::IsLoaded(){
	return bLoaded;
}
/*-----------------------------------------------
���:	GetShaderID
���������:	���������� ID ���������� �������
/*---------------------------------------------*/
UINT CShader::GetShaderID(){
	return uiShader;
}
/*-----------------------------------------------
���:	DeleteShader
���������:	������� ������ � ������� ������ GPU.
/*---------------------------------------------*/
void CShader::DeleteShader(){
	if(!IsLoaded())return;
	bLoaded = false;
	glDeleteShader(uiShader);
}
// ����������� ��������� �������
CShaderProgram::CShaderProgram(){
	bLinked = false;
}
/*-----------------------------------------------
���:	CreateProgram
���������:	������ ���������
/*---------------------------------------------*/
void CShaderProgram::CreateProgram(){
	uiProgram = glCreateProgram();
}
/*-----------------------------------------------
���:	AddShaderToProgram
���������:	sShader - ������ �� ����������
���������:	����������� � ��������� ���������������� ������
/*---------------------------------------------*/
bool CShaderProgram::AddShaderToProgram(CShader* shShader){
	if(!shShader->IsLoaded())return false;

	glAttachShader(uiProgram, shShader->GetShaderID());

	return true;
}
/*-----------------------------------------------
���:	LinkProgram
���������:	������������ �������� OpenGL ���������.
/*---------------------------------------------*/
bool CShaderProgram::LinkProgram(){
	glLinkProgram(uiProgram);
	int iLinkStatus;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &iLinkStatus);
	bLinked = iLinkStatus == GL_TRUE;
	return bLinked;
}
/*-----------------------------------------------
���:	DeleteProgram
���������:	������� ��������� � ����������� ������ GPU
/*---------------------------------------------*/
void CShaderProgram::DeleteProgram(){
	if(!bLinked)return;
	bLinked = false;
	glDeleteProgram(uiProgram);
}
/*-----------------------------------------------
���:	UseProgram
���������:	������� OpenGL ������������ ��� ���������
/*---------------------------------------------*/
void CShaderProgram::UseProgram(){
	if(bLinked)glUseProgram(uiProgram);
}