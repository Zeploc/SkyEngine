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
	
	void InsertEntityMeshToRenderList(std::map<TPointer<CMaterialInterface>, TArray<ISceneVisual*>>& MeshesByMaterial, const TPointer<Entity>& EntityToRender);
	void RenderScenes();
	void Render(std::vector<TPointer<Entity>> Entities);
	void RenderMesh(ISceneVisual* SceneVisual, STransform Transform);
	void RenderImGui();

	TPointer<CSceneRenderer> AddSceneRenderer(TPointer<Scene> InTargetScene, SVector2i InSize);
	
	TPointer<CShader> ActiveShader;
	
protected:
	std::vector<TPointer<CSceneRenderer>> SceneRenderers;
	Matrix4 CurrentView;
	Matrix4 CurrentProjection;
};

// template <typename T>
// void CRenderer::PassAttributeToShader(int32_t ShaderLocation, T Attribute)
// {
// 	PassAttributeToShader(ShaderLocation, Attribute);
// }
