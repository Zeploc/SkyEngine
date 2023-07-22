// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Terrain.h"

// Engine Includes //
#include "Camera/CameraManager.h"
#include "Render/Plane.h"
#include "Render/Shader.h"

#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Terrain::Terrain(FTransform _Transform, EANCHOR _Anchor, InitInfo& _InitInfo) : Entity(_Transform, _Anchor)
{
	mInfo = _InitInfo;
	ReadTerrainMap();
	Smooth();
	LightProperties.fAmbientStrength = 0.4f;
	LightProperties.fLightSpecStrength = 0.7f;
	LightProperties.fShininess = 20.0f;
	LightProperties.v3LightColour = {5, 5, 5};

	for (int i = 0; i < 256; i++)
	{
		std::vector<glm::vec3> NewVec;
		Points.push_back(NewVec);
		for (int j = 0; j < 256; j++)
		{
			glm::vec3 randomPoint;
			randomPoint.x = i;
			randomPoint.z = j;
			randomPoint.y = mHeightmap[i][j]; // (float)(rand() % 100 - 50) / 100;
			Points[i].push_back(randomPoint);
		}
	}

	std::vector<GLfloat> verticiesVec;
	indiciesVec.clear();

	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			verticiesVec.push_back(Points[i][j].x);
			verticiesVec.push_back(Points[i][j].y);
			verticiesVec.push_back(Points[i][j].z);

			verticiesVec.push_back(0.1f);
			verticiesVec.push_back(0.9f);
			verticiesVec.push_back(0.4f);
			verticiesVec.push_back(1.0f);

			/*verticiesVec.push_back((float)j / 256.0f);
			verticiesVec.push_back((float)i / 256.0f);*/
			verticiesVec.push_back(static_cast<float>(1) / 256.0f);
			verticiesVec.push_back(static_cast<float>(1) / 256.0f);

			verticiesVec.push_back(0.0f);
			verticiesVec.push_back(1.0f);
			verticiesVec.push_back(0.0f);

			if (i < 255 && j < 255)
			{
				int p1, p2, p3, p4;
				p1 = j * 256 + i;
				p2 = j * 256 + i + 1;
				p3 = (j + 1) * 256 + i + 1;
				p4 = (j + 1) * 256 + i;
				indiciesVec.push_back(p1);
				indiciesVec.push_back(p2);
				indiciesVec.push_back(p4);
				indiciesVec.push_back(p2);
				indiciesVec.push_back(p3);
				indiciesVec.push_back(p4);
			}
		}
	}

	int CurrentVertexID = 0;

	/*for (int j = 1; j < 255; j++)
	{
		for (int i = 1; i < 255; i++)
		{*/

	for (int i = 1; i < 255; i++)
	{
		for (int j = 1; j < 255; j++)
		{
			float fTop = mHeightmap[i - 1][j]; // Top
			float fBottom = mHeightmap[i + 1][j]; // Bottom
			float fLeft = mHeightmap[i][j - 1]; // Left
			float fRight = mHeightmap[i][j + 1]; // Right

			glm::vec3 v3TanZ(0.0f, (fTop - fBottom) * 0.5f, 1.0f);
			glm::vec3 v3TanX(1.0f, (fRight - fLeft) * 0.5f, 0.0f);

			// 
			glm::vec3 v3Normal;
			v3Normal = cross(v3TanZ, v3TanX);
			v3Normal = normalize(v3Normal);

			CurrentVertexID += 12;
			// Store the normal of the current vertex
			verticiesVec[(i * 256 + j) * 12 + 9] = v3Normal.x;
			verticiesVec[(i * 256 + j) * 12 + 10] = v3Normal.y;
			verticiesVec[(i * 256 + j) * 12 + 11] = v3Normal.z;
		}
	}

	const char* TextureSource = "Resources/Images/TerrainMap2.png";
	// If no texture, texture source is equal to ""
	bHasTexture = true;

	vao = Shader::CreateBuffer(TextureSource, texture, true, true);
	//if (TextureSource != "")
	glBufferData(GL_ARRAY_BUFFER, verticiesVec.size() * sizeof(GLfloat), verticiesVec.data(), GL_STATIC_DRAW);
	//else
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiciesVec.size() * sizeof(GLuint), indiciesVec.data(), GL_STATIC_DRAW);

	program = Shader::Programs["BaseProgram"];
}

Terrain::~Terrain()
{
}

