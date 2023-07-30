#include "GraphicsInstance.h"

#include "Entity/Entity.h"
#include "UI/UIElement.h"

void IGraphicsInstance::Render(Pointer<IGraphicsWindow> GraphicsWindow, std::vector<Pointer<Entity>> Entities, std::vector<Pointer<UIElement>> UIElements)
{
	for (const Pointer<Entity>& EntityToRender : Entities)
	{
		if (EntityToRender && EntityToRender->CanRender())
		{
			RenderEntity(EntityToRender);
		}
		std::vector<Pointer<Entity>> AdditionalEntitiesToRender = EntityToRender->GetAdditionalEntitiesToRender();
		for (const Pointer<Entity>& AdditionalEntity : AdditionalEntitiesToRender)
		{
			RenderEntity(AdditionalEntity);
		}
	}
	for (const Pointer<UIElement>& UIElement : UIElements)
	{
		RenderUIElement(UIElement);
	}
}

void IGraphicsInstance::RenderEntity(Pointer<Entity> Entity)
{	
	if (!Entity)
	{
		return;
	}
	const FTransform AnchoredTransform = Entity->GetAnchoredTransform();
	assert(Entity->EntityMesh);
	RenderMesh(Entity->EntityMesh, AnchoredTransform);	
}
