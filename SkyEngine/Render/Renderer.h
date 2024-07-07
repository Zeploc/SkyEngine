// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Math/Transform.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/Vector4.h"
#include "Math/Vector2.h"
#include "Scene/Scene.h"

class ISceneVisual;
class IFramebuffer;
class CEngineWindow;
class UIElement;
class Entity;
class CMeshComponent;
class CShader;
class CMaterialInterface;
class CTexture;
class CSceneRenderer;

class CRenderer : public std::enable_shared_from_this<CRenderer>
{
public:
	CRenderer();
	virtual ~CRenderer() = default;	
	
	void InsertEntityMeshToRenderList(std::map<TAssetObjectPointer<CMaterialInterface>, TArray<ISceneVisual*>>& MeshesByMaterial, const THardPointer<Entity>& EntityToRender);
	void RenderScenes();
	void Render(std::vector<THardPointer<Entity>> Entities);
	void RenderMesh(ISceneVisual* SceneVisual, STransform Transform);
	void RenderLines(ISceneVisual* SceneVisual, STransform Transform);
	void RenderImGui();

	// void AddBox(ISceneVisual* BoxVisual);
	// void RemoveBox(ISceneVisual* BoxVisual);

	THardPointer<CSceneRenderer> AddSceneRenderer(TAssetObjectPointer<Scene> InTargetScene, SVector2i InSize);
	
	THardPointer<CShader> ActiveShader;

	TAssetObjectPointer<CMaterialInterface> DefaultMaterial;
	
protected:
	std::vector<THardPointer<CSceneRenderer>> SceneRenderers;
	Matrix4 CurrentView;
	Matrix4 CurrentProjection;

	// TArray<ISceneVisual*> BoxVisuals;
	// THardPointer<CShader> BoxShader;
};

// template <typename T>
// void CRenderer::PassAttributeToShader(int32_t ShaderLocation, T Attribute)
// {
// 	PassAttributeToShader(ShaderLocation, Attribute);
// }
