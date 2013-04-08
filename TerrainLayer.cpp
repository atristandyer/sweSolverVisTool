#include "TerrainLayer.h"

TerrainLayer::TerrainLayer()
{
	fort14Location = "";
	glLoaded = false;
	shadersSet = false;
	minZ = 99999.0;
	maxZ = -99999.0;
	outlineShader.SetSource("/home/tristan/Qt Projects/sweSolverVisTool/blackVertexShader.vert", "/home/tristan/Qt Projects/sweSolverVisTool/blackFragmentShader.frag");
	fillShader.SetSource("/home/tristan/Qt Projects/sweSolverVisTool/colorVertexShader.vert", "/home/tristan/Qt Projects/sweSolverVisTool/colorFragmentShader.frag");

	axesPoints[0] = 0.0;
	axesPoints[1] = 0.0;
	axesPoints[2] = 0.0;
	axesPoints[3] = 1.0;
	axesPoints[4] = 1000.0;
	axesPoints[5] = 0.0;
	axesPoints[6] = 0.0;
	axesPoints[7] = 1.0;
	axesPoints[8] = 0.0;
	axesPoints[9] = 1000.0;
	axesPoints[10] = 0.0;
	axesPoints[11] = 1.0;
	axesPoints[12] = 0.0;
	axesPoints[13] = 0.0;
	axesPoints[14] = 1000.0;
	axesPoints[15] = 1.0;

	axesIndices[0] = 0;
	axesIndices[1] = 1;
	axesIndices[2] = 0;
	axesIndices[3] = 2;
	axesIndices[4] = 0;
	axesIndices[5] = 3;
}


TerrainLayer::~TerrainLayer()
{
	if (glLoaded)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &VAOId);
		glDeleteBuffers(1, &VBOId);
		glDeleteBuffers(1, &IBOId);
		glDeleteBuffers(1, &IBOlines);
		glDeleteBuffers(1, &AxesVAO);
		glDeleteBuffers(1, &AxesVBO);
		glDeleteBuffers(1, &AxesIBO);

	}
}


void TerrainLayer::SetFort14(std::string fort14Loc)
{
	fort14Location = fort14Loc;
	ReadFort14();
}


void TerrainLayer::SetCamera(Camera *cam)
{
	outlineShader.SetCamera(cam);
	fillShader.SetCamera(cam);
	shadersSet = true;
}


std::vector<Node> TerrainLayer::GetNodeList()
{
	return nodeList;
}


int TerrainLayer::GetGridDimension()
{
	return gridDim;
}


void TerrainLayer::ReadFort14()
{
	if (!fort14Location.empty())
	{
		std::ifstream fort14 (fort14Location.data());
		if (fort14.is_open())
		{
			std::string line;
			std::getline(fort14, line);
			std::getline(fort14, line);
			std::stringstream(line) >> numElements >> numNodes >> gridDim;

			for (int i=0; i<numNodes; i++)
			{
				Node newNode;
				fort14 >> newNode.nodeNumber;
				fort14 >> newNode.x;
				fort14 >> newNode.y;
				fort14 >> newNode.z;
				if (newNode.z > minZ)
					minZ = newNode.z;
				else if (newNode.z < maxZ)
					maxZ = newNode.z;
				nodeList.push_back(newNode);
			}

			for (int i=0; i<numElements; i++)
			{
				int nextElem;
				fort14 >> nextElem;
				elementList.push_back(nextElem);
			}


			for (int i=0; i<gridDim; i++)
			{
				if (i%2 == 0)
				{
					for (int j=0; j<gridDim; j++)
					{
						linesList.push_back(gridDim*i + j);
					}
				} else {
					for (int j=gridDim-1; j>=0; j--)
					{
						linesList.push_back(gridDim*i + j);
					}
				}
			}

			if (gridDim%2 == 0)
			{
				for (int i=gridDim*(gridDim-1); i<gridDim*gridDim; i++)
				{
					if (i%2 == 0)
					{
						for (int j=0; j<gridDim; j++)
						{
							linesList.push_back(i-gridDim*j);
						}
					} else {
						for (int j=gridDim-1; j>=0; j--)
						{
							linesList.push_back(i-gridDim*j);
						}
					}

				}
			} else {
				for (int i=gridDim*gridDim-1; i>=gridDim*(gridDim-1); i--)
				{
					if (i%2 == 0)
					{
						for (int j=0; j<gridDim; j++)
						{
							linesList.push_back(i-gridDim*j);
						}
					} else {
						for (int j=gridDim-1; j>=0; j--)
						{
							linesList.push_back(i-gridDim*j);
						}
					}

				}
			}

			std::cout << "Read terrain" << std::endl;
			fort14.close();
			fillShader.SetHeightRange(minZ, maxZ);
			LoadDataToGPU();
		}
	}
}


