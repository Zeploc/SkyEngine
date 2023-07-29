// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //
#include <string>


// Engine Includes //
#include "Core/Core.h"

class UIText;

class LogManager
{
public:
	void Init();

	void Render();

	void DisplayLogMessage(std::string _Message);

	Pointer<UIText> LoadingMessage;

	// Singleton
public:
	static Pointer<LogManager> GetInstance();

	static void DestoryInstance();

	~LogManager(); // Shared pointer has to be able to call destructor so can't be private

private:
	static Pointer<LogManager> m_pLogManager;

	LogManager();

	LogManager(const LogManager&); // Don't Implement
	void operator=(const LogManager&); // Don't implement
};
