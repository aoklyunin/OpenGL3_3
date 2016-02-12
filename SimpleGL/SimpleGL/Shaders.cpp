#include "common_header.h"

#include "shaders.h"

// конструктор
CShader::CShader(){
	bLoaded = false;
}
/*-----------------------------------------------
Имя:	loadShader
Параметры:	sFile - путь к файлу
			a_iType - тип шейдера (фрагмент, точка, геометрия)
Результат:	Загружает и компилирует шейдеры
/*---------------------------------------------*/
bool CShader::LoadShader(string sFile, int a_iType){
	FILE* fp = fopen(sFile.c_str(), "rt");
	if(!fp)return false;
	// ПОлучаем строки из файла
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
Имя:	IsLoaded
Результат:	True если шейдер загружен
/*---------------------------------------------*/
bool CShader::IsLoaded(){
	return bLoaded;
}
/*-----------------------------------------------
Имя:	GetShaderID
Результат:	Возвращает ID созданного шейдера
/*---------------------------------------------*/
UINT CShader::GetShaderID(){
	return uiShader;
}
/*-----------------------------------------------
Имя:	DeleteShader
Результат:	Удаляет шейдер и очищает память GPU.
/*---------------------------------------------*/
void CShader::DeleteShader(){
	if(!IsLoaded())return;
	bLoaded = false;
	glDeleteShader(uiShader);
}
// конструктор программы шейдера
CShaderProgram::CShaderProgram(){
	bLinked = false;
}
/*-----------------------------------------------
Имя:	CreateProgram
Результат:	Создаёт программу
/*---------------------------------------------*/
void CShaderProgram::CreateProgram(){
	uiProgram = glCreateProgram();
}
/*-----------------------------------------------
Имя:	AddShaderToProgram
Параметры:	sShader - Шейдер на добавление
Результат:	Добавляется к программе скомпилированный шейдер
/*---------------------------------------------*/
bool CShaderProgram::AddShaderToProgram(CShader* shShader){
	if(!shShader->IsLoaded())return false;

	glAttachShader(uiProgram, shShader->GetShaderID());

	return true;
}
/*-----------------------------------------------
Имя:	LinkProgram
Результат:	Осуществляет линковку OpenGL программы.
/*---------------------------------------------*/
bool CShaderProgram::LinkProgram(){
	glLinkProgram(uiProgram);
	int iLinkStatus;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &iLinkStatus);
	bLinked = iLinkStatus == GL_TRUE;
	return bLinked;
}
/*-----------------------------------------------
Имя:	DeleteProgram
Результат:	Удаляет программу и освободдает память GPU
/*---------------------------------------------*/
void CShaderProgram::DeleteProgram(){
	if(!bLinked)return;
	bLinked = false;
	glDeleteProgram(uiProgram);
}
/*-----------------------------------------------
Имя:	UseProgram
Результат:	Говорит OpenGL использовать эту программу
/*---------------------------------------------*/
void CShaderProgram::UseProgram(){
	if(bLinked)glUseProgram(uiProgram);
}