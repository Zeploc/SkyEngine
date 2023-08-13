// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "UILayer.h"

#include "imgui_internal.h"
#include "Core/Application.h"
#include "Platform/Window/EngineWindow.h"
#include "Dependencies/ImGui/imgui.h"
#include "Graphics/GraphicsInstance.h"
#include "Graphics/GL/imgui_impl_opengl3.h"
#include "Platform/Window/GLFW/GLFWWindow.h"
#include "Platform/Window/GLFW/imgui_impl_glfw.h"
#include "System/LogManager.h"
#include "System/TimeManager.h"
#include "UI/UIWidget.h"

CUILayer::CUILayer(TWeakPointer<CEngineWindow> InOwningWindow) : CLayer(InOwningWindow, "UI Layer")
{
}

CUILayer::~CUILayer()
{
	
}

void CUILayer::OnAttach()
{
	GuiContext = ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
	TPointer<CGLFWWindow> GraphicsWindow = std::static_pointer_cast<CGLFWWindow>(GetApplication()->GetApplicationWindow());
	// ImGui_ImplGlfw_InitForOpenGL(GraphicsWindow->GetGlWindow(), true);
	
	ImGuiIO& Io = ImGui::GetIO();
	Io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	Io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	Io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	
	// TODO: TEMPORARY: swap with sky engine keycodes
	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	Io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	Io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	Io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	Io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	Io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	Io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	Io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	Io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	Io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	Io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
	Io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	Io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	Io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
	Io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	Io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	Io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	Io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	Io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	Io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	Io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	Io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
	//
	ImGui_ImplOpenGL3_Init("#version 410 core");
}

void CUILayer::OnDetach()
{
}

void CUILayer::OnUpdate()
{
	ImGuiIO& Io  = ImGui::GetIO();
	Io.DeltaTime = CTimeManager::GetDeltaTime();
	const SVector2 WindowSize = SVector2(GetApplication()->GetApplicationWindow()->GetSize());
	Io.DisplaySize = { WindowSize.X, WindowSize.Y};

	ImGui_ImplOpenGL3_NewFrame();
	// ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	for (TPointer<CUIWidget> Widget : Widgets)
	{
		Widget->Update();
	}
	bool WantCaptureMouse = ImGui::GetIO().WantCaptureMouse;
	// CLogManager::GetInstance()->DisplayLogMessage(std::format("Wants mouse capture: {}", WantCaptureMouse));
}

void CUILayer::OnRender()
{	
	static bool show = true;
	ImGui::ShowDemoWindow(&show);
	
	for (TPointer<CUIWidget> Widget : Widgets)
	{
		Widget->DrawUI();
	}
	ImGui::Render();
	GetApplication()->GetApplicationWindow()->GetGraphicsInstance()->RenderImGui();
}

