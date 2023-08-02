// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/Mesh.h"
#include "System/Utils.h"

class ENGINE_API CPlane : public CMeshComponent
{
public:
	CPlane(const TPointer<Entity>& InOwner, float fWidth, float fHeight, TPointer<CMaterial> InMaterial = nullptr);

	CPlane(const TPointer<Entity>& InOwner, float InWidth, float InHeight, TPointer<CMaterial> InMaterial, glm::vec2 v2FrameCounts, int _iFPS);

	CPlane(const TPointer<Entity>& InOwner, float _fWidth, float _fHeight, TPointer<CMaterial> InMaterial, int iCount, bool bHorizontal);

	~CPlane();

	void Reset() override;

	// TODO: Update to graphics instance
	void Render(STransform Newtransform);

	void Update() override;
	
	bool CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos, TPointer<Entity> EntityCheck) override;

	Utils::AnimInfo AnimationInfo;
	double m_dFPSCounter = 0;
	double m_fFrameCheck = 0;
	Utils::Col2DI CollisionBox;

	glm::vec3 Points[4];
	glm::vec4 UVCoords;

protected:
	MeshData GetMeshData() override;
	struct PlaneInitialState
	{
		Utils::AnimInfo AnimationInfo;
		double m_dFPSCounter;
		double m_fFrameCheck;
		Utils::Col2DI CollisionBox;
	};

	PlaneInitialState m_PlaneInitialState;
};
