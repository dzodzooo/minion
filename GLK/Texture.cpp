#include "stdafx.h"
#include "Texture.h"
#include "GL\gl.h"
#include "GL/GLU.H"

Texture::Texture() {
	img = new DImage();
}

Texture::~Texture(){
	glDeleteTextures(1, &name);
}

void Texture::LoadTexture(CString filename) {
	img->Load(filename);
	glBindTexture(GL_TEXTURE_2D, name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, img->Width(), img->Height(), GL_RGBA, GL_UNSIGNED_BYTE, img->GetDIBBits());
}

void Texture::Prepare() {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glGenTextures(1, &name);
	LoadTexture(CString("res/minion_tex.png"));
}

void Texture::Select() {
	glBindTexture(GL_TEXTURE_2D, name);
}
