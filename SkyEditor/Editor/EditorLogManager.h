// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <string>

#include "System/LogManager.h"

class CEditorLogManager : public CLogManager
{
public:
	virtual void LogInternal(ELogMessageType MessageType, const std::string& InMessage) override;
	void ConsoleLogCreated();
	
protected:
	TArray<std::string> QueuedMessages;
};
