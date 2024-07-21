#pragma once
#include "DImage.h"
#include "GL\gl.h"

class Texture
{
	DImage* img;
	GLuint name;
	void LoadTexture(CString filename);
public:
	Texture();
	~Texture();
	
	void Prepare();
	void Select();
};