#include "EditorLogManager.h"

#include "EditorApp.h"
#include "UI/ConsoleLog.h"

void CEditorLogManager::DisplayLogMessage(const std::string& InMessage)
{
	CLogManager::DisplayLogMessage(InMessage);
	if (EditorApp->ConsoleLog)
	{
		EditorApp->ConsoleLog->AddLog(std::format("{}\n", InMessage).c_str());
	}
}

void CEditorLogManager::DisplayLogWarning(const std::string& InMessage)
{
	CLogManager::DisplayLogWarning(InMessage);
	if (EditorApp->ConsoleLog)
	{
		EditorApp->ConsoleLog->AddLog(std::format("{}\n", InMessage).c_str());
	}
}

void CEditorLogManager::DisplayLogError(const std::string& InMessage)
{
	CLogManager::DisplayLogError(InMessage);
	if (EditorApp->ConsoleLog)
	{
		EditorApp->ConsoleLog->AddLog(std::format("{}\n", InMessage).c_str());
	}
}