void TerrainLayer::LoadDataToGPU()
{

	const size_t VertexBufferSize = 4*sizeof(GLfloat)*numNodes;
	const size_t IndexBufferSize = sizeof(GLuint)*numElements;
	const size_t IndexBufferSize2 = sizeof(GLuint)*linesList.size();

	glGenVertexArrays(1, &VAOId);
	glGenBuffers(1, &VBOId);
	glGenBuffers(1, &IBOId);
	glGenBuffers(1, &IBOlines);

	glBindVertexArray(VAOId);

	// Send Vertex Data
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_STATIC_DRAW);
	GLfloat * glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glNodeData != 0)
	{
		for (int i=0; i<numNodes; i++)
		{
			glNodeData[4*i+0] = (GLfloat)nodeList[i].x;
			glNodeData[4*i+1] = (GLfloat)nodeList[i].y;
			glNodeData[4*i+2] = (GLfloat)nodeList[i].z;
			glNodeData[4*i+3] = (GLfloat)1.0;
		}
	} else {
		return;
	}

	if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
	{
		return;
	}

	// Send Index Data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
	GLuint * glElementData = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glElementData != 0)
	{
		for (int i=0; i<numElements; i++)
		{
			glElementData[i] = elementList[i]-1;
		}
	} else {
		return;
	}

	if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
	{
		return;
	}

	glBindVertexArray(0);

	// Send Lines Index Data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOlines);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize2, NULL, GL_STATIC_DRAW);
	GLuint *glLinesData = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glLinesData != 0)
	{
		for (unsigned int i=0; i<linesList.size(); i++)
		{
			glLinesData[i] = linesList[i];
		}
	}

	if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
	{
		return;
	}


	// Create axes
	const size_t AxesVertexBufferSize = sizeof(axesPoints)*sizeof(GLfloat);
	const size_t AxesIndexBufferSize = sizeof(axesIndices)*sizeof(GLuint);

	glGenVertexArrays(1, &AxesVAO);
	glGenBuffers(1, &AxesVBO);
	glGenBuffers(1, &AxesIBO);

	glBindVertexArray(AxesVAO);

	// Send Axes Vertex Data
	glBindBuffer(GL_ARRAY_BUFFER, AxesVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
	glBufferData(GL_ARRAY_BUFFER, AxesVertexBufferSize, NULL, GL_STATIC_DRAW);
	GLfloat *axesNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (axesNodeData != 0)
	{
		for (unsigned int i=0; i<sizeof(axesPoints); i++)
		{
			axesNodeData[i] = axesPoints[i];
		}
	} else {
		return;
	}

	if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
	{
		return;
	}

	// Send Axes Index Data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AxesIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, AxesIndexBufferSize, NULL, GL_STATIC_DRAW);
	GLuint *axesElementData = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (axesElementData != 0)
	{
		for (unsigned int i=0; i<sizeof(axesIndices); i++)
		{
			axesElementData[i] = axesIndices[i];
		}
	} else {
		return;
	}

	if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
	{
		return;
	}

	glBindVertexArray(0);

	GLenum errorCheck = glGetError();
	if (errorCheck != GL_NO_ERROR)
	{
		std::cout << "Error Loading Data to GPU" << std::endl;
		return;
	} else {
		std::cout << "Terrain Data Loaded to GPU" << std::endl;
		glLoaded = true;
	}
}


void TerrainLayer::Draw()
{
	if (glLoaded && shadersSet)
	{
		glBindVertexArray(VAOId);

		// Draw Fill
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonOffset(6, 6);
		if (fillShader.Use() == 0)
			glDrawElements(GL_TRIANGLE_STRIP, numElements, GL_UNSIGNED_INT, (GLvoid*)0);

		// Draw Outline
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOlines);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonOffset(4, 4);
		if (outlineShader.Use() == 0)
		{
			glDrawElements(GL_LINE_STRIP, linesList.size(), GL_UNSIGNED_INT, (GLvoid*)0);

			// Draw Axes
			glBindVertexArray(AxesVAO);
			glPolygonOffset(2, 2);
			glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}
}
