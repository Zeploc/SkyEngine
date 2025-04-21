#include "EditorViewportCanvas.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.inl>

#include "EditorApp.h"
#include "Core/WorldManager.h"
#include "Core/Asset/Asset.h"
#include "Dependencies/ImGui/imgui_internal.h"
#include "Dependencies/ImGuizmo/ImGuizmo.h"
#include "Entity/Camera.h"
#include "Render/Renderer.h"
#include "Platform/Window/EngineWindow.h"
#include "Render/Framebuffer.h"
#include "Render/SceneRenderer.h"
#include "Render/Meshes/MeshComponent.h"
#include "Scene/SceneManager.h"
#include "System/LogManager.h"
#include "System/TimeManager.h"
#include "UI/UIWidget.h"
#include "Render/Materials/InternalMaterial.h"
#include "Render/Meshes/Basic/DefaultMeshes.h"

CEditorViewportCanvas::CEditorViewportCanvas(TWeakPointer<CEngineWindow> InOwningWindow)
	: CViewportCanvas(InOwningWindow)
{
	CanvasName = "Render Viewport";
}

void CEditorViewportCanvas::OnUpdate()
{
	for (THardPointer<CUIWidget> Widget : Widgets)
	{
		Widget->Update();
	}

	if (EditorApp->DockSpaceID > 0)
	{
		ImGuiDockNode* ViewportNode = ImGui::DockBuilderGetCentralNode(EditorApp->DockSpaceID);
		// TODO: Proper imgui event for resize?
		// Check if viewport size has changed which invalidates buffer
		const SVector2i NewSize = {int(ViewportNode->Size.x), int(ViewportNode->Size.y)};	
		if (ViewportSize != NewSize)
		{
			ViewportSize = NewSize;
			OnViewportResize();
		}
	}
	
	SpectatorUpdate();
	
	CViewportCanvas::OnUpdate();
}

bool CEditorViewportCanvas::PreRender()
{
	ImGuiDockNode* ViewportNode = ImGui::DockBuilderGetCentralNode(EditorApp->DockSpaceID);
	
	// Window in viewport instead
	ImGuiWindowClass centralAlways = {};
	centralAlways.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;
	// window_flags |= ImGuiWindowFlags_NoMouseInputs;
	window_flags |= ImGuiWindowFlags_NoNavFocus;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_AlwaysUseWindowPadding;
	ImGui::SetNextWindowClass(&centralAlways);
	ImGui::SetNextWindowDockID(ViewportNode->ID, ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});
	if (!ImGui::Begin(CanvasName.c_str(), nullptr, window_flags))
	{
		return false;
	}
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
	return true;
}

void CEditorViewportCanvas::PostRender()
{
	CViewportCanvas::PostRender();
	ImGui::PopStyleVar();
}

void CEditorViewportCanvas::OnRender()
{
	CViewportCanvas::OnRender();

	ImGuiDockNode* ViewportNode = ImGui::DockBuilderGetCentralNode(EditorApp->DockSpaceID);
	// TODO: Remove widgets on viewport layer
	SCanvas ViewportCanvas;
	ViewportCanvas.Position = {ViewportNode->Pos.x,ViewportNode->Pos.y};
	ViewportCanvas.Size = {ViewportNode->Size.x, ViewportNode->Size.y};
	
	for (THardPointer<CUIWidget> Widget : Widgets)
	{
		Widget->DrawUI(ViewportCanvas);
	}

	uint32_t TextureID = SceneRenderer->GetFramebuffer()->GetColorAttachmentRendererID();//ViewportTexture->GetTextureRenderID();//
	ImGui::Image((void*)TextureID, ImVec2(ViewportCanvas.Size.X, ViewportCanvas.Size.Y), ImVec2(0, 1), ImVec2(1,0) );
	
	
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(std::format("ASSET:{}", CMaterialInterface::GetStaticName()).c_str()))
		{
			IM_ASSERT(payload->DataSize == sizeof(THardPointer<CAsset>));
			TObjectPointer<CAsset> PayloadAsset = *(const THardPointer<CAsset>*)payload->Data;
			TAssetObjectPointer<CMaterialInterface> DroppedMaterial = PayloadAsset->Load<CMaterialInterface>();
			if (DroppedMaterial)
			{
				UpdateSelectedEntity();
				if (SelectedEntity)
				{
					THardPointer<CMeshComponent> MeshComponent = SelectedEntity->FindComponent<CMeshComponent>();
					if (MeshComponent)
					{
						MeshComponent->SetMaterial(DroppedMaterial);
					}
					SelectEntity(nullptr);
				}
			}
			
		}
		ImGui::EndDragDropTarget();
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

	GizmoTransformSpace = ImGuizmo::MODE::LOCAL;
	if (SelectedEntity && GizmoMode != -1)
	{		
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ViewportNode->Pos.x, ViewportNode->Pos.y, ViewportNode->Size.x, ViewportNode->Size.y);
		glm::mat4 EntityModel = SelectedEntity->Transform.GetModelMatrix();
	
		STransform OriginalTransform = SelectedEntity->Transform;
		STransform Transform = OriginalTransform;
		Transform.FromMatrix(EntityModel);
		
		ImGuizmo::Manipulate(glm::value_ptr(SceneRenderer->GetView().ToGLM()), glm::value_ptr(SceneRenderer->GetProjection().ToGLM()),
		                     ImGuizmo::OPERATION(GizmoMode), ImGuizmo::MODE(GizmoTransformSpace), value_ptr(EntityModel));
		const SRotator OriginalRotation = SelectedEntity->Transform.Rotation;
		SelectedEntity->Transform.FromMatrix(EntityModel);
		// Used to stop gimbol lock
		const SRotator DeltaRotation = SelectedEntity->Transform.Rotation - OriginalRotation;
		SelectedEntity->Transform.Rotation = OriginalRotation;// + DeltaRotation;
	}

}

