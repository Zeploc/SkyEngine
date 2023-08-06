// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "GraphicsInstance.h"

#include "Entity/Entity.h"
#include "Render/Materials/InternalMaterial.h"
#include "Render/Shaders/Shader.h"
#include "UI/UIElement.h"

void IGraphicsInstance::InsertEntityMeshToRenderList(std::map<TPointer<CMaterialInterface>, std::vector<TPointer<CMeshComponent>>>& MeshesByMaterial, const TPointer<Entity>& EntityToRender)
{
	TPointer<CMaterialInterface> Material = EntityToRender->EntityMesh->MeshMaterial;
	if (!MeshesByMaterial.contains(Material))
	{
		std::vector<TPointer<CMeshComponent>> NewVector = {};
		MeshesByMaterial.insert(std::pair(Material, NewVector));
	}
	MeshesByMaterial[Material].push_back(EntityToRender->EntityMesh);
}

void IGraphicsInstance::Render(TPointer<IGraphicsWindow> GraphicsWindow, std::vector<TPointer<Entity>> Entities, std::vector<TPointer<UIElement>> UIElements)
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
			Shader->BindShader(shared_from_this());
		}
		
		// Bind material
		Material->BindMaterial(shared_from_this());

		// Draw meshes
		for (TPointer<CMeshComponent> MeshComponent : MaterialMeshSet.second)
		{
			const STransform AnchoredTransform = MeshComponent->Owner->GetAnchoredTransform();
			RenderMesh(MeshComponent, AnchoredTransform);
		}
	}
	
	for (const TPointer<UIElement>& UIElement : UIElements)
	{
		RenderUIElement(UIElement);
	}
	ActiveShader = nullptr;
}