void Terrain::DrawEntity()
{
	bool bFog = false;
	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "bIsTex"), bHasTexture);
	glUniform1i(glGetUniformLocation(program, "bFog"), bFog);
	glUniform1i(glGetUniformLocation(program, "bIsLit"), true);
	glUniform1i(glGetUniformLocation(program, "ToonShade"), true);
	Lighting::PassLightingToShader(program, LightProperties, Transform);
	if (bHasTexture)
	{
		glEnable(GL_BLEND);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	else
	{
		glUseProgram(program);
		glDisable(GL_BLEND);
	}
	if (bFog)
	{
		glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, value_ptr(CameraManager::GetInstance()->GetCameraPosition().ToGLM()));
		glUniform4fv(glGetUniformLocation(program, "vFogColor"), 1, value_ptr(Lighting::m_v4FogColour));
		glUniform1f(glGetUniformLocation(program, "StartFog"), Lighting::StartFogDistance);
		glUniform1f(glGetUniformLocation(program, "EndFog"), Lighting::EndFogDistance);
	}
	glFrontFace(GL_CW);
	glDisable(GL_CULL_FACE);
	CameraManager::GetInstance()->SetMVP(Transform, program);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indiciesVec.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Terrain::ReadTerrainMap()
{
	std::vector<float> Heightmap;

	// A height for each vertex
	std::vector<unsigned char> in(mInfo.NumRows * mInfo.NumCols);

	// Open the file.
	std::ifstream inFile;
	inFile.open(mInfo.HeightmapFilename.c_str(), std::ios_base::binary);

	std::cout << "Begin reading in terrain file" << std::endl;

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], in.size());

		// Done with file.
		inFile.close();
	}

	std::cout << "Read in complete, generating heightmap" << std::endl;

	// Copy the array data into a float array, and scale and offset the heights.
	Heightmap.resize(mInfo.NumRows * mInfo.NumCols, 0);
	for (int i = 0; i < mInfo.NumRows * mInfo.NumCols; ++i)
	{
		Heightmap[i] = static_cast<float>(in[i]) / 255.0f; //  *mInfo.HeightScale + mInfo.HeightOffset;
	}

	std::cout << "Heightmap generated" << std::endl;

	int CurrentRow = 0;
	std::vector<float> FirstRow;
	mHeightmap.push_back(FirstRow);
	for (int i = 0; i < Heightmap.size(); i++)
	{
		if (i % (mInfo.NumCols) == 0 && i != 0)
		{
			CurrentRow++;
			std::vector<float> NewRow;
			mHeightmap.push_back(NewRow);
		}
		mHeightmap[CurrentRow].push_back(Heightmap[i] * 100);
		//i += 3;
		//std::cout << Heightmap[i] << " ";
	}

	//for (int i = 0; i < mHeightmap.size(); i++)
	//{
	//	for (int j = 0; j < mHeightmap[i].size(); j++)
	//	{
	//		//if (j > 100) break;
	//		std::cout << round(mHeightmap[i][j] * 10) / 10 << " ";
	//	}
	//	std::cout << std::endl << std::endl;
	//}
}

void Terrain::Smooth()
{
	std::vector<std::vector<float>> dest;

	for (int i = 0; i < mInfo.NumRows; ++i)
	{
		std::vector<float> NewVec(mInfo.NumRows);
		dest.push_back(NewVec);
		for (int j = 0; j < mInfo.NumCols; ++j)
		{
			dest[i][j] = average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	mHeightmap = dest;
}

float Terrain::average(int i, int j)
{
	float avg = 0.0f;
	float num = 0.0f;

	for (int m = i - 1; m <= i + 1; ++m)
	{
		for (int n = j - 1; n <= j + 1; ++n)
		{
			if (m >= 0 && m < mInfo.NumRows && n >= 0 && n < mInfo.NumCols)
			{
				avg += mHeightmap[m][n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

float Terrain::GetYPosition(glm::vec2 CurrentPos)
{
	if (CurrentPos.x < 0 || CurrentPos.x >= (mInfo.NumCols - 1) || CurrentPos.y < 0 || CurrentPos.y >= (mInfo.NumRows - 1))
	{
		return 0.0f;
	}

	// Transform from terrain local space to "cell" space.
	float c = (CurrentPos.y); // +0.5f*width()) / mInfo.CellSpacing;
	float d = (CurrentPos.x); // -0.5f*depth()) / -mInfo.CellSpacing;

	// Get the row and column we are in.
	int row = static_cast<int>(floorf(d));
	int col = static_cast<int>(floorf(c));

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = Points[row][col].y; // mHeightmap[row*mInfo.NumCols + col];
	float B = Points[row][col + 1].y; // mHeightmap[row*mInfo.NumCols + col + 1];
	float C = Points[row + 1][col].y; //mHeightmap[(row + 1)*mInfo.NumCols + col];
	float D = Points[row + 1][col + 1].y; //mHeightmap[(row + 1)*mInfo.NumCols + col + 1];

	// Where we are relative to the cell.
	float s = c - static_cast<float>(col);
	float t = d - static_cast<float>(row);

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s * uy + t * vy;
	}
	// lower triangle DCB.
	float uy = C - D;
	float vy = B - D;
	return D + (1.0f - s) * uy + (1.0f - t) * vy;

	return 0.0f;
}
