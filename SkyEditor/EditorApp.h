// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Application.h"
#include "Core/Core.h"

class CConsoleLog;
class EditorApplication;

inline EditorApplication* EditorApp = nullptr;

class EditorApplication : public SkyEngine::Application
{
public:	
	TPointer<CConsoleLog> ConsoleLog;
	
	EditorApplication();
	~EditorApplication() {}
	
	bool ApplicationSetup() override;

	void SetupLogManager() override;
	
	void Update() override;

	void Render() override;

	void ChangeSize(int w, int h) override;
	void OnExit() override;
};