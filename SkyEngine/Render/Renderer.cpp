// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Renderer.h"

#include "Framebuffer.h"
#include "SceneRenderer.h"
#include "SceneVisual.h"
#include "Core/Application.h"
#include "Core/Asset/Asset.h"
#include "Entity/Component.h"
#include "Entity/Entity.h"
#include "Materials/Material.h"
#include "Render/Materials/InternalMaterial.h"
#include "Render/Shaders/Shader.h"
#include "Shaders/PBRShader.h"

CRenderer::CRenderer()
{
	TObjectPointer<CAsset> DefaultMaterialAsset = GetAssetManager()->FindAsset("Materials/DefaultMaterial.sasset");
	if (DefaultMaterialAsset)
	{
		DefaultMaterial = DefaultMaterialAsset->Load<CMaterial_PBR>();
	}
	else
	{
		// Made static to not go out of scope (will last the lifetime of the application)
		// TODO: Instead store this in a collection of materials/memory management
		static const THardPointer<CMaterial_PBR> DefaultPbrMaterial = std::make_shared<CMaterial_PBR>("DefaultMaterial");
		DefaultPbrMaterial->Params.DiffuseColour = {0.5f, 0.5f, 0.5f, 1.0f};
		DefaultPbrMaterial->bTwoSided = true;
		DefaultMaterialAsset = GetAssetManager()->AddAsset("Materials/DefaultMaterial.sasset", DefaultPbrMaterial->GetAssetClassName());
		DefaultMaterial = DefaultPbrMaterial;
		DefaultMaterialAsset->SetDefaultObject(DefaultMaterial);
	}
	GetMaterialManager()->AddMaterial(DefaultMaterial);
}

void CRenderer::InsertEntityMeshToRenderList(std::map<TAssetObjectPointer<CMaterialInterface>, TArray<ISceneVisual*>>& MeshesByMaterial, const THardPointer<Entity>& EntityToRender)
{
	std::map<TWeakPointer<CMaterialInterface>, TArray<ISceneVisual*>> Test;
	
	for (const THardPointer<CComponent>& Component : EntityToRender->GetComponents())
	{
		ISceneVisual* SceneVisual = GetInterface<ISceneVisual>(Component);
		if (SceneVisual && SceneVisual->GetMaterial())
		{
			TAssetObjectPointer<CMaterialInterface> Material = SceneVisual->GetMaterial();
			// TODO: Confirm default vector construction for map assignment is correct
			// if (!MeshesByMaterial.contains(Material))
			// {
			// 	TArray<ISceneVisual*> NewVector = {};
			// 	MeshesByMaterial.insert(std::pair(Material, NewVector));
			// }
			// Test[Material.GetWeak()].push_back(SceneVisual);
			MeshesByMaterial[Material].push_back(SceneVisual);
		}
	}	
}

void CRenderer::RenderScenes()
{
	for (THardPointer<CSceneRenderer> SceneRenderer : SceneRenderers)
	{
		CurrentView = SceneRenderer->GetView();
		CurrentProjection = SceneRenderer->GetProjection();
		SceneRenderer->Render();
	}
}

void CRenderer::Render(std::vector<THardPointer<Entity>> Entities)
{
	// TODO: Later store in/update list as new meshes added
	std::map<TAssetObjectPointer<CMaterialInterface>, TArray<ISceneVisual*>> MeshesByMaterial;
	
	for (const THardPointer<Entity>& EntityToRender : Entities)
	{
		if (EntityToRender && EntityToRender->CanRender())
		{
			InsertEntityMeshToRenderList(MeshesByMaterial, EntityToRender);
		}
		std::vector<THardPointer<Entity>> AdditionalEntitiesToRender = EntityToRender->GetAdditionalEntitiesToRender();
		for (const THardPointer<Entity>& AdditionalEntity : AdditionalEntitiesToRender)
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
		const TAssetObjectPointer<CMaterialInterface> Material = MaterialMeshSet.first;

		// Confirm/bind shader
		THardPointer<CShader> Shader = Material->GetBaseShader();
		if (ActiveShader != Shader)
		{
			ActiveShader = Shader;
			Shader->BindShader();
		}
		
		// Bind material
		Material->BindMaterial();

		// Draw meshes
		for (ISceneVisual* SceneVisual : MaterialMeshSet.second)
		{
			const STransform Transform = SceneVisual->GetRenderTransform();
			if (SceneVisual->ShouldRenderFaces())
			{
				RenderMesh(SceneVisual, Transform);
			}
			else
			{
				RenderLines(SceneVisual, Transform);
			}
		}
	}

	// if (!BoxVisuals.empty())
	// {
	//
	// 	if (!BoxShader)
	// 	{
	// 		BoxShader = ShaderManager::GetShader<CUnlitShader>();
	// 	}
	// 	
	// 	ActiveShader = BoxShader;
	// 	ActiveShader->BindShader();
	// 	
	// 	for (ISceneVisual* BoxVisual : BoxVisuals)
	// 	{
	// 		const STransform Transform = BoxVisual->GetRenderTransform();
	// 		RenderLines(BoxVisual, Transform);
	// 	}
	// }
	
	// for (const THardPointer<UIElement>& UIElement : UIElements)
	// {
	// 	RenderUIElement(UIElement);
	// }
	ActiveShader = nullptr;
}

void CRenderer::RenderMesh(ISceneVisual* SceneVisual, STransform Transform)
{
	GetGraphicsAPI()->ApplyMVP(ActiveShader->GetShaderProgram(), CurrentView, CurrentProjection, Transform);
	GetGraphicsAPI()->RenderMesh(SceneVisual);
}

void CRenderer::RenderLines(ISceneVisual* SceneVisual, STransform Transform)
{
	GetGraphicsAPI()->ApplyMVP(ActiveShader->GetShaderProgram(), CurrentView, CurrentProjection, Transform);
	GetGraphicsAPI()->RenderLines(SceneVisual, 2.0f);
}

void CRenderer::RenderImGui()
{
	GetGraphicsAPI()->RenderImGui();
}

// void CRenderer::AddBox(ISceneVisual* BoxVisual)
// {
// 	BoxVisuals.push_back(BoxVisual);
// }
//
// void CRenderer::RemoveBox(ISceneVisual* BoxVisual)
// {
// 	for (auto It = BoxVisuals.begin(); It != BoxVisuals.end(); ++It)
// 	{
// 		if (*It == BoxVisual)
// 		{
// 			BoxVisuals.erase(It);
// 			break;
// 		}
// 	}
// }

THardPointer<CSceneRenderer> CRenderer::AddSceneRenderer(TAssetObjectPointer<Scene> InTargetScene, SVector2i InSize)
{
	THardPointer<CSceneRenderer> SceneRenderer = CreatePointer<CSceneRenderer>();
	SceneRenderers.push_back(SceneRenderer);
	SceneRenderer->Init(InTargetScene, InSize);
	return SceneRenderer;
}
