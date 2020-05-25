#pragma once
#include "UIElement.h"

#include <vector>

class Widget :	public UIElement
{
public:
	Widget();
	~Widget();

	void LoadFromText(std::string WidgetName);
	void SaveText(std::string WidgetName);

	std::vector<std::shared_ptr<UIElement>> UIElements;
};

