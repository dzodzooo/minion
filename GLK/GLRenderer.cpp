#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"


#define PI 3.1415

CGLRenderer::CGLRenderer(void)
{
	world_angle_x = 0;
	world_angle_y = 0;
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC) {
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC) {
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glClearColor(0.2, 0.3, 0.2, 0);
	glEnable(GL_DEPTH_TEST);

	wglMakeCurrent(NULL, NULL);
}



void CGLRenderer::Reshape(CDC* pDC, int w, int h) {
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double)h, 1, 100);
	gluLookAt(0, 0, -20, 0.0, -2, 0.0, 0.0, 1.0, 0.0);
	//	The gluLookAt function takes three vectors : 
	//	-the eye position,
	//	-the center of the scene you are looking at,
	//	-and the up vector which specifies the direction that you consider to be "up" in your scene.
	glMatrixMode(GL_MODELVIEW);

	wglMakeCurrent(NULL, NULL);
}


void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawScene(CDC* pDC) {
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glRotatef(world_angle_y, -1, 0, 0);
	glRotatef(world_angle_x, 0, 1, 0);
	DrawMinion();



	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawMinion() {
	DrawBody();
	DrawFace();
	DrawLegs();
	DrawHands();
}
void CGLRenderer::DrawFace() {
	glColor3f(0,0,0);
	DrawCylinder(2.51, 2.51, 2.0/3, 30,1);
	glTranslatef(0, 0, -2.5);
	DrawEyes();
	glTranslatef(0, 0, 2.5);
}
void CGLRenderer::DrawBody() {
	glColor3f(1, 216 / 255.0, 0);
	glTranslatef(0, -1.5, 0);
	DrawCylinder(2.5, 2.5, 4.5, 30,1);
	glTranslatef(0, 2.25, 0);
	DrawSpherePart(2, 2.5, 30, 0.5);
	glTranslatef(0, -2.25, 0);

	glRotatef(180, 1, 0, 0);
	glTranslatef(0, 2.25, 0);
	DrawSpherePart(1.5, 2.5, 30, 0.5);
	glTranslatef(0, -2.25, 0);
	glRotatef(-180, 1, 0, 0);
	glTranslatef(0, 1.5, 0);
}
void CGLRenderer::DrawEyes() {
	glTranslatef(-1, 0, 0);
	DrawEye(1);
	glTranslatef(2, 0, 0);
	DrawEye(1);
	glTranslatef(-1, 0, 0);
}
void CGLRenderer::DrawEye(float r) {
	glColor3f(128 / 255.0, 128 / 255.0, 128 / 255.0);
	DrawCircle(r, 30, -0.01);
	glRotatef(90, 1, 0, 0);
	DrawCylinder(r, r, 0.7, 30, 1);
	glRotatef(-90, 1, 0, 0);


	glColor3f(1, 1, 1);
	DrawCircle(0.8 * r, 30, -0.02);

	double deltaX = 0, deltaY = 0;
	if(world_angle_x > 0) 
	{
		deltaX = min(0.5 * r, world_angle_x * 0.1);
	}
	else if(world_angle_x!=0)
	{deltaX = max(-0.5 * r, world_angle_x * 0.1); }

	if (world_angle_y > 0)
	{
		deltaY = min(0.5 * r,world_angle_y * 0.1);
	}
	else if (world_angle_y != 0)
	{
		deltaY = max(-0.5 * r, world_angle_y * 0.1);
	}

	glTranslatef(deltaX,deltaY, 0);


	glColor3f(127/255.0, 51/255.0, 0);
	DrawCircle(r *0.3, 30, -0.03);
	glColor3f(0, 0, 0);
	DrawCircle(r *0.1, 30, -0.04);
	glTranslatef(-0.15 * r, 0.2 * r, 0);
	glColor3f(1, 1, 1);
	DrawCircle(r * 0.1, 30, -0.05);
	glTranslatef(0.15 * r, -0.2 * r, 0);
	glTranslatef(-deltaX, -deltaY, 0);
}
void CGLRenderer::DrawCircle(float r, UINT nSeg, float z) {
	glBegin(GL_TRIANGLE_FAN);
	float angleSeg = 2 * PI / nSeg;
	float angle = 0;
	glVertex3f(0, 0, z);
	nSeg++;
	for (UINT i = 0; i < nSeg; i++) {
		glVertex3f(r * cos(angle), r * sin(angle), z);
		angle += angleSeg;
	}
	glEnd();
}	
void CGLRenderer::DrawCylinder(float r1, float r2, float h, int nseg, float percentage) {
	float angleSeg = 2 * PI / nseg;
	float angle = 0;
	nseg++;
	UINT num = nseg * max(min(1, percentage), 0);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <num; i++) {
		glVertex3f(r1*cos(angle), -h/2, r1*sin(angle));
		glVertex3f(r2*cos(angle),h/2, r2*sin(angle));
		angle += angleSeg;
	}
	glEnd();
}
void CGLRenderer::DrawSpherePart(float r1, float r2, UINT nSeg, float percentage) {
	glBegin(GL_QUAD_STRIP);
	float step = 2 * PI / nSeg;
	float alfa = 0;
	float beta = 0;
	UINT len = min(percentage,1) * nSeg;
	glBegin(GL_QUAD_STRIP);
	for (UINT i = 0; i < len; i++) {
		for (UINT j = 0; j < nSeg;j++) {
			float x1=cos(alfa)*cos(beta);
			float y1=sin(alfa);
			float z1=cos(alfa)*sin(beta);
			
			float x2=cos(alfa+step)*cos(beta);
			float y2=sin(alfa+step);
			float z2=cos(alfa+step)*sin(beta);
			glVertex3f(r2*x1, r1*y1, r2*z1);
			glVertex3f(r2*x2, r1*y2, r2*z2);
			beta += step;
		}
		alfa += step;
	}
	glEnd();
}
void CGLRenderer::DrawHands(){}
void CGLRenderer::DrawLegs(){
	glTranslatef(-0.6,0, 0);
	DrawLeg(LEFT_LEG);
	//DrawShoe();
	glTranslatef(1.2, 0, 0);
	DrawLeg(RIGHT_LEG);
	glTranslatef(-1, 0, 0);
}
void CGLRenderer::DrawLeg(int side) {
	
	glPushMatrix();
	glTranslatef(0, -5.5, 0);
	glColor3f(63.0/255, 100.0/255, 127.0/255);
	DrawCylinder(0.3, 0.6, 0.7, 30,1);
	glColor3f(0, 0, 0);
	glTranslatef(0, -0.5, -0.5);
	DrawShoe(side);
	glPopMatrix();
	
	
}

void CGLRenderer::DrawShoe(int side) {
	if (side != LEFT_LEG && side != RIGHT_LEG) throw "nedozvoljena vrednost";
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glRotatef(30*side, 0, 0, 1);
	glTranslatef(-side*0.2, 0, 0);
	DrawCylinder(0.4, 0.4, 1, 30, 0.5);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, -0.4);
	DrawSpherePart(0.4, 0.4, 30, 0.5);
	glTranslatef(0, 0, 0.8);
	DrawSpherePart(0.4, 0.4, 30, 0.5);
	glPopMatrix();
}