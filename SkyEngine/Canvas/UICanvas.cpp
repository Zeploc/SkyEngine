// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "UICanvas.h"

#include "Core/Application.h"
#include "Platform/Window/EngineWindow.h"
#include "Dependencies/ImGui/imgui.h"
#include "UI/UIWidget.h"

#include "ImGuizmo.h"

CUICanvas::CUICanvas(TWeakPointer<CEngineWindow> InOwningWindow, const std::string& InCanvasName)
: CCanvas(InOwningWindow, InCanvasName)
{
}

CUICanvas::~CUICanvas()
{
	
}

void CUICanvas::OnAttach()
{
	
}

void CUICanvas::OnDetach()
{
}

void CUICanvas::OnUpdate()
{	
	for (TSharedPointer<CUIWidget> Widget : Widgets)
	{
		Widget->Update();
	}
	bool WantCaptureMouse = ImGui::GetIO().WantCaptureMouse;
	// CLogManager::GetInstance()->DisplayLogMessage(std::format("Wants mouse capture: {}", WantCaptureMouse));
}

void CUICanvas::OnRender()
{
	// TODO: Change to canvas size
	SCanvas UICanvas;
	UICanvas.Position = {0,0};
	UICanvas.Size = {ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y};
	
	for (TSharedPointer<CUIWidget> Widget : Widgets)
	{
		// TODO: Temp until overall style setup
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.1f,.1f,.1f,1.0f));
		Widget->DrawUI(UICanvas);
		ImGui::PopStyleColor(1);	
	}
}

void CUICanvas::AddWidget(TSharedPointer<CUIWidget> InWidget)
{
	Widgets.push_back(InWidget);	
	InWidget->SetOwningLayer(this);
}


