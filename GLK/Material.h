#pragma once
#include "GL/GL.H"
class Material
{
	float* _ambient, * _diffuse, * _specular, * _emission, _shininess;
public:
	Material();
	~Material();
	void PrepareMaterial();
	void Select(GLenum par=GL_FRONT);
	void SetAmbient(float r = 0.2, float g = 0.2, float b = 0.2, float a = 1.0);
	void SetDiffuse(float r = 0.8, float g = 0.8, float b = 0.8, float a = 1.0);
	void SetSpecular(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1);
	void SetEmission(float r = 0, float g = 0, float b = 0, float a = 1);
	void SetShininess(float val=0); 
};