#pragma once

// This Includes //
#include "Engine\Entity.h"

// Library Includes //
#include <vector>
#include <memory>

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

class Terrain :	public Entity
{
public:

	Terrain(Utils::Transform _Transform, Utils::EANCHOR _Anchor, InitInfo& _InitInfo);
	~Terrain();


	virtual void DrawEntity() override;

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

