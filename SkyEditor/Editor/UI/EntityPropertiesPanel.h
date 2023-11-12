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

protected:
	void DrawMeshComponent(const std::shared_ptr<CMeshComponent>& MeshComponent);

	void MaterialsDropdown(const std::shared_ptr<CMeshComponent>& MeshComponent);
	void MeshDropdown(const std::shared_ptr<CMeshComponent>& MeshComponent);
};
