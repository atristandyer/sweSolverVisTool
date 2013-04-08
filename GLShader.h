#ifndef GLSHADER_H
#define GLSHADER_H

#include "GLData.h"
#include "GLCamera.h"
#include <string>
#include <fstream>
#include <iostream>

class GLShader
{
	public:

		// Function Definitions
		GLShader();
		~GLShader();
		int	Use();
		void	SetCamera(Camera *camera);
		void	SetSource(std::string vFile, std::string fFile);
		void	SetHeightRange(float low, float high);

	protected:

		// Variable Definitions
		GLuint		programID;
		Camera*		cam;

		float	heightRange[2];

		// Flags
		bool	compiled;
		bool	camSet;

		// Protected Functions
		void	UpdateUniforms();
		GLuint	CompileShaderPart(const char *source, GLenum shaderType);

};

#endif // GLSHADER_H
