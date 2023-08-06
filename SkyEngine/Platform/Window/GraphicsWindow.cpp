// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "GraphicsWindow.h"

#include "Core/Application.h"
#include "Graphics/GraphicsInstance.h"
#include "Input/Input.h"

IGraphicsWindow::IGraphicsWindow(std::string InWindowName, SVector2i InWindowSize, bool bFullScreen)
{
}

void IGraphicsWindow::CreateGraphicsInstance()
{
	GraphicsInstance = GetGraphicsAPI()->CreateNewInstance();
}

void IGraphicsWindow::SetCursorPosition(SVector2i InCursorPosition)
{
	Input::GetInstance()->MouseInput(InCursorPosition.x, InCursorPosition.y);
}

void IGraphicsWindow::PreRender()
{
	GraphicsInstance->PreRender(shared_from_this());
}

void IGraphicsWindow::Render(std::vector<TPointer<Entity>> Entities, std::vector<TPointer<UIElement>> UIElements)
{
	GraphicsInstance->Render(shared_from_this(), Entities, UIElements);
}

void IGraphicsWindow::PostRender()
{
	GraphicsInstance->PostRender(shared_from_this());
}

