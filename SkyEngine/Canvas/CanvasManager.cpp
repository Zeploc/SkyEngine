// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "CanvasManager.h"

#include "Canvas.h"
#include "Core/Application.h"
#include "Dependencies/ImGui/imgui.h"
#include "Platform/Window/GLFW/GLFWWindow.h"
#include "Graphics/GL/imgui_impl_opengl3.h"
#include "System/TimeManager.h"
#include "ImGuizmo.h"
#include "Render/Renderer.h"
#include "System/LogManager.h"

CCanvasManager::CCanvasManager()
{
}

CCanvasManager::~CCanvasManager()
{
	for (const CCanvas* Layer : Canvases)
	{
		delete Layer;
	}
	Canvases.clear();
}

void CCanvasManager::SetupCanvasManager(TPointer<CEngineWindow> InOwningWindow)
{
	OwningWindow = InOwningWindow;
	
	GuiContext = ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
	// TPointer<CGLFWWindow> GraphicsWindow = std::static_pointer_cast<CGLFWWindow>(GetApplication()->GetApplicationWindow());
	// ImGui_ImplGlfw_InitForOpenGL(GraphicsWindow->GetGlWindow(), true);
	
	ImGuiIO& Io = ImGui::GetIO();
	Io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	Io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	Io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	Io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; 
	
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
	GetGraphicsAPI()->ImGuiInit();
}

void CCanvasManager::OnEvent(CEvent& Event)
{	
	EventDispatcher dispatcher(Event);
	dispatcher.Dispatch<CMouseButtonPressedEvent>(SE_BIND_EVENT_FN(CCanvasManager::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<CMouseButtonReleasedEvent>(SE_BIND_EVENT_FN(CCanvasManager::OnMouseButtonReleasedEvent));
	dispatcher.Dispatch<CMouseMovedEvent>(SE_BIND_EVENT_FN(CCanvasManager::OnMouseMovedEvent));
	dispatcher.Dispatch<CMouseScrolledEvent>(SE_BIND_EVENT_FN(CCanvasManager::OnMouseScrolledEvent));
	dispatcher.Dispatch<CKeyPressedEvent>(SE_BIND_EVENT_FN(CCanvasManager::OnKeyPressedEvent));
	dispatcher.Dispatch<CKeyTypedEvent>(SE_BIND_EVENT_FN(CCanvasManager::OnKeyTypedEvent));
	dispatcher.Dispatch<CKeyReleasedEvent>(SE_BIND_EVENT_FN(CCanvasManager::OnKeyReleasedEvent));
	dispatcher.Dispatch<CWindowResizeEvent>(SE_BIND_EVENT_FN(CCanvasManager::OnWindowResizeEvent));
	// TEMP TEST
	// SVector2i MousePos = GetApplication()->GetApplicationWindow()->GetInput().MousePos;
	// return MousePos.X < 500 && MousePos.Y < 500;
	// return ImGui::IsAnyWindowFocused();
	
}

void CCanvasManager::Update()
{
	ImGuiIO& Io  = ImGui::GetIO();
	Io.DeltaTime = CTimeManager::GetDeltaTime();

	// TODO: Any overhead? Can be moved to update on window resize event
	const SVector2 WindowSize = SVector2(GetApplication()->GetApplicationWindow()->GetSize());
	Io.DisplaySize = { WindowSize.X, WindowSize.Y};

	ImGui_ImplOpenGL3_NewFrame();
	// ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	
	// TODO: Once linking events system, would work backwards in layer based on highest first
	for (CCanvas* Canvas : Canvases)
	{
		Canvas->OnUpdate();
	}
}

void CCanvasManager::Render()
{
	static bool show = true;
	// TODO: Temp until overall style setup
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.1f,.1f,.1f,1.0f));
	ImGui::ShowDemoWindow(&show);
	ImGui::PopStyleColor(1);
	
	for (CCanvas* Canvas : Canvases)
	{
		Canvas->Render();
	}
	
	ImGui::Render();

	// TODO: Should the window do this instead?
	GetRenderer()->RenderImGui();

	// TODO: Try link up multi-viewport
	// Update and Render additional Platform Windows
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

void CCanvasManager::PushLayer(CCanvas* InLayer)
{
	Canvases.emplace(Canvases.begin() + LayersInsertIndex, InLayer);
	LayersInsertIndex++;
}

void CCanvasManager::PushOverlay(CCanvas* InOverlay)
{
	Canvases.emplace_back(InOverlay);
}

void CCanvasManager::PopLayer(CCanvas* InLayer)
{
	// Find layer
	const auto it = std::find(Canvases.begin(), Canvases.end(), InLayer);
	// If found
	if (it != Canvases.end())
	{
		Canvases.erase(it);
		LayersInsertIndex--;
	}
}

void CCanvasManager::PopOverlay(CCanvas* InOverlay)
{
	// Find layer
	const auto it = std::find(Canvases.begin(), Canvases.end(), InOverlay);
	// If found
	if (it != Canvases.end())
	{
		Canvases.erase(it);
	}
}

bool CCanvasManager::OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event)
{
	//if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		ImGui::SetWindowFocus();
		return false;
	}
	
	// Update buttons
	ImGuiIO& io = ImGui::GetIO();
	for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
	{
		CWindowInput::InputState MouseButtonState = (CWindowInput::InputState)OwningWindow.lock()->GetInput().MouseState[i];
		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[i] = MouseButtonState == CWindowInput::INPUT_FIRST_PRESS || MouseButtonState == CWindowInput::INPUT_HOLD;
	}

	// Won't be hovered properly since mouse pos only updated in imgui when window is focused
	CLogManager::Get()->DisplayMessage(std::format("wants capture: {}", ImGui::GetIO().WantCaptureMouse));

	// if (!ImGui::GetIO().WantCaptureMouse)
	// {
	// 	ImGui::SetWindowFocus();
	// 	return false;
	// }

	CCanvas* NewCapturedCanvas = SendCanvasEvents(Event);
	if (NewCapturedCanvas)
	{
		CapturedLayer = NewCapturedCanvas;
		return true;
	}	
	
	return false;
}

