// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Plane.h"

// OpenGL Library //
#include <soil/SOIL2.h>

// Engine Includes //
#include "../MeshManager.h"
#include "Render/Shaders/ShaderManager.h"
#include "Core/Application.h"
#include "Render/Materials/Material.h"
#include "Render/Shaders/PBRShader.h"
#include "System/TimeManager.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
CPlane::CPlane(const TPointer<Entity>& InOwner, TPointer<CMaterialInterface> InMaterial)
: CMeshComponent(InOwner, MESH_PLANE, InMaterial)
{
	CollisionBox.fHeight = LEGACY_Height;
	CollisionBox.fWidth = LEGACY_Width;
	BindMeshData();	
}

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
CPlane::CPlane(const TPointer<Entity>& InOwner, TPointer<CMaterialInterface> InMaterial, glm::vec2 v2FrameCounts, int _iFPS)
: CMeshComponent(InOwner, MESH_PLANE, InMaterial)
{
	CollisionBox.fHeight = LEGACY_Height;
	CollisionBox.fWidth = LEGACY_Width;
	BindMeshData();
	
	AnimationInfo.iFPS = _iFPS;
	m_fFrameCheck = 1.0f / AnimationInfo.iFPS;

	const TPointer<TMaterial<CPBRShader>> Material = std::static_pointer_cast<CMaterial_PBR>(MeshMaterial);
	const TPointer<CTexture> TextureData = Material->Params.DiffuseTexture;

	AnimationInfo.v2FrameCount = v2FrameCounts;
	AnimationInfo.v2EndFrame = v2FrameCounts;
	AnimationInfo.v2FrameSize = {
		(TextureData->Width / v2FrameCounts.x) / TextureData->Width,
		(TextureData->Height / v2FrameCounts.y) / TextureData->Height
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
CPlane::CPlane(const TPointer<Entity>& InOwner, TPointer<CMaterialInterface> InMaterial, int iCount, bool bHorizontal)
: CMeshComponent(InOwner, MESH_PLANE, InMaterial)
{	
	CollisionBox.fHeight = LEGACY_Height;
	CollisionBox.fWidth = LEGACY_Width;
	BindMeshData();	

	const TPointer<TMaterial<CPBRShader>> Material = std::static_pointer_cast<CMaterial_PBR>(MeshMaterial);
	const TPointer<CTexture> TextureData = Material->Params.DiffuseTexture;
	float fImageRatio = static_cast<float>(TextureData->Width) / static_cast<float>(TextureData->Height);
	float fObjectRatio = LEGACY_Height / LEGACY_Width;
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

CPlane::~CPlane()
{
}

void CPlane::Render(STransform Newtransform)
{
	// TODO: move too renderer linking to plane
	if (m_fFrameCheck > 0)
	{
		m_dFPSCounter += CTimeManager::GetDeltaTime();
		if (m_dFPSCounter > m_fFrameCheck)
		{
			m_dFPSCounter = 0; // m_dFPSCounter - m_fFrameCheck;
			AnimationInfo.Advance();
		}
	}
	// TODO: Properly link sprite sheet
	if (MeshMaterial && MeshMaterial->HasTexture())
	{
		glUseProgram(MeshMaterial->GetBaseShader()->GetShaderProgram());
		GLint UVCoordsLoc = glGetUniformLocation(MeshMaterial->GetBaseShader()->GetShaderProgram(), "uTexCoordOffset");
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
void CPlane::Update()
{
	if (m_fFrameCheck > 0) // Doesn't run, framecheck is 0?
	{
		m_dFPSCounter += CTimeManager::GetDeltaTime();
		if (m_dFPSCounter > m_fFrameCheck)
		{
			m_dFPSCounter = 0; // m_dFPSCounter - m_fFrameCheck;
			AnimationInfo.Advance();
		}
	}
}