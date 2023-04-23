// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //
#include <memory>
#include <string>

// Engine Includes //

class UIText;

class LogManager
{
public:
	void Init();

	void Render();

	void DisplayLogMessage(std::string _Message);

	std::shared_ptr<UIText> LoadingMessage;

	// Singleton
public:
	static std::shared_ptr<LogManager> GetInstance();

	static void DestoryInstance();

	~LogManager(); // Shared pointer has to be able to call destructor so can't be private

private:
	static std::shared_ptr<LogManager> m_pLogManager;

	LogManager();

	LogManager(const LogManager&); // Don't Implement
	void operator=(const LogManager&); // Don't implement
};
