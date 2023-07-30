// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Pyramid.h"

// Engine Includes //
#include "Shader.h"
#include "Core/Application.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Pyramid::Pyramid(float fWidth, float fHeight, float fDepth, glm::vec4 _Colour)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
	Colour = _Colour;
	UVCoords = glm::vec4(0, 1, 0, 1);
	TextureSource = "";
	bHasTexture = false;
	m_iIndicies = 18;
	BindPyramid();
	m_eShape = EMESHTYPE::PYRAMID;
	program = Shader::Programs["BaseProgram"];
}

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Pyramid::Pyramid(float fWidth, float fHeight, float fDepth, glm::vec4 _Colour, const char* _TextureSource, glm::vec4 _UVCoords)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
	Colour = _Colour;
	TextureSource = _TextureSource;
	UVCoords = _UVCoords;
	bHasTexture = true;
	m_iIndicies = 18;
	BindPyramid();
	m_eShape = EMESHTYPE::PYRAMID;
	program = Shader::Programs["BaseProgram"];
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Pyramid::~Pyramid()
{
}

/************************************************************
#--Description--#: 	Binds pyramid with sepcified attributes
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Pyramid::BindPyramid()
{
	float fHalfWidth = m_fWidth / 2;
	float fHalfHeight = m_fHeight / 2;
	float fHalfDepth = m_fDepth / 2;

	std::vector<float> Texturedvertices = {
		// Positions								// Colors			
		-fHalfWidth, -fHalfHeight, -fHalfDepth, Colour.r, Colour.g, Colour.b, Colour.a, UVCoords.x, UVCoords.z,
		fHalfWidth, -fHalfHeight, -fHalfDepth, Colour.r, Colour.g, Colour.b, Colour.a, UVCoords.y, UVCoords.z,
		fHalfWidth, -fHalfHeight, fHalfDepth, Colour.r, Colour.g, Colour.b, Colour.a, UVCoords.y, UVCoords.w,
		-fHalfWidth, -fHalfHeight, fHalfDepth, Colour.r, Colour.g, Colour.b, Colour.a, UVCoords.x, UVCoords.w,

		0, fHalfHeight, 0, Colour.r, Colour.g, Colour.b, Colour.a, UVCoords.y / 2, UVCoords.w, // Top Point
	};
	
	std::vector<float> vertices = {
		// Positions								// Colors			
		-fHalfWidth, -fHalfHeight, -fHalfDepth, Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, -fHalfHeight, -fHalfDepth, Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, -fHalfHeight, fHalfDepth, Colour.r, Colour.g, Colour.b, Colour.a,
		-fHalfWidth, -fHalfHeight, fHalfDepth, Colour.r, Colour.g, Colour.b, Colour.a,

		0, fHalfHeight, 0, Colour.r, Colour.g, Colour.b, Colour.a
	};

	std::vector<uint32_t> indices = {
		0, 4, 3, // Side 1
		3, 4, 2, // Side 2
		2, 4, 1, // Side 3
		1, 4, 0, // Side 4

		3, 2, 1, // Bottom Triangle 1
		3, 1, 0 // Bottom Triangle 1
	};

	vao = GetGraphicsAPI()->CreateBuffer(TextureSource, Texture, true);
	// If no texture, texture source is equal to ""
	const std::vector<float> VerticesToUse = TextureSource != "" ? Texturedvertices : vertices;
	
	GetGraphicsAPI()->BindArray(VerticesToUse, indices, vao, false);
}

/************************************************************
#--Description--#: 	Rebinds the vao with the colour (not texture)
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Pyramid::Rebind()
{
	BindPyramid();
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Pyramid::Update()
{
}
