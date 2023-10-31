#include "SceneOutliner.h"

#include "EditorApp.h"
#include "Dependencies/ImGui/imgui.h"
#include "Editor/EditorViewportCanvas.h"
#include "Editor/Scene/EditorScene.h"
#include "Editor/Scene/TransformationWidget.h"
#include "Entity/Entity.h"
#include "Scene/SceneManager.h"

CSceneOutliner::CSceneOutliner(TWeakPointer<CEngineWindow> InOwningWindow)
: CUICanvas(InOwningWindow, "Scene Outliner")
{
	// TODO: Remove once redundant
    EditorScene = std::static_pointer_cast<class EditorScene>(SceneManager::GetInstance()->GetCurrentScene());
	StartingSize = {430, 450};    
}

void CSceneOutliner::OnRender()
{
	// TODO: Convert to widget and use base render
	if (ImGui::BeginListBox("##SceneOutlinerEntityList", ImVec2(-FLT_MIN, -FLT_MIN)))
	{
		const TPointer<Entity> SelectedEntity = EditorApp->EditorViewportLayer->GetSelectedEntity();
		TPointer<Entity> NewSelectedEntity = nullptr;
		for (TPointer<Entity> Entity : EditorScene->Entities)
		{
			// TODO: Temp until gizmo updated
			if (Entity == EditorScene->TransformationWidget->XMoveTransform ||
				Entity == EditorScene->TransformationWidget->YMoveTransform ||
				Entity == EditorScene->TransformationWidget->ZMoveTransform)
			{
				continue;
			}
			if (ImGui::Selectable(Entity->GetEntityName().c_str(), Entity == SelectedEntity))
			{
				NewSelectedEntity = Entity;
			}
		}
		if (NewSelectedEntity)
		{
			EditorApp->EditorViewportLayer->SelectEntity(NewSelectedEntity);
		}
		ImGui::EndListBox();
	}
}
