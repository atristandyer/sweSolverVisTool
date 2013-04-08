#ifndef TERRAINLAYER_H
#define TERRAINLAYER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "adcData.h"
#include "GLData.h"
#include "GLShader.h"
#include "GLCamera.h"

class TerrainLayer
{
	public:
		TerrainLayer();
		~TerrainLayer();

		void SetFort14(std::string fort14Loc);
		void SetCamera(Camera *cam);
		void Draw();

		std::vector<Node>	GetNodeList();
		int			GetGridDimension();

	protected:

		std::string	fort14Location;
		int		numNodes;
		int		numElements;
		int		gridDim;

		float		minZ, maxZ;

		float		axesPoints[16];
		int		axesIndices[6];

		GLuint		VAOId;
		GLuint		VBOId;
		GLuint		IBOId;
		GLuint		IBOlines;

		GLuint		AxesVAO;
		GLuint		AxesVBO;
		GLuint		AxesIBO;

		GLShader	outlineShader;
		GLShader	fillShader;

		std::vector<Node>	nodeList;
		std::vector<int>	elementList;
		std::vector<int>	linesList;

		bool	glLoaded;
		bool	shadersSet;

		void ReadFort14();
		void LoadDataToGPU();
};

#endif // TERRAINLAYER_H
