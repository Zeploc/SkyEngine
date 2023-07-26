#include "GraphicsWindow.h"

#include "GraphicsInstance.h"

void IGraphicsWindow::PreRender()
{
	GraphicsInstance->PreRender(shared_from_this());
}

void IGraphicsWindow::Render(std::vector<std::shared_ptr<Entity>> Entities, std::vector<std::shared_ptr<UIElement>> UIElements)
{
	GraphicsInstance->Render(shared_from_this(), Entities, UIElements);
}

void IGraphicsWindow::PostRender()
{
	GraphicsInstance->PostRender(shared_from_this());
}

