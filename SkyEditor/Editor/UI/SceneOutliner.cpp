#include "SceneOutliner.h"

#include "EditorApp.h"
#include "Dependencies/ImGui/imgui.h"
#include "Editor/EditorViewportCanvas.h"
#include "Editor/Scene/EditorScene.h"
#include "Entity/Entity.h"
#include "Scene/SceneManager.h"

CSceneOutliner::CSceneOutliner(TWeakPointer<CEngineWindow> InOwningWindow)
: CUICanvas(InOwningWindow, "Scene Outliner")
{
	// TODO: Remove once redundant
	StartingSize = {430, 450};
    EditorScene = SceneManager::GetInstance()->GetCurrentScene();
	// TODO: Make button to open and remove
	bCanClose = false;
}

void CSceneOutliner::OnRender()
{
    EditorScene = SceneManager::GetInstance()->GetCurrentScene();
	// TODO: Convert to widget and use base render
	if (ImGui::BeginListBox("##SceneOutlinerEntityList", ImVec2(-FLT_MIN, -FLT_MIN)))
	{
		const TPointer<Entity> SelectedEntity = EditorApp->EditorViewportLayer->GetSelectedEntity();
		TPointer<Entity> NewSelectedEntity = nullptr;
		for (TPointer<Entity> Entity : EditorScene->Entities)
		{
			if (ImGui::Selectable((Entity->GetEntityName() + "##" + std::to_string(Entity->GetEntityID())).c_str(), Entity == SelectedEntity))
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
