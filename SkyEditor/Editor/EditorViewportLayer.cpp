#include "EditorViewportLayer.h"

#include <glm/gtc/type_ptr.inl>

#include "EditorApp.h"
#include "Dependencies/ImGui/imgui_internal.h"
#include "Dependencies/ImGuizmo/ImGuizmo.h"
#include "Graphics/GraphicsInstance.h"
#include "Platform/Window/EngineWindow.h"
#include "Scene/SceneManager.h"
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

	CameraManager* Camera = CameraManager::GetInstance();
	// TODO: Update to only setup projection when this node/viewport changes size
	Camera->SwitchProjection(EProjectionMode::Perspective);
	
	CViewportLayer::OnRender();

	if (SelectedEntity)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ViewportNode->Pos.x, ViewportNode->Pos.y, ViewportNode->Size.x, ViewportNode->Size.y);
		glm::mat4 EntityModel = SelectedEntity->Transform.GetModelMatrix();
		ImGuizmo::Manipulate(glm::value_ptr(Camera->View.ToGLM()), glm::value_ptr(Camera->Projection.ToGLM()),
		                     ImGuizmo::OPERATION::ROTATE, ImGuizmo::MODE::LOCAL, value_ptr(EntityModel));
		const SRotator OriginalRotation = SelectedEntity->Transform.Rotation;
		SelectedEntity->Transform.FromMatrix(EntityModel);
		// Used to stop gimbol lock
		const SRotator DeltaRotation = SelectedEntity->Transform.Rotation - OriginalRotation;
		// SelectedEntity->Transform.Rotation = OriginalRotation + DeltaRotation;
	}

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

void CEditorViewportLayer::OnEvent(CEvent& Event)
{
	CViewportLayer::OnEvent(Event);	
	
	EventDispatcher dispatcher(Event);
	dispatcher.Dispatch<CMouseButtonPressedEvent>(SE_BIND_EVENT_FN(CEditorViewportLayer::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<CMouseButtonReleasedEvent>(SE_BIND_EVENT_FN(CEditorViewportLayer::OnMouseButtonReleasedEvent));
	dispatcher.Dispatch<CMouseMovedEvent>(SE_BIND_EVENT_FN(CEditorViewportLayer::OnMouseMovedEvent));
	dispatcher.Dispatch<CMouseScrolledEvent>(SE_BIND_EVENT_FN(CEditorViewportLayer::OnMouseScrolledEvent));
	dispatcher.Dispatch<CKeyPressedEvent>(SE_BIND_EVENT_FN(CEditorViewportLayer::OnKeyPressedEvent));
	dispatcher.Dispatch<CKeyTypedEvent>(SE_BIND_EVENT_FN(CEditorViewportLayer::OnKeyTypedEvent));
	dispatcher.Dispatch<CKeyReleasedEvent>(SE_BIND_EVENT_FN(CEditorViewportLayer::OnKeyReleasedEvent));
	dispatcher.Dispatch<CWindowResizeEvent>(SE_BIND_EVENT_FN(CEditorViewportLayer::OnWindowResizeEvent));
}

bool CEditorViewportLayer::OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event)
{
	CameraManager* CameraInstance = CameraManager::GetInstance();
	const TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	const SVector2i ScreenCenter = EditorApp->EditorViewportLayer->GetViewportPosition() + CameraInstance->GetScreenCenter();
	const SVector2i MousePos = ApplicationWindow->GetInput().MousePos;
	
	// TODO: Refine mouse visibility toggle/state

	bool bHandled = false;
	if (Event.GetMods() == 0)
	{
		// Right click with no mods
		if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
		{
			PreviousMousePosition = MousePos;
			ApplicationWindow->SetCursorVisible(false);
			CameraInstance->EnableSpectatorControls(true);
			bHandled = true;
		}
		else if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
		{
			UpdateSelectedEntity();
			bHandled = true;
		}
	}

	if (Event.GetMods() & CInput::ModiferType::Alt)
	{
		if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
		{
			bRotatingAroundPoint = true;
			ApplicationWindow->SetCursorVisible(false);
			PreviousMousePosition = MousePos;
			ApplicationWindow->SetCursorPosition(ScreenCenter);
		}
		else if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
		{
			bLookingAround = true;
			ApplicationWindow->SetCursorVisible(false);
			PreviousMousePosition = MousePos;
			ApplicationWindow->SetCursorPosition(ScreenCenter);
		}
	}
	if (Event.GetMods() & CInput::ModiferType::Shift)
	{
		if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
		{
			ApplicationWindow->SetCursorVisible(false);
			PreviousMousePosition = MousePos;
			ApplicationWindow->SetCursorPosition(ScreenCenter);
			bPanning = true;
		}
	}

	bHandled |= bRotatingAroundPoint;
	bHandled |= bLookingAround;
	bHandled |= bPanning;
	if (bHandled)
	{
		return true;
	}
	
	return false;
}

bool CEditorViewportLayer::OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event)
{	
	CameraManager* CameraInstance = CameraManager::GetInstance();
	const TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	
	if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
	{
		ApplicationWindow->SetCursorVisible(true);
		CameraInstance->EnableSpectatorControls(false);
		ApplicationWindow->SetCursorPosition(PreviousMousePosition);
		bLookingAround = false;
		return true;
	}
	if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT && (bPanning || bRotatingAroundPoint))
	{
		ApplicationWindow->SetCursorVisible(true);
		ApplicationWindow->SetCursorPosition(PreviousMousePosition);
		bPanning = false;
		bRotatingAroundPoint = false;
		return true;
	}
	if (Event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
	{
		return true;
	}
	return false;
}

