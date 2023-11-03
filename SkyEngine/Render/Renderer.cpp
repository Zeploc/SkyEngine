// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Renderer.h"

#include "Framebuffer.h"
#include "SceneRenderer.h"
#include "Core/Application.h"
#include "Entity/Entity.h"
#include "Render/Materials/InternalMaterial.h"
#include "Render/Shaders/Shader.h"

CRenderer::CRenderer()
{
}

void CRenderer::InsertEntityMeshToRenderList(std::map<TPointer<CMaterialInterface>, std::vector<TPointer<CMeshComponent>>>& MeshesByMaterial, const TPointer<Entity>& EntityToRender)
{
	TPointer<CMaterialInterface> Material = EntityToRender->EntityMesh->MeshMaterial;
	if (!MeshesByMaterial.contains(Material))
	{
		std::vector<TPointer<CMeshComponent>> NewVector = {};
		MeshesByMaterial.insert(std::pair(Material, NewVector));
	}
	MeshesByMaterial[Material].push_back(EntityToRender->EntityMesh);
}


void CRenderer::RenderScenes()
{
	for (TPointer<CSceneRenderer> SceneRenderer : SceneRenderers)
	{
		CurrentView = SceneRenderer->GetView();
		CurrentProjection = SceneRenderer->GetProjection();
		SceneRenderer->Render();
	}
}

void CRenderer::Render(std::vector<TPointer<Entity>> Entities)
{
	// TODO: Later store in/update list as new meshes added
	std::map<TPointer<CMaterialInterface>, std::vector<TPointer<CMeshComponent>>> MeshesByMaterial;
	
	for (const TPointer<Entity>& EntityToRender : Entities)
	{
		if (EntityToRender && EntityToRender->CanRender())
		{
			InsertEntityMeshToRenderList(MeshesByMaterial, EntityToRender);
		}
		std::vector<TPointer<Entity>> AdditionalEntitiesToRender = EntityToRender->GetAdditionalEntitiesToRender();
		for (const TPointer<Entity>& AdditionalEntity : AdditionalEntitiesToRender)
		{
			if (AdditionalEntity && AdditionalEntity->CanRender())
			{
				InsertEntityMeshToRenderList(MeshesByMaterial, AdditionalEntity);
			}
		}
	}

	// TODO: Later sort by shader program
	for (const auto& MaterialMeshSet : MeshesByMaterial)
	{
		const TPointer<CMaterialInterface> Material = MaterialMeshSet.first;

		// Confirm/bind shader
		TPointer<CShader> Shader = Material->GetBaseShader();
		if (ActiveShader != Shader)
		{
			ActiveShader = Shader;
			Shader->BindShader();
		}
		
		// Bind material
		Material->BindMaterial();

		// Draw meshes
		for (TPointer<CMeshComponent> MeshComponent : MaterialMeshSet.second)
		{
			const STransform AnchoredTransform = MeshComponent->Owner->GetAnchoredTransform();
			RenderMesh(MeshComponent, AnchoredTransform);
		}
	}
	
	// for (const TPointer<UIElement>& UIElement : UIElements)
	// {
	// 	RenderUIElement(UIElement);
	// }
	ActiveShader = nullptr;
}

void CRenderer::RenderMesh(TPointer<CMeshComponent> Mesh, STransform Transform)
{
	GetGraphicsAPI()->ApplyMVP(ActiveShader->GetShaderProgram(), CurrentView, CurrentProjection, Transform);
	GetGraphicsAPI()->RenderMesh(Mesh);
}

void CRenderer::RenderImGui()
{
	GetGraphicsAPI()->RenderImGui();
}

TPointer<CSceneRenderer> CRenderer::AddSceneRenderer(TPointer<Scene> InTargetScene, SVector2i InSize)
{
	TPointer<CSceneRenderer> SceneRenderer = CreatePointer<CSceneRenderer>();
	SceneRenderers.push_back(SceneRenderer);
	SceneRenderer->Init(InTargetScene, InSize);
	return SceneRenderer;
}
