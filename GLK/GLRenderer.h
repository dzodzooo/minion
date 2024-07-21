#pragma once
#include "Material.h"
#include "Texture.h"
class CGLRenderer
{
	enum SIDES { LEFT_SIDE = 1, RIGHT_SIDE = -1 };
	float world_angle_x, world_angle_y;
	Material *skin, *white, *black, *eye_clr,*gray, *blue;
	Texture* texture;
	
public:
	static SIDES legs;
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	void WorldRotate(double deltaX, double deltaY) {
		world_angle_x += 0.5*deltaX;
		world_angle_y += 0.5*deltaY;
	}
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DrawMinion();
	void DrawBody();
	void DrawHands();
	void DrawLegs();
	void DrawHand(SIDES side);
	void DrawLeg(SIDES side);
	void DrawShoe(SIDES side);
	void DrawFace();
	void DrawEyes();
	void DrawEye(float r);
	void DrawCircle(float r, UINT nSeg, float z);
	void DrawCylinder(float r1, float r2, float h, int nseg, float percentage);
	void DrawCylinderTexture(float r1, float r2, float h, int nseg, float percentage);
	void DrawSpherePart(float r1, float r2, UINT nSeg, float percentage);
	void DrawBoobs();
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};

