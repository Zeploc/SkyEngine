// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Mesh.h"

class Plane : public Mesh
{
public:
	Plane(float fWidth, float fHeight, glm::vec4 Colour);

	Plane(float fWidth, float fHeight, glm::vec4 Colour, const char* TextureSource, glm::vec4 UVCoords = glm::vec4(0, 1, 0, 1));

	Plane(float _fWidth, float _fHeight, glm::vec4 _Colour, const char* TextureSource, glm::vec2 v2FrameCounts, int _iFPS);

	Plane(float _fWidth, float _fHeight, glm::vec4 _Colour, const char* TextureSource, int iCount, bool bHorizontal);

	Plane(glm::vec3 _Points[4], glm::vec4 _Colour);

	Plane(glm::vec3 _Points[4], glm::vec4 _Colour, const char* _TextureSource, glm::vec4 _UVCoords = glm::vec4(0, 1, 0, 1));

	Plane(glm::vec3 _Points[4], glm::vec4 _Colour, const char* _TextureSource, int iCount, bool bHorizontal);

	~Plane();

	void BindPlane();

	void Rebind() override;

	void Reset() override;

	void Render(FTransform Newtransform) override;

	void Update() override;

	Utils::AnimInfo AnimationInfo;
	double m_dFPSCounter = 0;
	double m_fFrameCheck = 0;
	Utils::Col2DI CollisionBox;

	glm::vec3 Points[4];

protected:
	struct PlaneInitialState
	{
		Utils::AnimInfo AnimationInfo;
		double m_dFPSCounter;
		double m_fFrameCheck;
		Utils::Col2DI CollisionBox;
	};

	PlaneInitialState m_PlaneInitialState;
};
