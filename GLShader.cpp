#include "GLShader.h"

GLShader::GLShader()
{
	compiled = false;
	camSet = false;
	programID = 0;
	cam = 0;
	heightRange[0] = 0.0;
	heightRange[1] = 1.0;
}


GLShader::~GLShader()
{

}


int GLShader::Use()
{
	UpdateUniforms();
	if (compiled)
	{
		glUseProgram(programID);
		return 0;
	}
	return 1;
}


void GLShader::SetCamera(Camera *camera)
{
	cam = camera;
	camSet = true;
}


void GLShader::SetSource(std::string vFile, std::string fFile)
{
	std::ifstream vertexFile(vFile.data());
	std::ifstream fragFile(fFile.data());

	if (vertexFile.is_open() && fragFile.is_open())
	{
		std::string vertSource((std::istreambuf_iterator<char>(vertexFile)), (std::istreambuf_iterator<char>()));
		std::string fragSource((std::istreambuf_iterator<char>(fragFile)), (std::istreambuf_iterator<char>()));

		vertexFile.close();
		fragFile.close();

		const char *fullVertSource = vertSource.data();
		const char *fullFragSource = fragSource.data();

		GLuint vertexShaderID = CompileShaderPart(fullVertSource, GL_VERTEX_SHADER);
		GLuint fragmentShaderID = CompileShaderPart(fullFragSource, GL_FRAGMENT_SHADER);

		if (vertexShaderID && fragmentShaderID)
		{
			programID = glCreateProgram();
			glAttachShader(programID, vertexShaderID);
			glAttachShader(programID, fragmentShaderID);
			glLinkProgram(programID);
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);

			compiled = true;
		}
	}
}


void GLShader::SetHeightRange(float low, float high)
{
	heightRange[0] = low;
	heightRange[1] = high;
}


void GLShader::UpdateUniforms()
{
	if (compiled && camSet)
	{
		glUseProgram(programID);

		GLint MVPUniform = glGetUniformLocation(programID, "MVPMatrix");
		GLint HeightsUniform = glGetUniformLocation(programID, "HeightRange");

		glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, cam->MVPMatrix.m);
		glUniform2fv(HeightsUniform, 1, heightRange);
	}
}


GLuint GLShader::CompileShaderPart(const char *source, GLenum shaderType)
{
	GLuint shaderID = glCreateShader(shaderType);

	if (shaderID != 0)
	{
		glShaderSource(shaderID, 1, &source, NULL);
		glCompileShader(shaderID);

		GLint compileResult;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);
		if (compileResult != GL_TRUE)
		{
			std::cout << "Compile Error" << std::endl;
			return 0;
		} else {
			return shaderID;
		}
	} else {
		std::cout << "Error creating shader" << std::endl;
		return 0;
	}
}
