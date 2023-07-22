// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //

// Library Includes //
#include <glew/glew.h>
#include <string>
#include <vector>

#include "Entity/Entity.h"

// Engine Includes //

struct InitInfo
{
	std::string HeightmapFilename;
	std::string LayerMapFilename0;
	std::string LayerMapFilename1;
	std::string LayerMapFilename2;
	std::string LayerMapFilename3;
	std::string LayerMapFilename4;
	std::string BlendMapFilename;
	float HeightScale;
	float HeightOffset;
	GLuint NumRows;
	GLuint NumCols;
	float CellSpacing;
};

class Terrain : public Entity
{
public:
	Terrain(FTransform _Transform, EANCHOR _Anchor, InitInfo& _InitInfo);

	~Terrain();

	void DrawEntity() override;

	void ReadTerrainMap();

	void Smooth();

	float average(int i, int j);

	GLuint program;
	GLuint vao;
	GLuint texture;
	bool bHasTexture = false;
	std::vector<GLuint> indiciesVec;

	InitInfo mInfo;
	std::vector<std::vector<float>> mHeightmap;

	float GetYPosition(glm::vec2 CurrentPos);

	LightInfo LightProperties;

private:
	std::vector<std::vector<glm::vec3>> Points;
};
