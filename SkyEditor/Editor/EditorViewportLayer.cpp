#include "EditorViewportLayer.h"

#include "EditorApp.h"
#include "Dependencies/ImGui/imgui_internal.h"
#include "Graphics/GraphicsInstance.h"
#include "Platform/Window/EngineWindow.h"
#include "System/LogManager.h"
#include "UI/UIWidget.h"

CEditorViewportLayer::CEditorViewportLayer(TWeakPointer<CEngineWindow> InOwningWindow)
	: CViewportLayer(InOwningWindow)
{
}

void CEditorViewportLayer::OnUpdate()
{
	for (TPointer<CUIWidget> Widget : Widgets)
	{
		Widget->Update();
	}	
	
	CViewportLayer::OnUpdate();
}

void CEditorViewportLayer::OnRender()
{	
	ImGuiDockNode* ViewportNode = ImGui::DockBuilderGetCentralNode(EditorApp->DockSpaceID);
	
	SCanvas ViewportCanvas;
	ViewportCanvas.Position = {ViewportNode->Pos.x,ViewportNode->Pos.y};
	ViewportCanvas.Size = {ViewportNode->Size.x, ViewportNode->Size.y};
	
	for (TPointer<CUIWidget> Widget : Widgets)
	{
		Widget->DrawUI(ViewportCanvas);
	}
	
	// Draw a red rectangle in the central node just for demonstration purposes
	ImGui::GetBackgroundDrawList()->AddRect
	(
		{ViewportNode->Pos.x + 4, ViewportNode->Pos.y + 4},
		{ ViewportNode->Pos.x + ViewportNode->Size.x - 4, ViewportNode->Pos.y + ViewportNode->Size.y - 4 },
		IM_COL32(255, 255, 255, 200),
		0.5f,
		ImDrawFlags_None,
		2.f
	);

	OwningWindow.lock()->GetGraphicsInstance()->SetRenderViewport(SVector2i(ViewportCanvas.Position), SVector2i(ViewportCanvas.Size));

	// TODO: Update to only setup projection when this node/viewport changes size
	CameraManager::GetInstance()->SwitchProjection(EProjectionMode::Perspective);
	
	CViewportLayer::OnRender();

	// Window in viewport instead
	// ImGuiWindowClass centralAlways = {};
	// centralAlways.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_PassthruCentralNode;
	// ImGuiWindowFlags window_flags = 0;
	// window_flags |= ImGuiWindowFlags_NoBackground;
	// window_flags |= ImGuiWindowFlags_NoMouseInputs;
	// window_flags |= ImGuiWindowFlags_NoNavFocus;
	// window_flags |= ImGuiWindowFlags_NoMove;
	// ImGui::SetNextWindowClass(&centralAlways);
	// ImGui::SetNextWindowDockID(node->ID, ImGuiCond_Always);
	// ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	// // ImGui::SetNextWindowBgAlpha(0.0f);
	// ImGui::Begin("Render Viewport", nullptr, window_flags);
	// ImGui::PopStyleVar();
	// // ImGui::Text("Central node: (%f, %f)..(%fx%f)", node->Pos.x, node->Pos.y, node->Size.x, node->Size.y);
	// ImGui::End();	
}

void CEditorViewportLayer::AddViewportWidget(TPointer<CUIWidget> InWidget)
{	
	Widgets.push_back(InWidget);	
	InWidget->SetOwningLayer(this);
}

SVector2i CEditorViewportLayer::GetViewportSize()
{
	if (EditorApp->DockSpaceID == 0)
	{
		return CViewportLayer::GetViewportSize();
	}
	const ImGuiDockNode* CentralViewportNode = ImGui::DockBuilderGetCentralNode(EditorApp->DockSpaceID);
	return {(int)CentralViewportNode->Size.x, (int)CentralViewportNode->Size.y};
}

SVector2i CEditorViewportLayer::GetViewportPosition()
{
	if (EditorApp->DockSpaceID == 0)
	{
		return CViewportLayer::GetViewportPosition();
	}
	const ImGuiDockNode* CentralViewportNode = ImGui::DockBuilderGetCentralNode(EditorApp->DockSpaceID);
	return {(int)CentralViewportNode->Pos.x, (int)CentralViewportNode->Pos.y};
}
