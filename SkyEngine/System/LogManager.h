// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //

// Engine Includes //
#include "Core/Core.h"

enum class ELogMessageType
{
	Message,
	Warning,
	Error
};

class ENGINE_API CLogManager
{
public:
	void Init();

	void Render();
	virtual void LogInternal(ELogMessageType MessageType, const std::string& InMessage);

	void Log(ELogMessageType MessageType, const std::string& InMessage);
	
	virtual void DisplayMessage(const std::string& InMessage);
	virtual void DisplayWarning(const std::string& InMessage);
	virtual void DisplayError(const std::string& InMessage);

	// Singleton
public:
	static TSharedPointer<CLogManager> Get();

	CLogManager();
	~CLogManager(); // Shared pointer has to be able to call destructor so can't be private
};
