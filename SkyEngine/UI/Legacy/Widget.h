// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Local Includes //
#include "UIElement.h"

// Library Includes //

class Widget : public UIElement
{
public:
	Widget();

	~Widget();

	void LoadFromText(std::string WidgetName);

	void SaveText(std::string WidgetName);

	std::vector<TPointer<UIElement>> UIElements;
};
