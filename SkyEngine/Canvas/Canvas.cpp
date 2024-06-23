// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Canvas.h"

#include "imgui_internal.h"
#include "Dependencies/ImGui/imgui.h"
#include "Input/Input.h"
#include "Platform/Window/EngineWindow.h"
#include "System/LogManager.h"

CCanvas::CCanvas(TWeakPointer<CEngineWindow> InOwningWindow, const std::string& InCanvasName)
: OwningWindow(InOwningWindow), CanvasName(InCanvasName)
{
	StartingSize = {400, 600};
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

void CCanvas::Render()
{
	if (!bOpen && bCanClose)
	{
		return;
	}
	if (!PreRender())
	{
		ImGui::End();
		// TODO: Error message
		return;
	}
	OnRender();
	PostRender();
}

bool CCanvas::PreRender()
{
	ImGui::SetNextWindowSize(ImVec2(StartingSize.X,StartingSize.Y), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(CanvasName.c_str(), &bOpen))
	{
		return false;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
	return true;
}

void CCanvas::PostRender()
{
	ImGui::PopStyleVar();
	ImGui::End();
}

bool CCanvas::OnMouseButtonPressed(int MouseButton, int Mods)
{	
	return true;
}

bool CCanvas::OnMouseButtonReleased(int MouseButton, int Mods)
{
	return true;
}

bool CCanvas::OnMouseMoved(SVector2i MousePos)
{
	return false;
}

bool CCanvas::OnMouseScrolled(float XOffset, float YOffset)
{
	return false;
}

bool CCanvas::OnKeyPressed(int KeyCode, int Mods, int RepeatCount)
{
	return false;
}

bool CCanvas::OnKeyTyped(int KeyCode, int Mods)
{
	return false;
}

bool CCanvas::OnKeyReleased(int KeyCode, int Mods)
{
	return false;
}

bool CCanvas::OnWindowResize(unsigned Width, unsigned Height)
{	
	return false;
}

bool CCanvas::OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event)
{
	ImGuiWindow* CanvasWindow = ImGui::FindWindowByName(CanvasName.c_str());	
	if (!CanvasWindow)
	{
		return false;
	}
	if (!CanvasWindow->Active)
	{
		return false;
	}
	// if (!CanvasWindow->DockTabIsVisible)
	// {
	// 	return false;
	// }
	if (!ImGui::IsMouseHoveringRect(CanvasWindow->Rect().Min, CanvasWindow->Rect().Max, false))
	{
		return false;
	}
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
