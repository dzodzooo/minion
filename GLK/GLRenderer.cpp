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
	implants_size = 0;
	skin = new Material();
	white = new Material();
	black = new Material();
	eye_clr = new Material();
	gray = new Material();
	blue = new Material();
	texture = new Texture();
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
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	
	GLfloat light1_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { 0.0, 0.0, -5.0, 0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glEnable(GL_LIGHT1);

	skin->PrepareMaterial();
	skin->SetDiffuse(1, 216 / 255.0, 0);

	black->PrepareMaterial();
	black->SetDiffuse(0, 0, 0);

	white->PrepareMaterial();
	white->SetDiffuse(1, 1, 1);

	eye_clr->PrepareMaterial();
	eye_clr->SetDiffuse(127 / 255.0, 51 / 255.0, 0);

	gray->PrepareMaterial();
	gray->SetDiffuse(128 / 255.0, 128 / 255.0, 128 / 255.0);

	blue->PrepareMaterial();
	blue->SetDiffuse(63.0/255,100.0/255,127.0/255);

	texture->Prepare();
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
	DrawBoobs();
}
void CGLRenderer::DrawFace() {
	black->Select();
	DrawCylinder(2.51, 2.51, 2.0/3, 30,1);
	glTranslatef(0, 0, -2.5);
	DrawEyes();
	glTranslatef(0, 0, 2.5);
}
void CGLRenderer::DrawBody() {
	white->Select();
	glTranslatef(0, -1.5, 0);
	texture->Select();
	DrawCylinderTexture(2.5, 2.5, 4.5, 30,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glTranslatef(0, 2.25, 0);

	skin->Select();
	DrawSpherePart(2, 2.5, 30, 0.5);
	glTranslatef(0, -2.25, 0);

	glRotatef(180, 1, 0, 0);
	glTranslatef(0, 2.25, 0);
	blue->Select();
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
	
	glRotatef(90, 1, 0, 0);
	gray->Select(GL_FRONT_AND_BACK);
	DrawCylinder(r, r, 0.7, 30, 1);
	glRotatef(-90, 1, 0, 0);

	glTranslatef(0,0,-0.34);
	gray->Select(GL_FRONT_AND_BACK);
	DrawCircle(r, 30, -0.01);
	white->Select();	
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


	eye_clr->Select();
	DrawCircle(r *0.3, 30, -0.03);
	black->Select();
	DrawCircle(r *0.1, 30, -0.04);
	glTranslatef(-0.15 * r, 0.2 * r, 0);
	white->SetShininess(1);
	white->Select();

	DrawCircle(r * 0.1, 30, -0.05);
	glTranslatef(0.15 * r, -0.2 * r, 0);
	glTranslatef(-deltaX, -deltaY, 0);
	glTranslatef(0, 0, 0.34);
}
void CGLRenderer::DrawCircle(float r, UINT nSeg, float z) {
	glBegin(GL_TRIANGLE_FAN);
	float angleSeg = 2 * PI / nSeg;
	float angle = 0;
	glNormal3f(0, 0, z > 0 ? 1 : -1);
	glVertex3f(0, 0, z);
	nSeg++;
	for (UINT i = 0; i < nSeg; i++) {
		glNormal3f(0,0,z>0?1:-1);
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
		glNormal3f(r1 * cos(angle), 0, r1 * sin(angle));
		glVertex3f(r1*cos(angle), -h/2, r1*sin(angle));
		glNormal3f(r2 * cos(angle), 0, r2 * sin(angle));
		glVertex3f(r2*cos(angle),h/2, r2*sin(angle));
		angle += angleSeg;
	}
	glEnd();
}
void CGLRenderer::DrawCylinderTexture(float r1, float r2, float h, int nseg, float percentage) {
	float angleSeg = 2 * PI / nseg;
	float angle = 0;
	nseg++;
	UINT num = nseg * max(min(1, percentage), 0);
	glBegin(GL_QUAD_STRIP);
	float br = 0;
	float delta = 1.0 / nseg;
	for (int i = 0; i < num; i++) {
		glTexCoord2f(br,1);
		glNormal3f(r1 * cos(angle), 0, r1 * sin(angle));
		glVertex3f(r1 * cos(angle), -h / 2, r1 * sin(angle));
		glTexCoord2f(br, 0);
		glNormal3f(r2 * cos(angle), 0, r2 * sin(angle));
		glVertex3f(r2 * cos(angle), h / 2, r2 * sin(angle));
		angle += angleSeg;
		br += delta;
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
			glNormal3f(r2 * x1, r1 * y1, r2 * z1);
			glVertex3f(r2*x1, r1*y1, r2*z1);
			glNormal3f(r2 * x2, r1 * y2, r2 * z2);
			glVertex3f(r2*x2, r1*y2, r2*z2);
			beta += step;
		}
		alfa += step;
	}
	glEnd();
}
void CGLRenderer::DrawHands(){
	glPushMatrix();
	glTranslatef(2.5, -2, 0);
	DrawHand(LEFT_SIDE);
	glTranslatef(-5, 0, 0);
	DrawHand(RIGHT_SIDE);
	glPopMatrix();
}
void CGLRenderer::DrawHand(SIDES side) {
	glPushMatrix();
	skin->Select();
	float h = 2.4;
	DrawSpherePart(0.4,0.4,30,0.6);
	glTranslatef(side*0.33, -h/2, 0);
	glRotatef(side * 15, 0, 0, 1);
	DrawCylinder(0.3, 0.4, h, 30, 1);
	
	glTranslatef(0,-3/4.0*h/2,0);
	black->Select();
	DrawCylinder(0.3, 0.5, h / 4, 30, 1);

	glTranslatef(side * 0.1, 0.3*-h, 0);
	DrawSpherePart(0.5, 0.4, 30, 1);

	
	glTranslatef(-side*0.4, 0.2, 0);
	glRotatef(-15*side, 0, 0, 1);
	DrawSpherePart(0.4, 0.2, 30, 1);
	glPopMatrix();
}
void CGLRenderer::DrawLegs(){
	glPushMatrix();
	glTranslatef(-0.6,0, 0);
	DrawLeg(LEFT_SIDE);
	glTranslatef(1.2, 0, 0);
	DrawLeg(RIGHT_SIDE);
	glPopMatrix();
}
void CGLRenderer::DrawLeg(SIDES side) {
	
	glPushMatrix();
	glTranslatef(0, -5.5, 0);
	blue->Select();
	DrawCylinder(0.3, 0.6,0.8, 30,1);
	
	glTranslatef(0, -0.8, -0.5);
	DrawShoe(side);
	glPopMatrix();
}
void CGLRenderer::DrawShoe(SIDES side) {
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glRotatef(15*side, 0, 0, 1);
	glTranslatef(-side*0.2, -0.2, 0);

	black->Select();
	DrawCylinder(0.4, 0.4, 1, 30, 0.55);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, -0.4);

	black->Select();
	DrawSpherePart(0.4, 0.4, 30, 0.55);
	glTranslatef(0, 0, 0.8);

	black->Select();
	DrawSpherePart(0.4, 0.4, 30, 0.55);
	glPopMatrix();
}

void CGLRenderer::DrawBoobs() {
	float r = implants_size;
	blue->Select();
	glPushMatrix();
	glTranslatef(-0.75, -2.5, -2.5);
	DrawSpherePart(r, r, 30, 1);
	glTranslatef(1.5, 0, 0);
	DrawSpherePart(r, r, 30, 1);
	glPopMatrix();
}