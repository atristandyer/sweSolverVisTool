#ifndef WATERLAYER_H
#define WATERLAYER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "adcData.h"
#include "GLCamera.h"
#include "GLShader.h"
#include "TerrainLayer.h"

class WaterLayer
{
	public:
		WaterLayer();
		~WaterLayer();

		void SetTerrainLayer(TerrainLayer *terrain);
		void SetFort63(std::string fort63Loc);
		void SetCamera(Camera *cam);
		void LoadTimestep(int ts);
		int GetCurrentTimestep();
		void Draw();

	protected:

		std::string	fort63Location;
		int		numCells;
		int		numTS;
		int		gridDim;
		int		currTS;

		float	minZ, maxZ;

		GLuint	VAOId;
		GLuint	VBOId;
		GLuint	IBOId;
		GLuint	IBOlines;

		GLShader	outlineShader;
		GLShader	fillShader;

		std::vector<Node>	nodeList;
		std::vector<Node>	fullNodeList;
		std::vector<int>	fillList;
		std::vector<int>	lineList;
		std::vector<float>	elevations;

		bool	glLoaded;
		bool	shadersSet;

		void	ReadFort63();
		void	LoadDataToGPU();

};

#endif // WATERLAYER_H
