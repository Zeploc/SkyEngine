#include "EditorLogManager.h"

#include "EditorApp.h"
#include "UI/ConsoleLog.h"

void CEditorLogManager::DisplayMessage(const std::string& InMessage)
{
	CLogManager::DisplayMessage(InMessage);
	if (EditorApp->ConsoleLog)
	{
		EditorApp->ConsoleLog->AddLog(std::format("{}\n", InMessage).c_str());
	}
}

void CEditorLogManager::DisplayWarning(const std::string& InMessage)
{
	CLogManager::DisplayWarning(InMessage);
	if (EditorApp->ConsoleLog)
	{
		EditorApp->ConsoleLog->AddLog(std::format("{}\n", InMessage).c_str());
	}
}

void CEditorLogManager::DisplayError(const std::string& InMessage)
{
	CLogManager::DisplayError(InMessage);
	if (EditorApp->ConsoleLog)
	{
		EditorApp->ConsoleLog->AddLog(std::format("{}\n", InMessage).c_str());
	}
}