void CUILayer::OnEvent(CEvent& Event)
{
	EventDispatcher dispatcher(Event);
	dispatcher.Dispatch<CMouseButtonPressedEvent>(SE_BIND_EVENT_FN(CUILayer::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<CMouseButtonReleasedEvent>(SE_BIND_EVENT_FN(CUILayer::OnMouseButtonReleasedEvent));
	dispatcher.Dispatch<CMouseMovedEvent>(SE_BIND_EVENT_FN(CUILayer::OnMouseMovedEvent));
	dispatcher.Dispatch<CMouseScrolledEvent>(SE_BIND_EVENT_FN(CUILayer::OnMouseScrolledEvent));
	dispatcher.Dispatch<CKeyPressedEvent>(SE_BIND_EVENT_FN(CUILayer::OnKeyPressedEvent));
	dispatcher.Dispatch<CKeyTypedEvent>(SE_BIND_EVENT_FN(CUILayer::OnKeyTypedEvent));
	dispatcher.Dispatch<CKeyReleasedEvent>(SE_BIND_EVENT_FN(CUILayer::OnKeyReleasedEvent));
	// TEMP TEST
	// SVector2i MousePos = GetApplication()->GetApplicationWindow()->GetInput().MousePos;
	// return MousePos.X < 500 && MousePos.Y < 500;
	// return ImGui::IsAnyWindowFocused();
}

void CUILayer::AddWidget(TPointer<CUIWidget> InWidget)
{
	Widgets.push_back(InWidget);	
	InWidget->SetOwningLayer(this);
}

bool CUILayer::OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event)
{
	if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	//if (!ImGui::GetIO().WantCaptureMouse)
	{
		ImGui::SetWindowFocus();
		return false;
	}
	// Update buttons
	ImGuiIO& io = ImGui::GetIO();
	for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
	{
		CInput::InputState MouseButtonState = (CInput::InputState)OwningWindow.lock()->GetInput().MouseState[i];
		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[i] = MouseButtonState == CInput::INPUT_FIRST_PRESS || MouseButtonState == CInput::INPUT_HOLD;
	}

	// Won't be hovered properly since mouse pos only updated in imgui when window is focused
	CLogManager::GetInstance()->DisplayLogMessage(std::format("hovering: {}", ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)));

	// if (!ImGui::GetIO().WantCaptureMouse)
	// {
	// 	ImGui::SetWindowFocus();
	// 	return false;
	// }
	return true;
}

bool CUILayer::OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event)
{
	bool bHadCapture = ImGui::GetIO().WantCaptureMouse;
	
	// Update buttons
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[Event.GetMouseButton()] = false;

	// If hovering window on release or mouse is not released (captured by button), then the release is handled
	return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || !ImGui::IsMouseReleased(Event.GetMouseButton());
}

bool CUILayer::OnMouseMovedEvent(CMouseMovedEvent& Event)
{
	ImGuiIO& io = ImGui::GetIO();
	const ImVec2 mouse_pos_backup = io.MousePos;
	io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	const bool bFocused = OwningWindow.lock()->IsFocused();
	if (bFocused)
	{
		if (io.WantSetMousePos)
		{
			OwningWindow.lock()->SetCursorPosition({(int)mouse_pos_backup.x, (int)mouse_pos_backup.y});
		}
		else
		{
			SVector2i MousePos = OwningWindow.lock()->GetInput().MousePos;
			io.MousePos = ImVec2((float)MousePos.X, (float)MousePos.Y);
		}
		return true;
	}
	return false;
}

bool CUILayer::OnMouseScrolledEvent(CMouseScrolledEvent& Event)
{
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += Event.GetXOffset();
		io.MouseWheel += Event.GetYOffset();
		return true;
	}
	return false;
}

bool CUILayer::OnKeyPressedEvent(CKeyPressedEvent& Event)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[Event.GetKeyCode()] = true;
	
	// io.KeyCtrl = Event.GetMods() & CInput::ModiferType::Control;
	// io.KeyShift = Event.GetMods() & CInput::ModiferType::Shift;
	// io.KeyAlt = Event.GetMods() & CInput::ModiferType::Alt;
	// TODO: What is super???
	// io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	
	// Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	
	return ImGui::IsWindowFocused(ImGuiHoveredFlags_AnyWindow);
}

bool CUILayer::OnKeyTypedEvent(CKeyTypedEvent& Event)
{
	ImGuiIO& io = ImGui::GetIO();
	if (Event.GetKeyCode() > 0 && Event.GetKeyCode() < 0x10000)
	{
		io.AddInputCharacter((unsigned short)Event.GetKeyCode());
	}
	return ImGui::IsWindowFocused(ImGuiHoveredFlags_AnyWindow);
}

bool CUILayer::OnKeyReleasedEvent(CKeyReleasedEvent& Event)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[Event.GetKeyCode()] = false;
	
	// Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	
	return ImGui::IsWindowFocused(ImGuiHoveredFlags_AnyWindow);
}
