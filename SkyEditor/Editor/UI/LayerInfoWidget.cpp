#include "LayerInfoWidget.h"

#include "Dependencies/ImGui/imgui.h"
#include "Canvas/Canvas.h"
#include "Platform/Window/EngineWindow.h"

void CLayerInfoWidget::DrawUI(const SCanvas& DrawCanvas)
{
	bool bOpen = true;
	const float DISTANCE = 10.0f;
	static int corner = 0;
	ImVec2 window_pos = ImVec2((corner & 1) ? DrawCanvas.Size.x - DISTANCE : DISTANCE + DrawCanvas.Position.x, (corner & 2) ? DrawCanvas.Size.y - DISTANCE : DISTANCE + DrawCanvas.Position.y);
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	if (corner != -1)
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
	if (ImGui::Begin("Example: Simple Overlay", &bOpen, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		CCanvas* CapturedLayer = OwningCanvas->GetOwningWindow().lock()->GetCapturedLayer();
		if (CapturedLayer)
		{			
			ImGui::Text("CAPTURED LAYER:");
			ImGui::Separator();
			ImGui::Text(CapturedLayer->GetName().c_str());
		}
		else
		{
			ImGui::Text("No Captured layer");
		}
	}
	ImGui::End();
}
