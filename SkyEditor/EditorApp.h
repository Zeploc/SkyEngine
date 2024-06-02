// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Application.h"
#include "Core/Core.h"

class CConfigSettingsPanel;
class CKeyPressedEvent;
class CEditorViewportCanvas;
class CConsoleLog;
class EditorApplication;

inline EditorApplication* EditorApp = nullptr;

class EditorApplication : public SkyEngine::Application
{
public:
	// TODO: Swap to shared pointer
	CConsoleLog* ConsoleLog = nullptr;
	
	EditorApplication();
	~EditorApplication() {}
	
	bool ApplicationSetup(std::string ExecutablePath) override;

	// TODO: Remove once material assets setup
	void SetupPlaceholderMaterials();

	void SetupConfigs() override;
	void SetupLogManager() override;
	void SetupViewportLayer() override;
	
	void Update() override;
	void MainMenuBar();

	void Render() override;

	void OnEvent(CEvent& Event) override;
	bool OnKeyPressedEvent(CKeyPressedEvent& Event);
	void OnExit() override;

	void OpenNewScene();
	bool UnsavedSceneCheck();
	bool OpenSceneByPath(std::string FilePath);
	bool OpenScene(TPointer<Scene> NewScene) override;
	void SaveScene(bool bAsNew = false);

	unsigned int DockSpaceID = 0;
	
	CEditorViewportCanvas* EditorViewportLayer;
	CConfigSettingsPanel* ProjectSettingsPanel = nullptr;
	CConfigSettingsPanel* EditorSettingsPanel = nullptr;

	std::string ScenePath;
	std::string ContentPath;
	std::string WindowName;

protected:
	void SetProjectDirectory(std::string ExecutablePath) override;
};