bool CCanvasManager::OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event)
{	
	const int MouseButton = Event.GetMouseButton();
	const int Mods = Event.GetMods();
	
	bool bHadCapture = ImGui::GetIO().WantCaptureMouse;
	
	// Update buttons
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[MouseButton] = false;

	CCanvas* NewCapturedCanvas = SendCanvasEvents(Event);
	if (NewCapturedCanvas && CapturedLayer == NewCapturedCanvas)
	{
		// TODO: Always or only on capture?
		CapturedLayer = nullptr;
		return true;
	}
	
	// If hovering window on release or mouse is not released (captured by button), then the release is handled
	return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || !ImGui::IsMouseReleased(MouseButton);
}

bool CCanvasManager::OnMouseMovedEvent(CMouseMovedEvent& Event)
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

		SendCanvasEvents(Event);		
		return Event.WasHandled();
	}
	return false;
}

bool CCanvasManager::OnMouseScrolledEvent(CMouseScrolledEvent& Event)
{
	const float XOffset = Event.GetXOffset();
	const float YOffset = Event.GetYOffset();	
	
	// Move to higher level
	//if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	if (ImGui::GetIO().WantCaptureMouse)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += XOffset;
		io.MouseWheel += YOffset;
		
		SendCanvasEvents(Event);		
		return Event.WasHandled();
	}
	return false;
}

bool CCanvasManager::OnKeyPressedEvent(CKeyPressedEvent& Event)
{
	const int KeyCode = Event.GetKeyCode();
	const int Mods = Event.GetMods();
	const int RepeatCount = Event.GetRepeatCount();
	
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[KeyCode] = true;
	
	// io.KeyCtrl = Event.GetMods() & CWindowInput::ModiferType::Control;
	// io.KeyShift = Event.GetMods() & CWindowInput::ModiferType::Shift;
	// io.KeyAlt = Event.GetMods() & CWindowInput::ModiferType::Alt;
	// TODO: Windows Key?
	// io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	
	// Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

	CLogManager::Get()->DisplayMessage(std::format("wants keyboard capture: {}", ImGui::GetIO().WantCaptureKeyboard));
	
	SendCanvasEvents(Event);		
	if (Event.WasHandled())
	{
		return true;
	}
	
	// If window is focused (not viewport), have keyboard input go there instead
	return ImGui::GetIO().WantCaptureKeyboard || ImGui::IsWindowFocused(ImGuiHoveredFlags_AnyWindow);
}

bool CCanvasManager::OnKeyTypedEvent(CKeyTypedEvent& Event)
{
	const int KeyCode = Event.GetKeyCode();
	const int Mods = Event.GetMods();
	
	ImGuiIO& io = ImGui::GetIO();
	if (KeyCode > 0 && KeyCode < 0x10000)
	{
		io.AddInputCharacter((unsigned short)KeyCode);
	}
	
	SendCanvasEvents(Event);		
	if (Event.WasHandled())
	{
		return true;
	}
	
	return ImGui::GetIO().WantCaptureKeyboard || ImGui::IsWindowFocused(ImGuiHoveredFlags_AnyWindow);
}

bool CCanvasManager::OnKeyReleasedEvent(CKeyReleasedEvent& Event)
{
	const int KeyCode = Event.GetKeyCode();
	const int Mods = Event.GetMods();
	
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[KeyCode] = false;
	
	// Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	
	SendCanvasEvents(Event);		
	if (Event.WasHandled())
	{
		return true;
	}
	
	return ImGui::GetIO().WantCaptureKeyboard || ImGui::IsWindowFocused(ImGuiHoveredFlags_AnyWindow);	
}

bool CCanvasManager::OnWindowResizeEvent(CWindowResizeEvent& Event)
{
	// TODO: Switch window resize event to update Imgui here?
	
	SendCanvasEvents(Event);		
	if (Event.WasHandled())
	{
		return true;
	}
	
	return false;
}

CCanvas* CCanvasManager::SendCanvasEvents(CEvent& Event)
{
	if (CapturedLayer)
	{
		CapturedLayer->OnEvent(Event);
		return CapturedLayer;
	}
	for (auto it = end(); it != begin();)
	{
		(*--it)->OnEvent(Event);
		if (Event.WasHandled())
		{
			return *it;
		}
	}
	return nullptr;
}
