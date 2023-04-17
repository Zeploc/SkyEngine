// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// OpenGL Library Includes //
#include <glew.h>
#include <glm/glm.hpp>

// Engine Includes //
#include "Lighting.h"
#include "Engine/Math/FTransform.h"

// Library Include //
#include <memory>

class CollisionBounds;

class Mesh
{
public:
	Mesh();

	~Mesh();
	
	virtual void Render(FTransform Newtransform);

	virtual void Update()
	{
	}

	virtual void OnDestroy()
	{
	}

	virtual void Reset();

	virtual void Rebind()
	{
	}
	// Will replace if texture exists
	virtual void SetLit(bool _bIsLit);

	bool IsLit() { return bIsLit; }

	void SetReflection(bool _bReflecting);

	bool IsReflecting() { return bReflection; }

	void AddCollisionBounds(float fHeight, float fWidth, float fDepth, std::shared_ptr<Entity> _EntityRef);

	void AddCollisionBounds(std::shared_ptr<CollisionBounds> NewCollision);

	std::shared_ptr<CollisionBounds> GetCollisionBounds()
	{
		return MeshCollisionBounds;
	};

	bool bDepthTest = true;
	EMESHTYPE m_eShape;
	float m_fWidth;
	float m_fHeight;
	float m_fDepth = 0;
	glm::vec4 Colour;
	GLuint program;
	GLuint vao;
	GLuint texture;
	const char* TextureSource;
	glm::vec4 UVCoords;
	bool bHasTexture = false;
	int m_iIndicies;
	LightInfo LightProperties;
	bool bCullFace = true;
	// Stencil
	bool bStencil = false;
	glm::vec4 StencilColour = {0.5, 0.7, 0.8, 1.0f};
	//std::shared_ptr<Entity> EntityRef;
protected:
	std::shared_ptr<CollisionBounds> MeshCollisionBounds;
	bool bIsLit = true;
	bool bReflection = false;
	bool bFog = true;
};

