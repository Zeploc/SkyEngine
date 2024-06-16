// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Canvas/UICanvas.h"
#include "Dependencies/ImGui/imgui.h"
#include "System/LogManager.h"

class CConsoleLog : public CUICanvas
{
public:
	CConsoleLog(TWeakPointer<CEngineWindow> InOwningWindow);
	void Clear();
	void AddLog(ELogMessageType MessageType, const char* fmt, ...);
	virtual void OnRender() override;

protected:
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset
	bool                ScrollToBottom;
};