void CEditorViewportCanvas::AddViewportWidget(THardPointer<CUIWidget> InWidget)
{	
	Widgets.push_back(InWidget);	
	InWidget->SetOwningLayer(this);
}

SVector2i CEditorViewportCanvas::GetViewportSize()
{
	if (EditorApp->DockSpaceID == 0)
	{
		return CViewportCanvas::GetViewportSize();
	}
	const ImGuiDockNode* CentralViewportNode = ImGui::DockBuilderGetCentralNode(EditorApp->DockSpaceID);
	return {(int)CentralViewportNode->Size.x, (int)CentralViewportNode->Size.y};
}

SVector2i CEditorViewportCanvas::GetViewportPosition()
{
	if (EditorApp->DockSpaceID == 0)
	{
		return CViewportCanvas::GetViewportPosition();
	}
	const ImGuiDockNode* CentralViewportNode = ImGui::DockBuilderGetCentralNode(EditorApp->DockSpaceID);
	return {(int)CentralViewportNode->Pos.x, (int)CentralViewportNode->Pos.y};
}

void CEditorViewportCanvas::StartGizmoViewDrag()
{
	bGizmoViewDrag = true;
	InitialViewLockOffset = ViewportCamera->Transform.Position - SelectedEntity->Transform.Position;
}

bool CEditorViewportCanvas::OnMouseButtonPressed(int MouseButton, int Mods)
{
	const THardPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	const SVector2i ScreenCenter = GetViewportPosition() + GetViewportSize() / 2;
	const SVector2i MousePos = ApplicationWindow->GetInput().MousePos;
	
	// TODO: Refine mouse visibility toggle/state
	
	// When ove the gizmo, don't go into normal viewport controls
	if (ImGuizmo::IsOver())
	{
		if (Mods & CWindowInput::ModiferType::Alt)
		{
			std::stringstream DuplicateString;
			DuplicateString << SelectedEntity;
			const THardPointer<Entity> NewEntity = Entity::GetEntityFromStringStream(DuplicateString);
			SceneManager::GetInstance()->GetCurrentScene()->AddEntity(NewEntity);
			SelectEntity(NewEntity);
		}
		else if (Mods & CWindowInput::ModiferType::Shift)
		{
			StartGizmoViewDrag();
		}
		return true;
	}

	bool bHandled = false;
	if (Mods == 0)
	{
		
		// Right click with no mods
		if (MouseButton == GLFW_MOUSE_BUTTON_RIGHT)
		{
			PreviousMousePosition = MousePos;
			ApplicationWindow->SetCursorVisible(false);
			bUseSpectatorControls = true;
			ApplicationWindow->SetCursorPosition(ScreenCenter);
			bHandled = true;
		}
		else if (MouseButton == GLFW_MOUSE_BUTTON_LEFT)
		{
			UpdateSelectedEntity();
			bHandled = true;
		}
	}

	if (Mods & CWindowInput::ModiferType::Alt)
	{
		if (MouseButton == GLFW_MOUSE_BUTTON_LEFT)
		{
			bRotatingAroundPoint = true;
			ApplicationWindow->SetCursorVisible(false);
			PreviousMousePosition = MousePos;
			ApplicationWindow->SetCursorPosition(ScreenCenter);
		}
		else if (MouseButton == GLFW_MOUSE_BUTTON_RIGHT)
		{
			bDollyingOnFocusPoint = true;
			ApplicationWindow->SetCursorVisible(false);
			PreviousMousePosition = MousePos;
			ApplicationWindow->SetCursorPosition(ScreenCenter);
		}
	}
	if (Mods & CWindowInput::ModiferType::Shift)
	{
		if (MouseButton == GLFW_MOUSE_BUTTON_LEFT)
		{
			ApplicationWindow->SetCursorVisible(false);
			PreviousMousePosition = MousePos;
			ApplicationWindow->SetCursorPosition(ScreenCenter);
			bPanning = true;
		}
	}

	bHandled |= bRotatingAroundPoint;
	bHandled |= bDollyingOnFocusPoint;
	bHandled |= bPanning;
	if (bHandled)
	{
		return true;
	}
	
	return false;
}

