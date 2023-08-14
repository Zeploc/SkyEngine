#include "SceneOutliner.h"

#include "Dependencies/ImGui/imgui.h"
#include "Editor/Scene/EditorScene.h"
#include "Editor/Scene/TransformationWidget.h"
#include "Entity/Entity.h"
#include "Scene/SceneManager.h"

CSceneOutliner::CSceneOutliner()
{
    EditorScene = std::static_pointer_cast<class EditorScene>(SceneManager::GetInstance()->GetCurrentScene());
}

void CSceneOutliner::DrawUI(const SCanvas& DrawCanvas)
{
    static bool bOpen = true;
    ImGui::SetNextWindowSize(ImVec2(430,450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Scene Outliner", &bOpen))
    {
        ImGui::End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
    
    if (ImGui::BeginListBox("##SceneOutlinerEntityList", ImVec2(-FLT_MIN, -FLT_MIN)))
    {
        const TPointer<Entity> SelectedEntity = EditorScene->TransformationWidget->SelectedEntity;
        TPointer<Entity> NewSelectedEntity = nullptr;
        for (TPointer<Entity> Entity : EditorScene->Entities)
        {
            // Temp until gizmo updated
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
            EditorScene->SelectEntity(NewSelectedEntity);
        }
        ImGui::EndListBox();
    }

    ImGui::PopStyleVar();
    ImGui::End();
}
