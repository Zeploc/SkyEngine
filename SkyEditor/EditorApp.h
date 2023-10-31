// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Application.h"
#include "Core/Core.h"

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
	
	bool ApplicationSetup() override;

	void SetupLogManager() override;
	void SetupViewportLayer() override;
	
	void Update() override;

	void Render() override;

	void ChangeSize(int w, int h) override;
	void OnExit() override;

	unsigned int DockSpaceID = 0;
	
	CEditorViewportCanvas* EditorViewportLayer;
};