// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "CanvasManager.h"

#include "Canvas.h"
#include "Dependencies/ImGui/imgui.h"
#include "Platform/Window/GLFW/GLFWWindow.h"
#include "Graphics/GL/imgui_impl_opengl3.h"

CCanvasManager::CCanvasManager()
{
}

CCanvasManager::~CCanvasManager()
{
	for (const CCanvas* Layer : Layers)
	{
		delete Layer;
	}
	Layers.clear();
}

void CCanvasManager::SetupCanvasManager()
{
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
	ImGui_ImplOpenGL3_Init("#version 410 core");
}

void CCanvasManager::PushLayer(CCanvas* InLayer)
{
	Layers.emplace(Layers.begin() + LayersInsertIndex, InLayer);
	LayersInsertIndex++;
}

void CCanvasManager::PushOverlay(CCanvas* InOverlay)
{
	Layers.emplace_back(InOverlay);
}

void CCanvasManager::PopLayer(CCanvas* InLayer)
{
	// Find layer
	const auto it = std::find(Layers.begin(), Layers.end(), InLayer);
	// If found
	if (it != Layers.end())
	{
		Layers.erase(it);
		LayersInsertIndex--;
	}
}

void CCanvasManager::PopOverlay(CCanvas* InOverlay)
{
	// Find layer
	const auto it = std::find(Layers.begin(), Layers.end(), InOverlay);
	// If found
	if (it != Layers.end())
	{
		Layers.erase(it);
	}
}
