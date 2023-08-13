#include "LayerInfoWidget.h"

#include "Dependencies/ImGui/imgui.h"
#include "Layers/Layer.h"
#include "Layers/UILayer.h"
#include "Platform/Window/EngineWindow.h"

void CLayerInfoWidget::DrawUI()
{
	bool bOpen = true;
	const float DISTANCE = 10.0f;
	static int corner = 0;
	ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	if (corner != -1)
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
	if (ImGui::Begin("Example: Simple Overlay", &bOpen, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		CLayer* CapturedLayer = OwningLayer->GetOwningWindow().lock()->GetCapturedLayer();
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