bool CEditorViewportCanvas::OnMouseButtonReleased(int MouseButton, int Mods)
{	
	const THardPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	
	if (MouseButton == GLFW_MOUSE_BUTTON_RIGHT)
	{
		ApplicationWindow->SetCursorVisible(true);
		bUseSpectatorControls = false;
		ApplicationWindow->SetCursorPosition(PreviousMousePosition);
		bDollyingOnFocusPoint = false;
		return true;
	}
	if (MouseButton == GLFW_MOUSE_BUTTON_LEFT && (bPanning || bRotatingAroundPoint))
	{
		ApplicationWindow->SetCursorVisible(true);
		ApplicationWindow->SetCursorPosition(PreviousMousePosition);
		bPanning = false;
		bRotatingAroundPoint = false;
		return true;
	}
	if (MouseButton == GLFW_MOUSE_BUTTON_LEFT)
	{
		bGizmoViewDrag = false;
		return true;
	}
	return false;
}

bool CEditorViewportCanvas::OnMouseMoved(SVector2i MousePos)
{
	const THardPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	const SVector2i ScreenCenter = GetViewportPosition() + GetViewportSize() / 2;
	const SVector2i ScreenOffset = (MousePos) - ScreenCenter;
	const SVector2 Offset = SVector2(ScreenOffset) * MouseSensitivity;
	
	if (bGizmoViewDrag)
	{
		// TODO: Needs improvement, has accelerate feel as its not simple the mouse movement on the gizmo
		ViewportCamera->Transform.Position = SelectedEntity->Transform.Position + InitialViewLockOffset;
		return true;
	}
	
	const SVector CameraPivotPoint = ViewportCamera->Transform.Position + ViewportCamera->GetForwardVector() * CurrentFocusDistance;
	if (bRotatingAroundPoint)
	{		
		SVector NewCameraForwardVector = ViewportCamera->GetForwardVector();
		NewCameraForwardVector.Rotate(Offset.X, SVector(0,1,0));
		NewCameraForwardVector.Rotate(Offset.Y, ViewportCamera->GetRightVector());
		ViewportCamera->SetForwardVector(NewCameraForwardVector);
		ViewportCamera->Transform.Position = CameraPivotPoint + (-ViewportCamera->GetForwardVector() * CurrentFocusDistance);
		ApplicationWindow->SetCursorPosition(ScreenCenter);
		return true;
	}
	if (bDollyingOnFocusPoint)
	{	
		CurrentFocusDistance += Offset.Y * .3f * -1.0f;
		ApplicationWindow->SetCursorPosition(ScreenCenter);
		ViewportCamera->Transform.Position = CameraPivotPoint + (-ViewportCamera->GetForwardVector() * CurrentFocusDistance);
		return true;
	}
	if (bPanning)
	{		
		SVector NewCameraPosition = ViewportCamera->Transform.Position;
		NewCameraPosition += ViewportCamera->GetRightVector() * Offset.X;
		NewCameraPosition += -ViewportCamera->GetUpVector() * Offset.Y;
		ViewportCamera->Transform.Position = NewCameraPosition;
		ApplicationWindow->SetCursorPosition(ScreenCenter);
		return true;
	}
	if (bUseSpectatorControls)
	{
		SVector NewCameraForwardVector = ViewportCamera->GetForwardVector();
		NewCameraForwardVector.Rotate(Offset.X, SVector(0,1,0));
		NewCameraForwardVector.Rotate(Offset.Y, ViewportCamera->GetRightVector());
		ViewportCamera->SetForwardVector(NewCameraForwardVector);
		ApplicationWindow->SetCursorPosition(ScreenCenter);
	}
	
	return false;
}

