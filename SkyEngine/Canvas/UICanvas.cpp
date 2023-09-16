// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "UICanvas.h"

#include "Core/Application.h"
#include "Platform/Window/EngineWindow.h"
#include "Dependencies/ImGui/imgui.h"
#include "Graphics/GraphicsInstance.h"
#include "Graphics/GL/imgui_impl_opengl3.h"
#include "System/TimeManager.h"
#include "UI/UIWidget.h"

#include "ImGuizmo.h"

CUICanvas::CUICanvas(TWeakPointer<CEngineWindow> InOwningWindow) : CCanvas(InOwningWindow, "UI Layer")
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
	ImGuiIO& Io  = ImGui::GetIO();
	Io.DeltaTime = CTimeManager::GetDeltaTime();

	// TODO: Any overhead? Can be moved to update on window resize event
	const SVector2 WindowSize = SVector2(GetApplication()->GetApplicationWindow()->GetSize());
	Io.DisplaySize = { WindowSize.X, WindowSize.Y};

	ImGui_ImplOpenGL3_NewFrame();
	// ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	
	for (TPointer<CUIWidget> Widget : Widgets)
	{
		Widget->Update();
	}
	bool WantCaptureMouse = ImGui::GetIO().WantCaptureMouse;
	// CLogManager::GetInstance()->DisplayLogMessage(std::format("Wants mouse capture: {}", WantCaptureMouse));
}

void CUICanvas::OnRender()
{	
	static bool show = true;
	// TODO: Temp until overall style setup
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.1f,.1f,.1f,1.0f));
	ImGui::ShowDemoWindow(&show);
	ImGui::PopStyleColor(1);

	// TODO: Change to canvas size
	SCanvas UICanvas;
	UICanvas.Position = {0,0};
	UICanvas.Size = {ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y};
	
	for (TPointer<CUIWidget> Widget : Widgets)
	{
		// TODO: Temp until overall style setup
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.1f,.1f,.1f,1.0f));
		Widget->DrawUI(UICanvas);
		ImGui::PopStyleColor(1);	
	}
	ImGui::Render();
	GetApplication()->GetApplicationWindow()->GetGraphicsInstance()->RenderImGui();

	// TODO: Try link up multi-viewport
	// Update and Render additional Platform Windows
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

void CUICanvas::AddWidget(TPointer<CUIWidget> InWidget)
{
	Widgets.push_back(InWidget);	
	InWidget->SetOwningLayer(this);
}