bool CEditorViewportLayer::OnMouseMovedEvent(CMouseMovedEvent& Event)
{
	const TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	const SVector2i ScreenCenter = EditorApp->EditorViewportLayer->GetViewportPosition() + EditorApp->EditorViewportLayer->GetViewportSize() / 2;
	CameraManager* CameraInstance = CameraManager::GetInstance();
	const SVector2i ScreenOffset = Event.GetMousePos() - ScreenCenter;
	const SVector2 Offset = SVector2(ScreenOffset) * CameraInstance->MouseSensitivity;	
	
	const SVector CameraPivotPoint = CameraInstance->GetCameraPosition() + CameraInstance->GetCameraForwardVector() * CurrentFocusDistance;
	if (bRotatingAroundPoint)
	{		
		SVector NewCameraForwardVector = CameraInstance->GetCameraForwardVector();
		NewCameraForwardVector.Rotate(Offset.X, SVector(0,1,0));
		NewCameraForwardVector.Rotate(Offset.Y, CameraInstance->GetCameraRightVector());
		CameraInstance->SetCameraForwardVector(NewCameraForwardVector);
		CameraInstance->SetCameraPos(CameraPivotPoint + (-CameraInstance->GetCameraForwardVector() * CurrentFocusDistance));
		ApplicationWindow->SetCursorPosition(ScreenCenter);
		return true;
	}
	if (bLookingAround)
	{	
		CurrentFocusDistance += Offset.Y * .3f;
		ApplicationWindow->SetCursorPosition(ScreenCenter);
		CameraInstance->SetCameraPos(CameraPivotPoint + (-CameraInstance->GetCameraForwardVector() * CurrentFocusDistance));
		return true;
	}
	if (bPanning)
	{		
		SVector NewCameraPosition = CameraInstance->GetCameraPosition();
		NewCameraPosition += CameraInstance->GetCameraRightVector() * Offset.X * CameraInstance->MouseSensitivity;
		NewCameraPosition += -CameraInstance->GetCameraUpVector() * Offset.Y * CameraInstance->MouseSensitivity;
		CameraInstance->SetCameraPos(NewCameraPosition);
		ApplicationWindow->SetCursorPosition(ScreenCenter);
		return true;
	}
	
	return false;
}

bool CEditorViewportLayer::OnMouseScrolledEvent(CMouseScrolledEvent& Event)
{
	return false;
}

bool CEditorViewportLayer::OnKeyPressedEvent(CKeyPressedEvent& Event)
{
	if (Event.GetKeyCode() == GLFW_KEY_ESCAPE) // Escape
	{
		if (SelectedEntity)
		{
			SelectEntity(nullptr);
			return true;
		}
		// ApplicationWindow->SetCursorVisible(true);
		// CameraInstance->EnableSpectatorControls(false);
	}
	if (Event.GetKeyCode() == GLFW_KEY_F)
	{
		if (SelectedEntity)
		{
			CameraManager* CameraInstance = CameraManager::GetInstance();
			CameraInstance->SetCameraPos(SelectedEntity->Transform.Position + (-CameraInstance->GetCameraForwardVector() * CurrentFocusDistance));
			return true;
		}
	}
	if (Event.GetKeyCode() == GLFW_KEY_G)
	{
		bWireframe = !bWireframe;
		const TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
		ApplicationWindow->GetGraphicsInstance()->SetWireframeMode(bWireframe);
		return true;
	}
	return false;
}

bool CEditorViewportLayer::OnKeyTypedEvent(CKeyTypedEvent& Event)
{
	return false;
}

bool CEditorViewportLayer::OnKeyReleasedEvent(CKeyReleasedEvent& Event)
{
	return false;
}

bool CEditorViewportLayer::OnWindowResizeEvent(CWindowResizeEvent& Event)
{
	return false;
}

void CEditorViewportLayer::SelectEntity(TPointer<Entity> HitEntity)
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
		}
	}
}

void CEditorViewportLayer::UpdateSelectedEntity()
{
	// TODO: Remove once redundant
	if (!EditorScene)
	{
		EditorScene = std::static_pointer_cast<class EditorScene>(SceneManager::GetInstance()->GetCurrentScene());
	}
	
	TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	SVector2i MousePos = ApplicationWindow->GetInput().MousePos;
	SVector rayStart = CameraManager::GetInstance()->GetCameraPosition();
	SVector RayDirection = CameraManager::GetInstance()->ScreenToWorldDirection(MousePos);
	SVector HitPos;
	std::vector<TPointer<Entity>> HitEntities;
	std::vector<SVector> HitPosition;
	for (auto& Ent : EditorScene->Entities)
	{
		if (!Ent->bRayCast || !Ent->EntityMesh || !Ent->IsVisible())
		{
			continue; // Don't check for raycast
		}
		// TODO: Fix check hit for plane and pryamid
		if (Ent->CheckHit(rayStart, RayDirection, HitPos))
		{
			HitEntities.push_back(Ent);
			HitPosition.push_back(HitPos);
		}
	}
	if (HitEntities.size() > 0)
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
		TPointer<Entity> HitEntity = HitEntities[ClosestHitID]; // Hit ent
		HitPos = HitPosition[ClosestHitID];

		SelectEntity(HitEntity);
	}
}
