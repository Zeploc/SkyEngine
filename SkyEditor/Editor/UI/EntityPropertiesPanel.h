// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "imgui.h"
#include "Canvas/UICanvas.h"
#include "Render/Meshes/MeshComponent.h"

class EditorScene;

class CEntityPropertiesPanel : public CUICanvas
{
public:
	CEntityPropertiesPanel(TWeakPointer<CEngineWindow> InOwningWindow);
	virtual void OnRender() override;

	TPointer<EditorScene> EditorScene;
protected:
	void DrawMeshComponent(const std::shared_ptr<CMeshComponent>& MeshComponent);
	static int MaterialTextChanged(ImGuiInputTextCallbackData* data);
	static int MeshTextChanged(ImGuiInputTextCallbackData* data);

	static TPointer<CMaterialInterface> FindMaterial(std::string MaterialName);
};