bool CEditorViewportCanvas::OnMouseScrolled(float XOffset, float YOffset)
{
	return false;
}

void CEditorViewportCanvas::FocusEntity()
{
	ViewportCamera->Transform.Position = SelectedEntity->Transform.Position + (-ViewportCamera->GetForwardVector() * CurrentFocusDistance);
}

bool CEditorViewportCanvas::DeleteSelected()
{
	if (!SelectedEntity)
	{
		return false;
	}
	const TAssetObjectPointer<Scene> Scene = SceneManager::GetInstance()->GetCurrentScene();
	Scene->DestroyEntity(SelectedEntity);
	SelectedEntity = nullptr;
	SelectEntity(nullptr);
	return true;
}

void CEditorViewportCanvas::CreateEntity(TAssetObjectPointer<CMesh> Mesh)
{
	std::string EntityName = "NewEntity" ;
	if (Mesh)
	{
		EntityName = Mesh->Asset->DisplayName;
	}
	const THardPointer<Entity> NewEntity(new Entity(STransform(), EntityName));
	SceneManager::GetInstance()->GetCurrentScene()->AddEntity(NewEntity);
	SelectEntity(NewEntity, true);
	if (Mesh.IsValid())
	{
		THardPointer<CMeshComponent> NewMeshComponent = std::make_shared<CMeshComponent>(NewEntity, Mesh, nullptr);
		NewEntity->AddComponent(NewMeshComponent);
	}
}

bool CEditorViewportCanvas::OnKeyPressed(int KeyCode, int Mods, int RepeatCount)
{
	if (bUseSpectatorControls)
	{
		return false;
	}
	if (ImGuizmo::IsUsing() && KeyCode == GLFW_KEY_LEFT_SHIFT)
	{			
		StartGizmoViewDrag();
	}
	if (bPanning || bDollyingOnFocusPoint || bRotatingAroundPoint)
	{
		return false;
	}
	if (KeyCode == GLFW_KEY_ESCAPE) // Escape
	{
		if (SelectedEntity)
		{
			SelectEntity(nullptr);
			return true;
		}
		// ApplicationWindow->SetCursorVisible(true);
	}
	if (KeyCode == GLFW_KEY_F && Mods & CWindowInput::ModiferType::Control)
	{
		CreateEntity(DefaultMesh::GetCube());
		return true;
	}
	if (KeyCode == GLFW_KEY_G && Mods & CWindowInput::ModiferType::Control)
	{
		CreateEntity(DefaultMesh::GetSphere());
		return true;
	}
	if (KeyCode == GLFW_KEY_F)
	{
		if (SelectedEntity)
		{
			FocusEntity();
			return true;
		}
	}
	if (KeyCode == GLFW_KEY_G)
	{
		bWireframe = !bWireframe;
		const THardPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
		GetGraphicsAPI()->SetWireframeMode(bWireframe);
		return true;
	}
	if (KeyCode == GLFW_KEY_DELETE)
	{
		if (DeleteSelected())
		{
			return true;
		}
	}
	if (KeyCode == GLFW_KEY_Q)
	{
		// TODO: Enable once stencil selection is implemented
		// GizmoMode = -1;
		// return true;
	}
	if (KeyCode == GLFW_KEY_E)
	{
		GizmoMode = ImGuizmo::OPERATION::ROTATE;
		return true;
	}
	if (KeyCode == GLFW_KEY_R)
	{
		GizmoMode = ImGuizmo::OPERATION::SCALE;
		return true;
	}
	if (KeyCode == GLFW_KEY_W)
	{
		GizmoMode = ImGuizmo::OPERATION::TRANSLATE;
		return true;
	}
	return false;
}

