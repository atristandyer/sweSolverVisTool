#include "WaterLayer.h"

WaterLayer::WaterLayer()
{
	fort63Location = "";
	glLoaded = false;
	shadersSet = false;
	currTS = 0;
	minZ = 99999.0;
	maxZ = -99999.0;
	outlineShader.SetSource("/home/tristan/Qt Projects/sweSolverVisTool/blackVertexShader.vert", "/home/tristan/Qt Projects/sweSolverVisTool/blackFragmentShader.frag");
	fillShader.SetSource("/home/tristan/Qt Projects/sweSolverVisTool/colorVertexShader2.vert", "/home/tristan/Qt Projects/sweSolverVisTool/colorFragmentShader2.frag");
}


WaterLayer::~WaterLayer()
{
	if (glLoaded)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &VAOId);
		glDeleteBuffers(1, &VBOId);
		glDeleteBuffers(1, &IBOId);
		glDeleteBuffers(1, &IBOlines);
	}
}


void WaterLayer::SetTerrainLayer(TerrainLayer *terrain)
{
	nodeList = terrain->GetNodeList();
	gridDim = terrain->GetGridDimension();

	for (int i=0; i<gridDim-1; i++)
	{
		for (int j=0; j<gridDim-1; j++)
		{
			fullNodeList.push_back(nodeList[i*gridDim+j]);
			fullNodeList.push_back(nodeList[i*gridDim+j+1]);
			fullNodeList.push_back(nodeList[(i+1)*gridDim+j]);
			fullNodeList.push_back(nodeList[(i+1)*gridDim+j+1]);

			unsigned int startIndex = fullNodeList.size()-4;
			fillList.push_back(startIndex);
			fillList.push_back(startIndex+1);
			fillList.push_back(startIndex+2);
			fillList.push_back(startIndex+1);
			fillList.push_back(startIndex+2);
			fillList.push_back(startIndex+3);
			lineList.push_back(startIndex);
			lineList.push_back(startIndex+1);
			lineList.push_back(startIndex+1);
			lineList.push_back(startIndex+3);
			lineList.push_back(startIndex+3);
			lineList.push_back(startIndex+2);
			lineList.push_back(startIndex+2);
			lineList.push_back(startIndex);
		}
	}

	LoadDataToGPU();
}


void WaterLayer::SetFort63(std::string fort63Loc)
{
	fort63Location = fort63Loc;
	ReadFort63();
}


void WaterLayer::SetCamera(Camera *cam)
{
	outlineShader.SetCamera(cam);
	fillShader.SetCamera(cam);
	shadersSet = true;
}


void WaterLayer::LoadTimestep(int ts)
{
	if (!glLoaded)
	{
		LoadDataToGPU();
	}
	else if (ts >= 0 && ts < numTS)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		GLfloat *glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glNodeData)
		{
			int counter = 0;
			for (int i=ts*numCells; i<(ts+1)*numCells; i++)
			{
				for (int j=0; j<4; j++)
				{
					glNodeData[16*counter+4*j+2] = elevations[i];
				}
				counter++;
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
		} else {
			std::cout << "Unable to map buffer" << std::endl;
		}

		currTS = ts;
	}
}


int WaterLayer::GetCurrentTimestep()
{
	return currTS;
}


void WaterLayer::Draw()
{
	if (glLoaded && shadersSet)
	{
		glBindVertexArray(VAOId);

		// Draw Fill
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonOffset(6, 6);
		if (fillShader.Use() == 0)
			glDrawElements(GL_TRIANGLES, numCells*2*3, GL_UNSIGNED_INT, (GLvoid*)0);

		// Draw Outline
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOlines);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonOffset(4, 4);
		if (outlineShader.Use() == 0)
			glDrawElements(GL_LINES, numCells*4*2, GL_UNSIGNED_INT, (GLvoid*)0);

		glBindVertexArray(0);
		glUseProgram(0);
	}
}


void WaterLayer::ReadFort63()
{
	if (!fort63Location.empty())
	{
		std::ifstream fort63 (fort63Location.data());
		if (fort63.is_open())
		{
			std::string line;
			std::getline(fort63, line);
			std::getline(fort63, line);
			std::stringstream(line) >> numTS >> numCells;
			for (int i=0; i<numTS; i++)
			{
				std::getline(fort63, line);
				if (i>0)
					std::getline(fort63, line);
				int cellNum;
				float elevation;
				for (int j=0; j<numCells; j++)
				{
					fort63 >> cellNum >> elevation;
					elevations.push_back(elevation);
					if (elevation < minZ)
						minZ = elevation;
					else if (elevation > maxZ)
						maxZ = elevation;
				}
			}

			std::cout << "Read Water Elevation Time-Histories" << std::endl;
			fort63.close();
			fillShader.SetHeightRange(minZ, maxZ);
			std::cout << "minZ: " << minZ << "\tmaxZ: " << maxZ << std::endl;
			LoadTimestep(0);
		}
	}
}


void WaterLayer::LoadDataToGPU()
{
	const size_t VertexBufferSize = 4*sizeof(GLfloat)*fullNodeList.size();
	const size_t IndexBufferSize = sizeof(GLuint)*fillList.size();
	const size_t IndexBufferLineSize = sizeof(GLuint)*lineList.size();

	glGenVertexArrays(1, &VAOId);
	glGenBuffers(1, &VBOId);
	glGenBuffers(1, &IBOId);
	glGenBuffers(1, &IBOlines);

	glBindVertexArray(VAOId);

	// Send Vertex Data
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_DYNAMIC_DRAW);
	GLfloat *glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glNodeData != 0)
	{
		for (unsigned int i=0; i<fullNodeList.size(); i++)
		{
			glNodeData[4*i+0] = (GLfloat)fullNodeList[i].x;
			glNodeData[4*i+1] = (GLfloat)fullNodeList[i].y;
			glNodeData[4*i+2] = (GLfloat)fullNodeList[i].z;
			glNodeData[4*i+3] = 1.0;
		}
	} else {
		return;
	}

	if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
	{
		return;
	}

	// Send Fill Index Data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
	GLuint *glElementData = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glElementData != 0)
	{
		for (unsigned int i=0; i<fillList.size(); i++)
		{
			glElementData[i] = (GLuint)fillList[i];
		}
	} else {
		return;
	}

	if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
	{
		return;
	}

	glBindVertexArray(0);

	// Send Outline Index Data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOlines);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferLineSize, NULL, GL_STATIC_DRAW);
	GLuint *glLineData = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glLineData != 0)
	{
		for (unsigned int i=0; i<lineList.size(); i++)
		{
			glLineData[i] = (GLuint)lineList[i];
		}
	} else {
		return;
	}

	if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
	{
		return;
	}

	GLenum errorCheck = glGetError();
	if (errorCheck != GL_NO_ERROR)
	{
		std::cout << "Error Loading Data to GPU" << std::endl;
		return;
	} else {
		std::cout << "Water Data Loaded to GPU" << std::endl;
		glLoaded = true;
	}
}









































