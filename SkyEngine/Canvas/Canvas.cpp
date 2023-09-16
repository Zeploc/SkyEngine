// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Canvas.h"

#include "Dependencies/ImGui/imgui.h"
#include "Input/Input.h"
#include "Platform/Window/EngineWindow.h"
#include "System/LogManager.h"

CCanvas::CCanvas(TWeakPointer<CEngineWindow> InOwningWindow, const std::string& InDebugName)
: OwningWindow(InOwningWindow), DebugName(InDebugName)
{
}

CCanvas::~CCanvas()
{
}

void CCanvas::OnEvent(CEvent& Event)
{
	EventDispatcher dispatcher(Event);
	dispatcher.Dispatch<CMouseButtonPressedEvent>(SE_BIND_EVENT_FN(CCanvas::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<CMouseButtonReleasedEvent>(SE_BIND_EVENT_FN(CCanvas::OnMouseButtonReleasedEvent));
	dispatcher.Dispatch<CMouseMovedEvent>(SE_BIND_EVENT_FN(CCanvas::OnMouseMovedEvent));
	dispatcher.Dispatch<CMouseScrolledEvent>(SE_BIND_EVENT_FN(CCanvas::OnMouseScrolledEvent));
	dispatcher.Dispatch<CKeyPressedEvent>(SE_BIND_EVENT_FN(CCanvas::OnKeyPressedEvent));
	dispatcher.Dispatch<CKeyTypedEvent>(SE_BIND_EVENT_FN(CCanvas::OnKeyTypedEvent));
	dispatcher.Dispatch<CKeyReleasedEvent>(SE_BIND_EVENT_FN(CCanvas::OnKeyReleasedEvent));
	dispatcher.Dispatch<CWindowResizeEvent>(SE_BIND_EVENT_FN(CCanvas::OnWindowResizeEvent));
	// TEMP TEST
	// SVector2i MousePos = GetApplication()->GetApplicationWindow()->GetInput().MousePos;
	// return MousePos.X < 500 && MousePos.Y < 500;
	// return ImGui::IsAnyWindowFocused();
}

bool CCanvas::OnMouseButtonPressed(int Button, int Mods)
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
		CInput::InputState MouseButtonState = (CInput::InputState)OwningWindow.lock()->GetInput().MouseState[i];
		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[i] = MouseButtonState == CInput::INPUT_FIRST_PRESS || MouseButtonState == CInput::INPUT_HOLD;
	}

	// Won't be hovered properly since mouse pos only updated in imgui when window is focused
	CLogManager::GetInstance()->DisplayLogMessage(std::format("wants capture: {}", ImGui::GetIO().WantCaptureMouse));

	// if (!ImGui::GetIO().WantCaptureMouse)
	// {
	// 	ImGui::SetWindowFocus();
	// 	return false;
	// }
	return true;
}

bool CCanvas::OnMouseButtonReleased(int Button, int Mods)
{
	bool bHadCapture = ImGui::GetIO().WantCaptureMouse;
	
	// Update buttons
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[Button] = false;

	// If hovering window on release or mouse is not released (captured by button), then the release is handled
	return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || !ImGui::IsMouseReleased(Button);
}

bool CCanvas::OnMouseMoved(SVector2i MousePos)
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

bool CCanvas::OnMouseScrolled(float XOffset, float YOffset)
{
	//if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	if (ImGui::GetIO().WantCaptureMouse)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += XOffset;
		io.MouseWheel += YOffset;
		return true;
	}
	return false;
}

bool CCanvas::OnKeyPressed(int KeyCode, int Mods, int RepeatCount)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[KeyCode] = true;
	
	// io.KeyCtrl = Event.GetMods() & CInput::ModiferType::Control;
	// io.KeyShift = Event.GetMods() & CInput::ModiferType::Shift;
	// io.KeyAlt = Event.GetMods() & CInput::ModiferType::Alt;
	// TODO: Windows Key?
	// io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	
	// Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

	CLogManager::GetInstance()->DisplayLogMessage(std::format("wants keyboard capture: {}", ImGui::GetIO().WantCaptureKeyboard));
	// If window is focused (not viewport), have keyboard input go there instead
	return ImGui::GetIO().WantCaptureKeyboard || ImGui::IsWindowFocused(ImGuiHoveredFlags_AnyWindow);
}

bool CCanvas::OnKeyTyped(int KeyCode, int Mods)
{
	ImGuiIO& io = ImGui::GetIO();
	if (KeyCode > 0 && KeyCode < 0x10000)
	{
		io.AddInputCharacter((unsigned short)KeyCode);
	}
	return ImGui::GetIO().WantCaptureKeyboard || ImGui::IsWindowFocused(ImGuiHoveredFlags_AnyWindow);
}

bool CCanvas::OnKeyReleased(int KeyCode, int Mods)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[KeyCode] = false;
	
	// Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	
	return ImGui::GetIO().WantCaptureKeyboard || ImGui::IsWindowFocused(ImGuiHoveredFlags_AnyWindow);
}

bool CCanvas::OnWindowResize(unsigned Width, unsigned Height)
{
	// TODO: Switch window resize event to update Imgui here?
	
	return false;
}

bool CCanvas::OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event)
{
	return OnMouseButtonPressed(Event.GetMouseButton(), Event.GetMods());
}

bool CCanvas::OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event)
{
	return OnMouseButtonReleased(Event.GetMouseButton(), Event.GetMods());
}

bool CCanvas::OnMouseMovedEvent(CMouseMovedEvent& Event)
{
	return OnMouseMoved(Event.GetMousePos());
}

bool CCanvas::OnMouseScrolledEvent(CMouseScrolledEvent& Event)
{
	return OnMouseScrolled(Event.GetXOffset(), Event.GetYOffset());
}

bool CCanvas::OnKeyPressedEvent(CKeyPressedEvent& Event)
{
	return OnKeyPressed(Event.GetKeyCode(), Event.GetMods(), Event.GetRepeatCount());
}

bool CCanvas::OnKeyTypedEvent(CKeyTypedEvent& Event)
{
	return OnKeyTyped(Event.GetKeyCode(), Event.GetMods());
}

bool CCanvas::OnKeyReleasedEvent(CKeyReleasedEvent& Event)
{
	return OnKeyReleased(Event.GetKeyCode(), Event.GetMods());
}

bool CCanvas::OnWindowResizeEvent(CWindowResizeEvent& Event)
{
	return OnWindowResize(Event.GetWidth(), Event.GetHeight());
}
