#include "GraphicsInstance.h"

#include "Entity/Entity.h"
#include "Render/Shaders/Shader.h"
#include "UI/UIElement.h"

void IGraphicsInstance::InsertEntityMeshToRenderList(std::map<TPointer<CMaterial>, std::vector<TPointer<CMeshComponent>>> MeshesByMaterial, const TPointer<Entity>& EntityToRender)
{
	TPointer<CMaterial> Material = EntityToRender->EntityMesh->MeshMaterial;
	if (!MeshesByMaterial.contains(Material))
	{
		MeshesByMaterial.insert(MeshesByMaterial, {});
	}
	MeshesByMaterial[Material].push_back(EntityToRender->EntityMesh);
}

void IGraphicsInstance::Render(TPointer<IGraphicsWindow> GraphicsWindow, std::vector<TPointer<Entity>> Entities, std::vector<TPointer<UIElement>> UIElements)
{
	// TODO: Later store in/update list as new meshes added
	std::map<TPointer<CMaterial>, std::vector<TPointer<CMeshComponent>>> MeshesByMaterial;
	
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
		const std::shared_ptr<CMaterial> Material = MaterialMeshSet.first;

		// Confirm/bind shader
		TPointer<CShader> Shader = Material->GetShader();
		if (ActiveShader != Shader)
		{
			Shader->BindShader(shared_from_this());
			ActiveShader = Shader;
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

void IGraphicsInstance::RenderEntity(TPointer<Entity> Entity)
{	
	if (!Entity)
	{
		return;
	}
	const STransform AnchoredTransform = Entity->GetAnchoredTransform();
	assert(Entity->EntityMesh);
	RenderMesh(Entity->EntityMesh, AnchoredTransform);	
}
