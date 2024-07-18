#include "stdafx.h"
#include "Material.h"
#include <gl/GL.h>


Material::Material() {
	_ambient = new float[4];
	_diffuse = new float[4];
	_specular = new float[4];
	_emission = new float[4];


}
Material::~Material() {
	delete[] _ambient;
	delete[] _diffuse;
	delete[] _specular;
	delete[] _emission;
}
void Material::PrepareMaterial() {
	SetAmbient();
	SetDiffuse();
	SetSpecular();
	SetEmission(); 
	SetShininess();
}

void Material::Select(GLenum par) {
	glMaterialfv(par, GL_AMBIENT, _ambient);
	glMaterialfv(par, GL_DIFFUSE, _diffuse);
	glMaterialfv(par, GL_SPECULAR, _specular);
	glMaterialfv(par, GL_SHININESS, &_shininess);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, _emission);
}
void Material::SetAmbient(float r, float g, float b, float a) {
	_ambient[0] = r;
	_ambient[1] = g;
	_ambient[2] = b;
	_ambient[3] = a;
}
void Material::SetDiffuse(float r, float g, float b, float a){
	_diffuse[0] = r;
	_diffuse[1] = g;
	_diffuse[2] = b;
	_diffuse[3] = a;
}
void Material::SetSpecular(float r, float g, float b, float a){
	_specular[0] = r;
	_specular[1] = g;
	_specular[2] = b;
	_specular[3] = a;
}
void Material::SetEmission(float r, float g, float b, float a){
	_emission[0] = r;
	_emission[1] = g;
	_emission[2] = b;
	_emission[3] = a;
}
void Material::SetShininess(float val) {
	_shininess = val;
}
