// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// OpenGL Library Includes //
#include <glew/glew.h>
#include <glm/glm.hpp>

// Engine Includes //
#include "Lighting.h"
#include "Math/FTransform.h"

// Library Include //
#include <memory>

#include "Texture.h"
#include "System/EnumTypes.h"

class Entity;
class CollisionBounds;

class ENGINE_API Mesh
{
public:
	Mesh();

	~Mesh();
	
	virtual bool CheckHit(Vector3 RayStart, Vector3 RayDirection, Vector3& HitPos, Pointer<Entity> EntityCheck);

	virtual void Update() {}

	virtual void OnDestroy() {}

	virtual void Reset();

	virtual void Rebind() {}

	// Will replace if texture exists
	virtual void SetLit(bool _bIsLit);

	bool IsLit() { return bIsLit; }

	void SetReflection(bool _bReflecting);

	bool IsReflecting() { return bReflection; }

	void AddCollisionBounds(float fHeight, float fWidth, float fDepth, Pointer<Entity> _EntityRef);

	void AddCollisionBounds(Pointer<CollisionBounds> NewCollision);

	Pointer<CollisionBounds> GetCollisionBounds()
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
	TextureData Texture;
	const char* TextureSource;
	glm::vec4 UVCoords;
	bool bHasTexture = false;
	int m_iIndicies;
	LightInfo LightProperties;
	bool bCullFace = true;
	// Stencil
	bool bStencil = false;
	glm::vec4 StencilColour = {0.5, 0.7, 0.8, 1.0f};
	//Pointer<Entity> EntityRef;
	bool bIsLit = true;
	bool bReflection = false;
	bool bFog = true;
protected:
	Pointer<CollisionBounds> MeshCollisionBounds;
};