void CEditorViewportCanvas::SpectatorUpdate()
{
	if (!bUseSpectatorControls)
	{
		return;
	}

	const SVector ForwardMovement = ViewportCamera->GetForwardVector() * CameraSpeed * CTimeManager::GetDeltaTime();
	
	const THardPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	if (ApplicationWindow->GetInput().KeyState[GLFW_KEY_W] == CWindowInput::INPUT_HOLD)
	{
		ViewportCamera->Transform.Position += ForwardMovement;
	}
	else if (ApplicationWindow->GetInput().KeyState[GLFW_KEY_S] == CWindowInput::INPUT_HOLD)
	{
		ViewportCamera->Transform.Position -= ForwardMovement;
	}
	
	const SVector RightMovement = ViewportCamera->GetRightVector() * CameraSpeed * CTimeManager::GetDeltaTime();
	if (ApplicationWindow->GetInput().KeyState[GLFW_KEY_A] == CWindowInput::INPUT_HOLD)
	{
		ViewportCamera->Transform.Position -= RightMovement;
	}
	else if (ApplicationWindow->GetInput().KeyState[GLFW_KEY_D] == CWindowInput::INPUT_HOLD)
	{
		ViewportCamera->Transform.Position += RightMovement;
	}
	
	const SVector UpMovement = ViewportCamera->GetUpVector() * CameraSpeed * CTimeManager::GetDeltaTime();
	if (ApplicationWindow->GetInput().KeyState[GLFW_KEY_E] == CWindowInput::INPUT_HOLD)
	{
		ViewportCamera->Transform.Position += UpMovement;
	}
	else if (ApplicationWindow->GetInput().KeyState[GLFW_KEY_Q] == CWindowInput::INPUT_HOLD)
	{
		ViewportCamera->Transform.Position -= UpMovement;
	}
}

bool CEditorViewportCanvas::OnKeyTyped(int KeyCode, int Mods)
{
	return false;
}

bool CEditorViewportCanvas::OnKeyReleased(int KeyCode, int Mods)
{
	if (KeyCode == GLFW_KEY_LEFT_SHIFT)
	{		
		bGizmoViewDrag = false;
	}
	return false;
}

bool CEditorViewportCanvas::OnWindowResize(unsigned int Width, unsigned int Height)
{
	return false;
}

void CEditorViewportCanvas::SelectEntity(THardPointer<Entity> HitEntity, bool bFocusCamera)
{
	if (HitEntity == SelectedEntity)
	{
		return;
	}
	SelectedEntity = HitEntity;
	if (HitEntity)
	{
		// TransformationWidget->Transform.Position = HitEntity->Transform.Position;
	}
	const bool bNewVisibleState = HitEntity != nullptr;
	// if (TransformationWidget->IsVisible() != bNewVisibleState)
	{
		// TransformationWidget->SetVisible(bNewVisibleState);
		if (bNewVisibleState)
		{
			CurrentFocusDistance = 7.0f;
			if (bFocusCamera)
			{
				FocusEntity();
				
			}
		}
	}
}

void CEditorViewportCanvas::UpdateSelectedEntity()
{
	TAssetObjectPointer<Scene> Scene = SceneManager::GetInstance()->GetCurrentScene();

	THardPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	SVector2i MousePos = ApplicationWindow->GetInput().MousePos;
	SVector rayStart = ViewportCamera->Transform.Position;
	SVector RayDirection = ScreenToWorldDirection(MousePos);
	SVector HitPos;
	std::vector<THardPointer<Entity>> HitEntities;
	std::vector<SVector> HitPosition;
	for (auto& Ent : Scene->Entities)
	{
		if (!Ent->bRayCast || !Ent->IsVisible())
		{
			continue; // Don't check for raycast
		}
		if (Ent->CheckHit(rayStart, RayDirection, HitPos))
		{
			HitEntities.push_back(Ent);
			HitPosition.push_back(HitPos);
		}
	}
	// Poor mans depth testing
	if (!HitEntities.empty())
	{
		int ClosestHitID = 0;
		for (int i = 0; i < HitEntities.size(); i++)
		{
			// If new hit is closer
			if (glm::length(rayStart - HitPosition[i]) < glm::length(rayStart - HitPosition[ClosestHitID]))
			{
				ClosestHitID = i;
			}
		}
		THardPointer<Entity> HitEntity = HitEntities[ClosestHitID]; // Hit ent
		HitPos = HitPosition[ClosestHitID];

		SelectEntity(HitEntity);
	}
	else
	{		
		SelectEntity(nullptr);
	}
}