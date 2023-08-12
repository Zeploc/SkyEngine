// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <string>

#include "System/LogManager.h"

class CEditorLogManager : public CLogManager
{
public:
	void DisplayLogMessage(const std::string& InMessage) override;
	void DisplayLogWarning(const std::string& InMessage) override;
	void DisplayLogError(const std::string& InMessage) override;
	
};
