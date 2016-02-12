#include "common_header.h"
#include "win_OpenGLApp.h"
#include "shaders.h"

float fTriangle[9]; // ���������� ����� ������������
float fQuad[12]; // ���������� ����� �������
float fTriangleColor[9]; // ����� ����� ������������
float fQuadColor[12]; // ����� ����� ��������

UINT uiVBO[4]; // VBO Vertex Buffer Object
			   // ������ �����	
UINT uiVAO[2]; // VAO Vertex Array Object
			   // ������ �����	

CShader shVertex, shFragment;
CShaderProgram spMain;
/*-----------------------------------------------
���:		initScene
��������:	lpParam - ��������� �� ��� ������
Result:		�������������� OpenGL 
/*---------------------------------------------*/
void InitScene(LPVOID lpParam){
	// ���� ������� 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// ����� ���������� ������ ������������
	fTriangle[0] = -0.4f; fTriangle[1] = 0.1f; fTriangle[2] = 0.0f;
	fTriangle[3] = 0.4f;  fTriangle[4] = 0.1f; fTriangle[5] = 0.0f;
	fTriangle[6] = 0.0f;  fTriangle[7] = 0.7f; fTriangle[8] = 0.0f;
	// ����� ����� ������ ������������
	fTriangleColor[0] = 1.0f; fTriangleColor[1] = 0.0f; fTriangleColor[2] = 0.0f;
	fTriangleColor[3] = 0.0f; fTriangleColor[4] = 1.0f; fTriangleColor[5] = 0.0f;
	fTriangleColor[6] = 0.0f; fTriangleColor[7] = 0.0f; fTriangleColor[8] = 1.0f;
	// ����� ���������� ������ ��������
	fQuad[0] = -0.2f; fQuad[1] = -0.1f;  fQuad[2] = 0.0f;
	fQuad[3] = -0.2f; fQuad[4] = -0.6f;  fQuad[5] = 0.0f;
	fQuad[6] = 0.2f;  fQuad[7] = -0.1f;  fQuad[8] = 0.0f;
	fQuad[9] = 0.2f;  fQuad[10] = -0.6f; fQuad[11] = 0.0f;
	// ����� ����� ������ ��������
	fQuadColor[0] = 1.0f; fQuadColor[1] = 0.0f;  fQuadColor[2] = 0.0f;
	fQuadColor[3] = 0.0f; fQuadColor[4] = 1.0f;  fQuadColor[8] = 0.0f;
	fQuadColor[6] = 0.0f; fQuadColor[7] = 0.0f;  fQuadColor[5] = 1.0f;
	fQuadColor[9] = 1.0f; fQuadColor[10] = 1.0f; fQuadColor[11] = 0.0f;
	// ��� ������� ���������� ������� � ������ � ������ ������� �� ��������������
	glGenVertexArrays(2, uiVAO); // ���������� ��� VAO, ���� ��� ������������, ���� ��� ��������
	glGenBuffers(4, uiVBO);		 // � ������ VBO

	// �������, ��� ����� �������� � ������� ������������
	glBindVertexArray(uiVAO[0]);
	// ��������� ����� ��������� ������ ������������
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// ��������� ����� ������ ������������
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//  �������, ��� ����� �������� � ������� ��������
	glBindVertexArray(uiVAO[1]);
	// ��������� ����� ��������� ������ ��������
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// ��������� ����� ������ ��������
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuadColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// ��������� �������
	shVertex.LoadShader("data\\shaders\\shader.vert", GL_VERTEX_SHADER);
	shFragment.LoadShader("data\\shaders\\shader.frag", GL_FRAGMENT_SHADER);

	spMain.CreateProgram();
	spMain.AddShaderToProgram(&shVertex);
	spMain.AddShaderToProgram(&shFragment);

	spMain.LinkProgram();
	spMain.UseProgram();
}
/*-----------------------------------------------
���:	RenderScene
���������:	lpParam - ���������, �� ��� ������
���������:	��������� �����
/*---------------------------------------------*/
void RenderScene(LPVOID lpParam){
	// ��������������� ��������� � ��������� �� ���������� OpanGL
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;
	// ������� �����
	glClear(GL_COLOR_BUFFER_BIT);
	// �������� �������� ������ ������������
	glBindVertexArray(uiVAO[0]);
	// ������ ����������� �� ������ ��� ������
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//  �������� �������� ������ ��������
	glBindVertexArray(uiVAO[1]);
	// ������ ��� ������������
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// ������ ������
	oglControl->SwapBuffersM();
}
/*-----------------------------------------------
���:	ReleaseScene
���������:	lpParam - ���������, �� ��� ������
���������:	����������� �����
/*---------------------------------------------*/
void ReleaseScene(LPVOID lpParam){
	spMain.DeleteProgram();
	shVertex.DeleteShader();
	shFragment.DeleteShader();
}