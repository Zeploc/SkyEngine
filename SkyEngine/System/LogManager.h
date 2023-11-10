// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //

// Engine Includes //
#include "Core/Core.h"

class ENGINE_API CLogManager
{
public:
	void Init();

	void Render();

	virtual void DisplayMessage(const std::string& InMessage);
	virtual void DisplayWarning(const std::string& InMessage);
	virtual void DisplayError(const std::string& InMessage);

	// Singleton
public:
	static TPointer<CLogManager> Get();

	CLogManager();
	~CLogManager(); // Shared pointer has to be able to call destructor so can't be private
};
