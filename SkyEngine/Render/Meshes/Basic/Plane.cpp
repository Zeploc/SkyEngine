// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Plane.h"

// OpenGL Library //
#include <soil/SOIL2.h>

// Engine Includes //
#include "Render/Shader.h"
#include "Core/Application.h"
#include "System/Time.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Plane::Plane(float fWidth, float fHeight, glm::vec4 _Colour)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	CollisionBox.fHeight = m_fHeight;
	CollisionBox.fWidth = m_fWidth;
	MeshMaterial = std::make_shared<Material>("BaseProgram");
	MeshMaterial->Colour = _Colour;
	BindMeshData();	
}

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Plane::Plane(float fWidth, float fHeight, glm::vec4 _Colour, const char* _TextureSource)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	CollisionBox.fHeight = m_fHeight;
	CollisionBox.fWidth = m_fWidth;
	MeshMaterial = std::make_shared<Material>("BaseProgram");
	MeshMaterial->SetTexture(_TextureSource);
	MeshMaterial->Colour = _Colour;
	BindMeshData();	
}

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Plane::Plane(float _fWidth, float _fHeight, glm::vec4 _Colour, const char* _TextureSource, glm::vec2 v2FrameCounts, int _iFPS)
{
	m_fWidth = _fWidth;
	m_fHeight = _fHeight;
	CollisionBox.fHeight = m_fHeight;
	CollisionBox.fWidth = m_fWidth;
	MeshMaterial = std::make_shared<Material>("BaseProgram");
	MeshMaterial->SetTexture(_TextureSource);
	MeshMaterial->Colour = _Colour;
	BindMeshData();
	
	AnimationInfo.iFPS = _iFPS;
	m_fFrameCheck = 1.0f / AnimationInfo.iFPS;

	const TextureData TextureData = MeshMaterial->GetTextureData();

	AnimationInfo.v2FrameCount = v2FrameCounts;
	AnimationInfo.v2EndFrame = v2FrameCounts;
	AnimationInfo.v2FrameSize = {
		(TextureData.Width / v2FrameCounts.x) / TextureData.Width,
		(TextureData.Height / v2FrameCounts.y) / TextureData.Height
	};

	// TODO: Link coordinates in material for sprite
	UVCoords = glm::vec4(0, AnimationInfo.v2FrameSize.x, 0, AnimationInfo.v2FrameSize.y);
}

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Plane::Plane(float _fWidth, float _fHeight, glm::vec4 _Colour, const char* _TextureSource, int iCount, bool bHorizontal)
{	
	m_fWidth = _fWidth;
	m_fHeight = _fHeight;
	CollisionBox.fHeight = m_fHeight;
	CollisionBox.fWidth = m_fWidth;
	MeshMaterial = std::make_shared<Material>("BaseProgram");
	MeshMaterial->SetTexture(_TextureSource);
	MeshMaterial->Colour = _Colour;
	BindMeshData();	

	const TextureData TextureData = MeshMaterial->GetTextureData();
	float fImageRatio = static_cast<float>(TextureData.Width) / static_cast<float>(TextureData.Height);
	float fObjectRatio = _fHeight / _fWidth;
	float hSize = static_cast<float>(iCount);
	float vSize = static_cast<float>(iCount);
	if (bHorizontal)
	{
		vSize = iCount * fObjectRatio * fImageRatio;
	}
	else
	{
		hSize = iCount / fObjectRatio / fImageRatio;
	}

	// TODO: Link coordinates in material for sprite
	UVCoords = glm::vec4(0, hSize, 0, vSize);
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Plane::~Plane()
{
}

void Plane::Reset()
{
	Mesh::Reset();
	// Reset plane specific states
	AnimationInfo = m_PlaneInitialState.AnimationInfo;
	m_dFPSCounter = m_PlaneInitialState.m_dFPSCounter;
	m_fFrameCheck = m_PlaneInitialState.m_fFrameCheck;
	CollisionBox = m_PlaneInitialState.CollisionBox;
}

/************************************************************
#--Description--#:	Render Current Mesh to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Plane::Render(FTransform Newtransform)
{
	// TODO: move too renderer linking to plane
	if (m_fFrameCheck > 0)
	{
		m_dFPSCounter += Time::dTimeDelta;
		if (m_dFPSCounter > m_fFrameCheck)
		{
			m_dFPSCounter = 0; // m_dFPSCounter - m_fFrameCheck;
			AnimationInfo.Advance();
		}
	}
	// TODO: Properly link sprite sheet
	if (MeshMaterial && MeshMaterial->HasTexture())
	{
		glUseProgram(MeshMaterial->GetShaderProgram());
		GLint UVCoordsLoc = glGetUniformLocation(MeshMaterial->GetShaderProgram(), "uTexCoordOffset");
		glUniform2f(UVCoordsLoc, AnimationInfo.v2CurrentFrame.x * AnimationInfo.v2FrameSize.x, AnimationInfo.v2CurrentFrame.y * AnimationInfo.v2FrameSize.y);
	}
	// Mesh::Render(Newtransform);
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Plane::Update()
{
	if (m_fFrameCheck > 0) // Doesn't run, framecheck is 0?
	{
		m_dFPSCounter += Time::dTimeDelta;
		if (m_dFPSCounter > m_fFrameCheck)
		{
			m_dFPSCounter = 0; // m_dFPSCounter - m_fFrameCheck;
			AnimationInfo.Advance();
		}
	}
}

bool Plane::CheckHit(Vector3 RayStart, Vector3 RayDirection, Vector3& HitPos, Pointer<Entity> EntityCheck)
{
	glm::vec3 HalfDimensionvec = glm::vec3(m_fWidth / 2.0f, m_fHeight / 2.0f, m_fDepth / 2.0f);
	return Utils::CheckFaceHit(glm::vec3(-HalfDimensionvec.x, -HalfDimensionvec.y, HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, HalfDimensionvec.y, HalfDimensionvec.z), RayStart, RayDirection, EntityCheck, HitPos);
}

MeshData Plane::GetMeshData()
{
	const float HalfWidth = m_fWidth / 2;
	const float HalfHeight = m_fHeight / 2;
	const float HalfDepth = m_fDepth / 2;
	
	const std::vector<float> VertexPositions = {
		// Front Face
		-HalfWidth, HalfHeight, HalfDepth, 
		HalfWidth, HalfHeight, HalfDepth, 
		HalfWidth, -HalfHeight, HalfDepth,
		-HalfWidth, -HalfHeight, HalfDepth,
	};
	const std::vector<float> UVCoords = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	const std::vector<float> Normals = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,		
	};
	
	const std::vector<uint32_t> Indices = {
		0, 1, 2, // First Triangle
		0, 2, 3 // Second Triangle
	};

	MeshData PlaneMeshData(VertexPositions, Indices, Normals);
	if (MeshMaterial->HasTexture())
	{
		PlaneMeshData.SetUVs(UVCoords);
	}
	return PlaneMeshData;
}
