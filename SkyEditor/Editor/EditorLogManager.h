// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <string>

#include "System/LogManager.h"

class CEditorLogManager : public CLogManager
{
public:
	void DisplayMessage(const std::string& InMessage) override;
	void DisplayWarning(const std::string& InMessage) override;
	void DisplayError(const std::string& InMessage) override;
	void ConsoleLogCreated();
	
protected:
	TArray<std::string> QueuedMessages;
};
