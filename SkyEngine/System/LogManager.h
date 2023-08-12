// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //

// Engine Includes //
#include "Core/Core.h"

class UIText;

class ENGINE_API CLogManager
{
public:
	void Init();

	void Render();

	virtual void DisplayLogMessage(const std::string& InMessage);
	virtual void DisplayLogWarning(const std::string& InMessage);
	virtual void DisplayLogError(const std::string& InMessage);

	TPointer<UIText> LoadingMessage;

	// Singleton
public:
	static TPointer<CLogManager> GetInstance();

	CLogManager();
	~CLogManager(); // Shared pointer has to be able to call destructor so can't be private
};
