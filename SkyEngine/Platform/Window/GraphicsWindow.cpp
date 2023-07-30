#include "GraphicsWindow.h"

#include "Core/Application.h"
#include "Graphics/GraphicsInstance.h"

IGraphicsWindow::IGraphicsWindow(std::string InWindowName, Vector2 InWindowSize, bool bFullScreen)
{
}

void IGraphicsWindow::CreateGraphicsInstance()
{
	GraphicsInstance = GetGraphicsAPI()->CreateNewInstance();
}

void IGraphicsWindow::PreRender()
{
	GraphicsInstance->PreRender(shared_from_this());
}

void IGraphicsWindow::Render(std::vector<Pointer<Entity>> Entities, std::vector<Pointer<UIElement>> UIElements)
{
	GraphicsInstance->Render(shared_from_this(), Entities, UIElements);
}

void IGraphicsWindow::PostRender()
{
	GraphicsInstance->PostRender(shared_from_this());
}

