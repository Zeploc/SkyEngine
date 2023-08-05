// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //

// Engine Includes //
#include "Core/Core.h"

class UIText;

class LogManager
{
public:
	void Init();

	void Render();

	void DisplayLogMessage(const std::string& InMessage);
	void DisplayLogWarning(const std::string& InMessage);
	void DisplayLogError(const std::string& InMessage);

	TPointer<UIText> LoadingMessage;

	// Singleton
public:
	static TPointer<LogManager> GetInstance();

	static void DestoryInstance();

	~LogManager(); // Shared pointer has to be able to call destructor so can't be private

private:
	static TPointer<LogManager> m_pLogManager;

	LogManager();

	LogManager(const LogManager&); // Don't Implement
	void operator=(const LogManager&); // Don't implement
};
